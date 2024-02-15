#pragma once

#define PROPERTY(type, name) \
public: \
    const type &name() const { \
        return name##_; \
    } \
    void name(const type &v) { \
        name##_ = v; \
    } \
private: \
    type name##_

