#pragma once

#include "my_utils/log.h"

#include "my_model/my_actor.h"

class Level {

private:

    static constexpr auto TAG = "Level";

    Level(const Level &) = delete;
    Level(Level &&) = delete;

    Actor actor_;

public:
    Level() = default;

    Actor &actor() {
        return actor_;
    }

};
