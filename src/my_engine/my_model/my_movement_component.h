#pragma once

#include "my_model/my_actor.h"

class MovementComponent : public Actor {

public:

    MovementComponent() = default;
    ~MovementComponent() = default;

    void MoveUp(float distance) {
        auto owner = parent();
        if (!owner) {
            return;
        }
        auto transform = owner->transform();
        auto rotate_matrix = transform.rotate_transform();
        glm::vec3 direction = rotate_matrix * Transform::direction_up_default();
        transform.translate() = transform.translate() + direction * distance;
        owner->transform(transform);
    }

    void Move(float distance, float angle = 0.0f) {
        auto owner = parent();
        if (!owner) {
            return;
        }
        auto transform = owner->transform();
        auto rotate_matrix = transform.rotate_transform();
        glm::vec3 direction = rotate_matrix * Transform::direction_default();
        glm::vec3 direction_up = rotate_matrix * Transform::direction_up_default();
        direction = glm::rotate(glm::mat4(1.0f), glm::radians(angle), direction_up) * glm::vec4(direction, 1.0f);
        transform.translate() = transform.translate() + direction * distance;
        owner->transform(transform);
    }
};