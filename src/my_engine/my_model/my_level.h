#pragma once

#include "my_utils/log.h"

#include "my_model/my_actor.h"

class LevelCleaner {

    friend class Level;

private:
    LevelCleaner(const LevelCleaner &) = delete;
    LevelCleaner(LevelCleaner &&) = delete;

    virtual void OnLevelFinish(std::weak_ptr<Level>) = 0;

protected:
    LevelCleaner() = default;

public:
    virtual ~LevelCleaner() = default;

};

class Level;

class LevelActor : public Actor {

    friend class Level;

private:
    std::weak_ptr<Level> level_;

public:
    std::shared_ptr<Level> level() {
        return level_.lock();
    }
};

class Level : public Model<Level> {

    friend class LevelManager;

private:

    static constexpr auto TAG = "Level";

    Level(const Level &) = delete;
    Level(Level &&) = delete;

    const std::string name_ = 0;

    std::shared_ptr<LevelActor> actor_ = Model<LevelActor>::Make();

    bool finish_ = false;

    LevelCleaner *cleaner_ = nullptr;

protected:

    void OnCreate() override {
        LOGI(TAG, "OnCreate %s", name_.data());
        actor_->level_ = self();
        actor_->name(name());
    }

public:

    Level(const std::string &name)
    : name_(name) {
        LOGI(TAG, "ctor %s", name_.data());
    }

    virtual ~Level() {
        LOGI(TAG, "dtor %s", name_.data());
    }

    const std::string &name() const {
        return name_;
    }

    LevelActor &actor() {
        return *actor_;
    }

    void Finish() {
        if (finish_) {
            LOGW(TAG, "Finish duplicate %s", name_.data());
            return;
        }
        finish_ = true;
        if (!cleaner_) {
            LOGE(TAG, "Finish invalid %s", name_.data());
            throw std::exception();
        }
        cleaner_->OnLevelFinish(self());
    }

};
