#pragma once

#include "my_utils/log.h"

#include "my_model/my_level.h"

class LevelManager : public LevelCleaner {

private:

    static constexpr auto TAG = "LevelManager";

    LevelManager(const LevelManager &) = delete;
    LevelManager(LevelManager &&) = delete;

    std::vector<std::shared_ptr<Level>> level_;

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

public:
    LevelManager() {
        LOGI(TAG, "ctor");
    }

    ~LevelManager() {
        LOGI(TAG, "dtor");
    }

    void StartLevel(std::shared_ptr<Level> level) {
        if (!level) {
            LOGE(TAG, "StartLevel nullptr");
            throw std::exception();
        }
        LOGI(TAG, "StartLevel %s", level->name().data());
        for (auto &i : level_) {
            if (i == level) {
                LOGW(TAG, "StartLevel duplicate");
                return;
            }
        }
        level->cleaner_ = this;
        level_.push_back(std::move(level));
    }

    bool empty() const {
        return level_.empty();
    }
};
