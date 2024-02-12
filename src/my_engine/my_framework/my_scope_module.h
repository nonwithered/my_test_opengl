#pragma once

#include "my_framework/my_live_module.h"

template<typename ...T>
class ScopeModule: public LiveModule {

    static constexpr auto TAG = "ScopeModule";

private:

    using tuple_type = typename std::tuple<std::weak_ptr<T>...>;

    template<size_t N>
    struct element {
        using ptr = typename std::tuple_element<N, tuple_type>::type;
        using type = typename ptr::element_type;
    };

    tuple_type data_;

    template<size_t N>
    bool alive() {
        return (bool) data<N>() && (bool) data<N - 1>();
    }

    template<>
    bool alive<0>() {
        return (bool) data<0>();
    }

    bool alive(Global &context) final {
        return alive<std::tuple_size<tuple_type>::value - 1>();
    }

protected:

    template<size_t N>
    std::shared_ptr<typename element<N>::type> data() {
        return std::get<N>(data_).lock();
    }

public:
    ScopeModule(std::weak_ptr<T>... data) : LiveModule(), data_(std::move(data)...) {
    }

    ~ScopeModule() = default;
};
