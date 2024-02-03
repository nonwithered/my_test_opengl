#pragma once

#include <log.h>

class Module;

class Context {

private:
    Context(const Context &) = delete;

protected:
    Context(Context &&) = default;
    Context() = default;

public:
    virtual ~Context() = default;

    virtual int width() = 0;

    virtual int height() = 0;

    virtual const std::string &title() = 0;

    virtual void Close() = 0;

    virtual void NewModule(std::unique_ptr<Module> create) = 0;

};

class Module {

private:
    static constexpr auto TAG = "Module";
    bool finish_ = false;

    std::vector<std::unique_ptr<Module>> pending_modules_;
    std::vector<std::unique_ptr<Module>> modules_;

    Module(const Module &) = delete;
    Module(Module &&) = delete;

protected:
    virtual void Frame(Context &context) {
    }

    virtual void Event(Context &context, int key, bool press) {
    }

    void Close() {
        finish_ = true;
    }

public:
    Module() = default;

    virtual ~Module() = default;

    bool IsClosed() {
        return finish_;
    }

    void NewModule(std::unique_ptr<Module> module) {
        LOGI(TAG, "NewModule");
        pending_modules_.push_back(std::move(module));
    }

    void PerformEvent(Context &context, int key, bool press) {
        for (auto i = modules_.begin(); i != modules_.end(); ++i) {
            auto &module = *i;
            module->Event(context, key, press);
        }
    }

    void PerformFrame(Context &context) {
        for (auto i = pending_modules_.begin(); i != pending_modules_.end(); ) {
            auto &module = *i;
            modules_.push_back(std::move(module));
            i = pending_modules_.erase(i);
        }
        Frame(context);
        for (auto i = modules_.begin(); i != modules_.end(); ) {
            auto &module = *i;
            if (module->IsClosed()) {
                LOGI(TAG, "close module");
                i = modules_.erase(i);
            } else {
                ++i;
                module->PerformFrame(context);
            }
        }
    }

};
