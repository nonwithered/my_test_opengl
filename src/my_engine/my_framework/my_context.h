#pragma once

#include "my_utils/my_log.h"

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

    virtual ResourceManager &resource() = 0;

    virtual bool primary() = 0;

    virtual const std::string &title() = 0;

    virtual void SetWindowTitle(const std::string &) = 0;

    virtual std::array<int, 2> GetFramebufferSize() = 0;

    virtual std::array<int, 2> GetWindowSize() = 0;

    virtual void SetWindowSize(int width, int height) = 0;

    virtual std::array<int, 2> GetWindowPos() = 0;

    virtual void SetWindowPos(int x, int y) = 0;

    virtual std::array<double, 2> GetCursorPos() = 0;

    virtual void SetCursorPos(double x, double y) = 0;

    virtual bool GetKey(int key) = 0;

    virtual bool GetMouseButton(int button) = 0;

    virtual int GetInputMode(int mode) = 0;

    virtual void SetInputMode(int mode, int value) = 0;

};

class FrameMonitor {

private:
    FrameMonitor(const FrameMonitor &) = delete;
    FrameMonitor(FrameMonitor &&) = delete;

protected:

    FrameMonitor() = default;

public:
    virtual ~FrameMonitor() = default;

    virtual void Frame(Global &context) {
    }
};
