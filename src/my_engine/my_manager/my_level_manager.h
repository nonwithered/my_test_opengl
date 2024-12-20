#pragma once

#include "my_utils/my_log.h"

#include "my_model/my_level.h"

class Global;

class LevelManager : public LevelCleaner {

    friend class Runtime;

private:

    static constexpr auto TAG = "LevelManager";

    LevelManager(const LevelManager &) = delete;
    LevelManager(LevelManager &&) = delete;

    std::list<std::shared_ptr<Level>> level_;

    Global &global_;

    operator Global &() override {
        return global_;
    }

    void OnLevelFinish(std::weak_ptr<Level> level) override {
        auto p = level.lock();
        if (!p) {
            LOGE(TAG, "OnLevelFinish invalid");
            throw std::exception();
        }
        auto last_ = current();
        for (auto i = level_.begin(); i != level_.end(); ) {
            auto &p_ = *i;
            if (p == p_) {
                i = level_.erase(i);
            } else {
                ++i;
            }
        }
        auto current_ = current();
        if (last_ != current_) {
            if (last_) {
                last_->PerformPause();
            }
            if (current_) {
                current_->PerformResume();
            }
        }
    }

    void PerformFrame(std::function<void(Module &)>frame) {
        auto level = current();
        if (!level) {
            return;
        }
        if (level->PerformFrame(global_, frame)) {
            level->PerformFinish();
        }
    }

    void PerformFramebufferSize(Context &context, int width, int height) {
        auto level = current();
        if (!level) {
            return;
        }
        return level->PerformFramebufferSize(context, width, height);
    }

    void PerformKeyEvent(Context &context, int key, bool press) {
        auto level = current();
        if (!level) {
            return;
        }
        return level->PerformKeyEvent(context, key, press);
    }

    void PerformMouseButtonEvent(Context &context, int button, bool press) {
        auto level = current();
        if (!level) {
            return;
        }
        return level->PerformMouseButtonEvent(context, button, press);
    }

    void OnWindowClose(Context &context) {
        auto level = current();
        if (!level) {
            return;
        }
        LOGI(TAG, "OnWindowClose %s", context.title().data());
        return level->OnWindowClose(context);
    }

public:
    LevelManager(Global &global) : global_(global) {
        LOGI(TAG, "ctor");
    }

    ~LevelManager() {
        LOGI(TAG, "dtor");
    }

    template<typename T, typename ...Args>
    std::weak_ptr<T> StartLevel(Args... args) {
        auto level = Model<T>::Make(std::forward<Args>(args)...);
        if (!level) {
            LOGE(TAG, "StartLevel nullptr");
            throw std::exception();
        }
        LOGI(TAG, "StartLevel %s %s", level->name().data(), typeid(T).name());
        for (auto &i : level_) {
            if (i == level) {
                LOGW(TAG, "StartLevel duplicate");
                return level;
            }
        }

        auto last_ = current();
        if (last_) {
            last_->PerformPause();
        }

        level->cleaner_ = this;
        std::weak_ptr<Level> weak = level;
        level_.push_front(level);
        Level &current_ = *level;
        current_.PerformStart();
        current_.PerformResume();
        return level;
    }

    std::shared_ptr<Level> current() const {
        if (level_.empty()) {
            return nullptr;
        }
        return level_.front();
    }
};
