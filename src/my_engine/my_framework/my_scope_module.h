#pragma once

#include "my_framework/my_live_module.h"

template<typename T>
class ScopeModule: public LiveModule {

    static constexpr auto TAG = "ScopeModule";

private:

    std::weak_ptr<T> data_;

    bool alive() final {
        return (bool) data();
    }

protected:

    std::shared_ptr<T> data() {
        return data_.lock();
    }

public:
    ScopeModule(std::weak_ptr<T> data) : LiveModule(), data_(std::move(data)) {
    }

    ~ScopeModule() = default;
};
