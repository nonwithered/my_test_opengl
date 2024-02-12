#pragma once

#include "my_utils/my_log.h"
#include "my_utils/my_cast.h"

#include "my_model/my_actor.h"

#include "my_framework/my_context.h"
#include "my_framework/my_window_manager.h"

#include "my_player/my_player_manager.h"

class LevelCleaner {

    friend class Level;

private:
    LevelCleaner(const LevelCleaner &) = delete;
    LevelCleaner(LevelCleaner &&) = delete;

    virtual void OnLevelFinish(std::weak_ptr<Level>) = 0;

    virtual operator Global &() = 0;

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

class Level : public Model<Level>, public PlayerManager {

    friend class LevelManager;

private:

    static constexpr auto TAG = "Level";

    Level(const Level &) = delete;
    Level(Level &&) = delete;

    const std::string name_ = 0;

    std::shared_ptr<LevelActor> actor_ = Model<LevelActor>::Make();

    bool finish_ = false;

    LevelCleaner *cleaner_ = nullptr;

    PlayerManager player_;

    std::function<void(std::shared_ptr<PlayerController>)> new_player_;
    std::unique_ptr<std::pair<Context *, std::vector<Context *>>> require_window_;

    void PerformFinish() {
        if (finish_) {
            LOGW(TAG, "Finish duplicate %s", name_.data());
            return;
        }
        LOGI(TAG, "Finish");
        finish_ = true;
        if (!cleaner_) {
            LOGE(TAG, "Finish invalid %s", name_.data());
            throw std::exception();
        }
        cleaner_->OnLevelFinish(self());
    }

    void PerformStart() {
        new_player_ = [this](auto controller) {
            PlayerManager::NewPlayer(std::move(controller));
        };
        OnStart();
        new_player_ = nullptr;
    }

    void PerformResume() {
        require_window_ = std::make_unique<decltype(require_window_)::element_type>();
        OnResume();
        require_window_->second.push_back(require_window_->first);
        global().window().EnsureSurvivor(require_window_->second);
        require_window_.reset();
    }

    void PerformPause() {
        OnPause();
    }

protected:

    void OnCreate() override {
        LOGI(TAG, "OnCreate %s", name_.data());
        actor_->level_ = self();
        actor_->name(name());
    }

    virtual void OnStart() {
    }

    virtual void OnResume() {
    }

    virtual void OnPause() {
        Collect([](PlayerController &controller) {
            auto player = TypeCast<BaseLocalPlayerController>(&controller);
            if (player) {
                player->clear();
            }
        });
    }

    template<typename T, typename ...Args>
    std::weak_ptr<T> NewPlayer(Args... args) {
        if (!new_player_) {
            LOGE(TAG, "NewPlayer invalid");
            throw std::exception();
        }
        auto type_name = typeid(T).name();
        LOGI(TAG, "NewPlayer %s", type_name);
        auto player = Model<T>::Make(std::forward<Args>(args)...);
        auto weak = player;
        new_player_(std::move(player));
        return weak;
    }

    Context &RequireWindow(const std::string &title, int width = 0, int height = 0) {
        if (!require_window_) {
            LOGE(TAG, "RequireWindow invalid");
            throw std::exception();
        }
        if (width == 0 && height == 0) {
            auto &context = global().window().RequirePrimary(title);
            require_window_->first = &context;
            return context;
        }
        auto &context = global().window().RequireIndex(require_window_->second.size(), title, width, height);
        require_window_->second.push_back(&context);
        return context;
    }

public:

    Level(const std::string &name)
    : Model(), PlayerManager(), name_(name) {
        LOGI(TAG, "ctor %s", name_.data());
    }

    ~Level() {
        LOGI(TAG, "dtor %s", name_.data());
    }

    const std::string &name() const {
        return name_;
    }

    LevelActor &actor() {
        return *actor_;
    }

    Global &global() {
        return *cleaner_;
    }

};
