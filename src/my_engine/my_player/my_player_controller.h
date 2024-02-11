#pragma once

#include "my_utils/my_log.h"

#include "my_framework/my_scope_module.h"

#include "my_model/my_level.h"

class PlayerController : ScopeModule<Level> {

private:

    static constexpr auto TAG = "PlayerController";

protected:
    PlayerController(std::weak_ptr<Level> data) : ScopeModule(std::move(data)) {
    }

};
