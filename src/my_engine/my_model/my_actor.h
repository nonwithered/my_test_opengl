#pragma once

#include "my_utils/log.h"

#include "my_model/my_model.h"

#include "my_model/my_transform.h"

class Actor : public GroupModel {

private:

    static constexpr auto TAG = "Actor";

    Transform transform_;

    Actor(const Actor &) = delete;
    Actor(Actor &&) = delete;

public:

    Actor() = default;

    ~Actor() = default;

    Transform &transform() {
        return transform_;
    }

    const Transform &transform() const {
        return transform_;
    }
};
