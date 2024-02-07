#pragma once

#include "my_utils/log.h"

class UniformOperator {

public:
    
    UniformOperator() = default;
    ~UniformOperator() = default;

private:

    UniformOperator(const UniformOperator &) = delete;
    UniformOperator(UniformOperator &&) = delete;

public:

    // GLfloat

    void operator()(GLint location, const glm::fvec1 &v) {
        glUniform1f(location, v[0]);
    }

    void operator()(GLint location, const glm::fvec2 &v) {
        glUniform2f(location, v[0], v[1]);
    }

    void operator()(GLint location, const glm::fvec3 &v) {
        glUniform3f(location, v[0], v[1], v[2]);
    }

    void operator()(GLint location, const glm::fvec4 &v) {
        glUniform4f(location, v[0], v[1], v[2], v[3]);
    }

    void operator()(GLint location, const std::vector<glm::fvec1> &v) {
        auto [count, value] = V(v);
        glUniform1fv(location, count, (const GLfloat *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::fvec2> &v) {
        auto [count, value] = V(v);
        glUniform2fv(location, count, (const GLfloat *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::fvec3> &v) {
        auto [count, value] = V(v);
        glUniform3fv(location, count, (const GLfloat *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::fvec4> &v) {
        auto [count, value] = V(v);
        glUniform4fv(location, count, (const GLfloat *) value.data());
    }

    // GLint

    void operator()(GLint location, const glm::ivec1 &v) {
        glUniform1i(location, v[0]);
    }

    void operator()(GLint location, const glm::ivec2 &v) {
        glUniform2i(location, v[0], v[1]);
    }

    void operator()(GLint location, const glm::ivec3 &v) {
        glUniform3i(location, v[0], v[1], v[2]);
    }

    void operator()(GLint location, const glm::ivec4 &v) {
        glUniform4i(location, v[0], v[1], v[2], v[3]);
    }

    void operator()(GLint location, const std::vector<glm::ivec1> &v) {
        auto [count, value] = V(v);
        glUniform1iv(location, count, (const GLint *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::ivec2> &v) {
        auto [count, value] = V(v);
        glUniform2iv(location, count, (const GLint *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::ivec3> &v) {
        auto [count, value] = V(v);
        glUniform3iv(location, count, (const GLint *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::ivec4> &v) {
        auto [count, value] = V(v);
        glUniform4iv(location, count, (const GLint *) value.data());
    }

    // GLdouble

    void operator()(GLint location, const glm::dvec1 &v) {
        glUniform1d(location, v[0]);
    }

    void operator()(GLint location, const glm::dvec2 &v) {
        glUniform2d(location, v[0], v[1]);
    }

    void operator()(GLint location, const glm::dvec3 &v) {
        glUniform3d(location, v[0], v[1], v[2]);
    }

    void operator()(GLint location, const glm::dvec4 &v) {
        glUniform4d(location, v[0], v[1], v[2], v[3]);
    }

    void operator()(GLint location, const std::vector<glm::dvec1> &v) {
        auto [count, value] = V(v);
        glUniform1dv(location, count, (const GLdouble *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::dvec2> &v) {
        auto [count, value] = V(v);
        glUniform2dv(location, count, (const GLdouble *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::dvec3> &v) {
        auto [count, value] = V(v);
        glUniform3dv(location, count, (const GLdouble *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::dvec4> &v) {
        auto [count, value] = V(v);
        glUniform4dv(location, count, (const GLdouble *) value.data());
    }

    // GLuint

    void operator()(GLint location, const glm::uvec1 &v) {
        glUniform1ui(location, v[0]);
    }

    void operator()(GLint location, const glm::uvec2 &v) {
        glUniform2ui(location, v[0], v[1]);
    }

    void operator()(GLint location, const glm::uvec3 &v) {
        glUniform3ui(location, v[0], v[1], v[2]);
    }

    void operator()(GLint location, const glm::uvec4 &v) {
        glUniform4ui(location, v[0], v[1], v[2], v[3]);
    }

    void operator()(GLint location, const std::vector<glm::uvec1> &v) {
        auto [count, value] = V(v);
        glUniform1uiv(location, count, (const GLuint *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::uvec2> &v) {
        auto [count, value] = V(v);
        glUniform2uiv(location, count, (const GLuint *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::uvec3> &v) {
        auto [count, value] = V(v);
        glUniform3uiv(location, count, (const GLuint *) value.data());
    }

    void operator()(GLint location, const std::vector<glm::uvec4> &v) {
        auto [count, value] = V(v);
        glUniform4uiv(location, count, (const GLuint *) value.data());
    }

    // fmat

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat2x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2fv(location, count, transpose, (const GLfloat *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat2x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2x3fv(location, count, transpose, (const GLfloat *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat2x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2x4fv(location, count, transpose, (const GLfloat *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat3x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3x2fv(location, count, transpose, (const GLfloat *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat3x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3fv(location, count, transpose, (const GLfloat *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat3x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3x4fv(location, count, transpose, (const GLfloat *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat4x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4x2fv(location, count, transpose, (const GLfloat *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat4x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4x3fv(location, count, transpose, (const GLfloat *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat4x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4fv(location, count, transpose, (const GLfloat *) value.data());
    }

    // dmat

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat2x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2dv(location, count, transpose, (const GLdouble *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat2x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2x3dv(location, count, transpose, (const GLdouble *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat2x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2x4dv(location, count, transpose, (const GLdouble *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat3x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3x2dv(location, count, transpose, (const GLdouble *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat3x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3dv(location, count, transpose, (const GLdouble *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat3x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3x4dv(location, count, transpose, (const GLdouble *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat4x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4x2dv(location, count, transpose, (const GLdouble *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat4x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4x3dv(location, count, transpose, (const GLdouble *) value.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat4x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4dv(location, count, transpose, (const GLdouble *) value.data());
    }

private:

    template<glm::length_t L, typename T, glm::qualifier Q>
    static std::pair<GLsizei, std::vector<const T *>> V(const std::vector<glm::vec<L, T, Q>> &v) {
        GLsizei count = (GLsizei) v.size();
        std::vector<const T *> value(count);
        for (auto i = 0; i != count; ++i) {
            value[i] = glm::value_ptr(v[i]);
        }
        return std::pair(count, std::move(value));
    }

    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
    static std::pair<GLsizei, std::vector<const T *>> V(const std::vector<glm::mat<C, R, T, Q>> &v) {
        GLsizei count = (GLsizei) v.size();
        std::vector<const T *> value(count);
        for (auto i = 0; i != count; ++i) {
            value[i] = glm::value_ptr(v[i]);
        }
        return std::pair(count, std::move(value));
    }

};

template<typename T, glm::length_t C, glm::length_t R>
class UniformValue;

template<>
class UniformValue<void, 0, 0> {

private:

    using self_type = typename UniformValue<void, 0, 0>;

    UniformValue(self_type &&) = delete;

protected:
    UniformValue() = default;
    UniformValue(const self_type &) = default;

public:
    virtual ~UniformValue() = default;
    virtual void Uniform(GLint location) const = 0;
};

template<typename T, glm::length_t L>
class UniformValue<T, L, 0> : public UniformValue<void, 0, 0> {

private:
    using value_type = typename glm::vec<L, T, glm::defaultp>;
    using self_type = typename UniformValue<T, L, 0>;

    UniformValue(self_type &&) = delete;

    const value_type value_;

public:

    UniformValue(const value_type &value) : value_(value) {
    }

    UniformValue(const self_type &) = default;

    ~UniformValue() final = default;

    void Uniform(GLint location) const final {
        LOGE("TEST", "%d %f %f %f", location, value_[0], value_[1], value_[2]);
        UniformOperator()(location, value_);
    }
};

template<typename T, glm::length_t L>
class UniformValue<T, L, 1> : public UniformValue<void, 0, 0> {

private:
    using value_type = typename std::vector<glm::vec<L, T, glm::defaultp>>;
    using self_type = typename UniformValue<T, L, 1>;

    UniformValue(self_type &&) = delete;

    const value_type value_;

public:

    UniformValue(const value_type &value) : value_(value) {
    }

    UniformValue(const self_type &) = default;

    ~UniformValue() final = default;

    void Uniform(GLint location) const final {
        UniformOperator()(location, value_);
    }
};

template<typename T, glm::length_t C, glm::length_t R>
class UniformValue : public UniformValue<void, 0, 0> {

private:
    using value_type = typename std::vector<glm::mat<C, R, T, glm::defaultp>>;
    using self_type = typename UniformValue<T, C, R>;

    UniformValue(self_type &&) = delete;

    const value_type value_;
    const bool transpose_;

public:

    UniformValue(const value_type &value, bool transpose) : value_(value), transpose_(transpose) {
    }

    UniformValue(const self_type &) = default;

    ~UniformValue() final = default;

    void Uniform(GLint location) const final {
        UniformOperator()(location, transpose_, value_);
    }
};

// GLfloat

using Uniform1f = typename UniformValue<GLfloat, 1, 0>;
using Uniform2f = typename UniformValue<GLfloat, 2, 0>;
using Uniform3f = typename UniformValue<GLfloat, 3, 0>;
using Uniform4f = typename UniformValue<GLfloat, 4, 0>;
using Uniform1fv = typename UniformValue<GLfloat, 1, 1>;
using Uniform2fv = typename UniformValue<GLfloat, 2, 1>;
using Uniform3fv = typename UniformValue<GLfloat, 3, 1>;
using Uniform4fv = typename UniformValue<GLfloat, 4, 1>;

// GLint

using Uniform1i = typename UniformValue<GLint, 1, 0>;
using Uniform2i = typename UniformValue<GLint, 2, 0>;
using Uniform3i = typename UniformValue<GLint, 3, 0>;
using Uniform4i = typename UniformValue<GLint, 4, 0>;
using Uniform1iv = typename UniformValue<GLint, 1, 1>;
using Uniform2iv = typename UniformValue<GLint, 2, 1>;
using Uniform3iv = typename UniformValue<GLint, 3, 1>;
using Uniform4iv = typename UniformValue<GLint, 4, 1>;

// GLdouble

using Uniform1d = typename UniformValue<GLdouble, 1, 0>;
using Uniform2d = typename UniformValue<GLdouble, 2, 0>;
using Uniform3d = typename UniformValue<GLdouble, 3, 0>;
using Uniform4d = typename UniformValue<GLdouble, 4, 0>;
using Uniform1dv = typename UniformValue<GLdouble, 1, 1>;
using Uniform2dv = typename UniformValue<GLdouble, 2, 1>;
using Uniform3dv = typename UniformValue<GLdouble, 3, 1>;
using Uniform4dv = typename UniformValue<GLdouble, 4, 1>;

// GLuint

using Uniform1ui = typename UniformValue<GLuint, 1, 0>;
using Uniform2ui = typename UniformValue<GLuint, 2, 0>;
using Uniform3ui = typename UniformValue<GLuint, 3, 0>;
using Uniform4ui = typename UniformValue<GLuint, 4, 0>;
using Uniform1uiv = typename UniformValue<GLuint, 1, 1>;
using Uniform2uiv = typename UniformValue<GLuint, 2, 1>;
using Uniform3uiv = typename UniformValue<GLuint, 3, 1>;
using Uniform4uiv = typename UniformValue<GLuint, 4, 1>;

// fmat

using UniformMatrix2fv = typename UniformValue<GLfloat, 2, 2>;
using UniformMatrix2x3fv = typename UniformValue<GLfloat, 2, 3>;
using UniformMatrix2x4fv = typename UniformValue<GLfloat, 2, 4>;
using UniformMatrix3x2fv = typename UniformValue<GLfloat, 3, 2>;
using UniformMatrix3fv = typename UniformValue<GLfloat, 3, 3>;
using UniformMatrix3x4fv = typename UniformValue<GLfloat, 3, 4>;
using UniformMatrix4x2fv = typename UniformValue<GLfloat, 4, 2>;
using UniformMatrix4x3fv = typename UniformValue<GLfloat, 4, 3>;
using UniformMatrix4fv = typename UniformValue<GLfloat, 4, 4>;

// dmat

using UniformMatrix2dv = typename UniformValue<GLdouble, 2, 2>;
using UniformMatrix2x3dv = typename UniformValue<GLdouble, 2, 3>;
using UniformMatrix2x4dv = typename UniformValue<GLdouble, 2, 4>;
using UniformMatrix3x2dv = typename UniformValue<GLdouble, 3, 2>;
using UniformMatrix3dv = typename UniformValue<GLdouble, 3, 3>;
using UniformMatrix3x4dv = typename UniformValue<GLdouble, 3, 4>;
using UniformMatrix4x2dv = typename UniformValue<GLdouble, 4, 2>;
using UniformMatrix4x3dv = typename UniformValue<GLdouble, 4, 3>;
using UniformMatrix4dv = typename UniformValue<GLdouble, 4, 4>;
