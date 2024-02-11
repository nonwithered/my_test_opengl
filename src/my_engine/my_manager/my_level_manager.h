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

    void OnLevelFinish(std::weak_ptr<Level> level) override {
        auto p = level.lock();
        if (!p) {
            LOGE(TAG, "OnLevelFinish invalid");
            throw std::exception();
        }
        for (auto i = level_.begin(); i != level_.end(); ) {
            auto &p_ = *i;
            if (p == p_) {
                i = level_.erase(i);
            } else {
                ++i;
            }
        }
    }

    void PerformFrame(std::function<void(Module &)>frame) {
        auto level = current();
        if (!level) {
            return;
        }
        level->player_.PerformFrame(global_, frame);
    }

    void PerformKeyEvent(Context &context, int key, bool press) {
        auto level = current();
        if (!level) {
            return;
        }
        return level->player_.PerformKeyEvent(context, key, press);
    }

    void PerformMouseButtonEvent(Context &context, int button, bool press) {
        auto level = current();
        if (!level) {
            return;
        }
        return level->player_.PerformMouseButtonEvent(context, button, press);
    }

public:
    LevelManager(Global &global) : global_(global) {
        LOGI(TAG, "ctor");
    }

    ~LevelManager() {
        LOGI(TAG, "dtor");
    }

    template<typename T, typename ...Args>
    void StartLevel(Args... args) {
        auto level = Model<T>::Make(std::forward<Args>(args)...);
        if (!level) {
            LOGE(TAG, "StartLevel nullptr");
            throw std::exception();
        }
        LOGI(TAG, "StartLevel %s %s", level->name().data(), typeid(T).name());
        for (auto &i : level_) {
            if (i == level) {
                LOGW(TAG, "StartLevel duplicate");
                return;
            }
        }
        level->cleaner_ = this;
        std::weak_ptr<Level> weak = level;
        level_.push_front(level);
        ((Level &) *level).PerformStart(global_);
    }

    std::shared_ptr<Level> current() const {
        if (level_.empty()) {
            return nullptr;
        }
        return level_.front();
    }
};
