#pragma once

#include <log.h>

class Actor;

class Global {

private:
    Global(const Global &) = delete;
    Global(Global &&) = delete;

protected:
    Global() = default;

public:
    virtual ~Global() = default;

    virtual Actor &Model() = 0;

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

    virtual int width() = 0;

    virtual int height() = 0;

    virtual const std::string &title() = 0;

    virtual void Close() = 0;

    virtual void NewModule(std::unique_ptr<Module> create) = 0;

};