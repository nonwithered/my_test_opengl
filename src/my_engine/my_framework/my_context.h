#pragma once

#include "my_utils/log.h"

#include "my_model/my_model.h"

class LevelManager;
class WindowManager;
class ResourceManager;

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
    virtual WindowManager &window() = 0;

};

class Context {

private:
    Context(const Context &) = delete;
    Context(Context &&) = delete;

protected:
    Context() = default;

public:
    virtual ~Context() = default;

    virtual Global &global() = 0;

    virtual int width() = 0;

    virtual int height() = 0;

    virtual const std::string &title() = 0;

    virtual ResourceManager &resource() = 0;

    virtual bool GetKey(int key) = 0;

    virtual bool GetMouseButton(int button) = 0;

    virtual std::array<double, 2> GetCursorPos() = 0;

};