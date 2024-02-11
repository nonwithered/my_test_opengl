
#pragma once

#include "my_utils/my_log.h"

inline GLsizei SizeOf(GLenum type) {
    static constexpr auto TAG = "SizeOf";
    switch (type) {
        case GL_BYTE: return sizeof(GLbyte);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        case GL_SHORT: return sizeof(GLshort);
        case GL_UNSIGNED_SHORT: return sizeof(GLushort);
        case GL_INT: return sizeof(GLint);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_DOUBLE: return sizeof(GLdouble);
    }
    LOGE(TAG, "SizeOf invalid %" PRIx64, (uint64_t) type);
    throw std::exception();
}

template<typename T>
inline constexpr GLenum TypeOf() {
    static_assert(false, "TypeOf invalid");
}

template<>
inline constexpr GLenum TypeOf<GLbyte>() {
    return GL_BYTE;
}

template<>
inline constexpr GLenum TypeOf<GLubyte>() {
    return GL_UNSIGNED_BYTE;
}

template<>
inline constexpr GLenum TypeOf<GLshort>() {
    return GL_SHORT;
}

template<>
inline constexpr GLenum TypeOf<GLushort>() {
    return GL_UNSIGNED_SHORT;
}

template<>
inline constexpr GLenum TypeOf<GLint>() {
    return GL_INT;
}

template<>
inline constexpr GLenum TypeOf<GLuint>() {
    return GL_UNSIGNED_INT;
}

template<>
inline constexpr GLenum TypeOf<GLfloat>() {
    return GL_FLOAT;
}

template<>
inline constexpr GLenum TypeOf<GLdouble>() {
    return GL_DOUBLE;
}
