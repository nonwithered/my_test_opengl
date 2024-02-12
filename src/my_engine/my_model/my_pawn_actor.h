#pragma once

#include "my_model/my_movement_component.h"
#include "my_player/my_player_controller.h"

class PawnActor : public Actor {

private:

    static constexpr auto TAG = "PawnActor";

    std::weak_ptr<PlayerController> player_;

protected:

    void OnCreate() override {
        Actor::OnCreate();
        insert(Model<MovementComponent>::Make());
    }

public:
    std::shared_ptr<PlayerController> player() {
        return player_.lock();
    }

    void player(std::shared_ptr<PlayerController> controller) {
        player_ = controller;
    }

};
