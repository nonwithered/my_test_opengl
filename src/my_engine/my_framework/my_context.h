#pragma once

#include "my_utils/log.h"

#include "my_model/my_model.h"

#include "my_manager/my_resource_manager.h"
#include "my_manager/my_level_manager.h"

class Global {

private:
    Global(const Global &) = delete;
    Global(Global &&) = delete;

protected:
    Global() = default;

public:
    virtual ~Global() = default;

    virtual float interval() = 0;

    virtual LevelManager &level() = 0;

};

class Module;

class Context {

private:
    Context(const Context &) = delete;
    Context(Context &&) = delete;

protected:
    Context() = default;

public:
    virtual ~Context() = default;

    virtual Global &global() = 0;

    virtual ResourceManager &resource() = 0;

    virtual int width() = 0;

    virtual int height() = 0;

    virtual const std::string &title() = 0;

    virtual void NewModule(std::unique_ptr<Module> create) = 0;

    virtual bool GetKey(int key) = 0;

    virtual bool GetMouseButton(int button) = 0;

    virtual std::array<double, 2> GetCursorPos() = 0;

};