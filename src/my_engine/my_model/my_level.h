#pragma once

#include "my_utils/my_log.h"
#include "my_utils/my_cast.h"

#include "my_model/my_actor.h"

#include "my_framework/my_context.h"

#include "my_player/my_player_controller.h"
#include "my_player/my_local_player_controller.h"

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

    std::vector<std::unique_ptr<PlayerController>> controller_;

    void PerformStart(Global &context) {
        OnStart(context, controller_);
    }

    void PerformFrame(Global &context_, std::function<void(Module &)>frame) {
        for (auto &controller : controller_) {
            controller->Frame(context_);
        }
        for (auto &controller : controller_) {
            auto *p = TypeCast<LocalPlayerController>(controller.get());
            if (p) {
                frame(p->module());
            }
        }
    }

    void OnFramebufferSize(Context &context, int width, int height) {
    }

    void PerformKeyEvent(Context &context, int key, bool press) {
        for (auto &controller : controller_) {
            auto *p = TypeCast<LocalPlayerController>(controller.get());
            if (p) {
                p->module().PerformKeyEvent(context, key, press);
            }
        }
    }

    void PerformMouseButtonEvent(Context &context, int button, bool press) {
        for (auto &controller : controller_) {
            auto *p = TypeCast<LocalPlayerController>(controller.get());
            if (p) {
                p->module().PerformMouseButtonEvent(context, button, press);
            }
        }
    }

protected:

    using vector_controller_t = typename std::vector<std::unique_ptr<PlayerController>>;

    void OnCreate() override {
        LOGI(TAG, "OnCreate %s", name_.data());
        actor_->level_ = self();
        actor_->name(name());
    }

    virtual void OnStart(Global &context, vector_controller_t &controller) {
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
