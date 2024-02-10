#pragma once

#include "my_utils/log.h"

class UniformOperator {

private:

    static constexpr auto TAG = "UniformOperator";

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
        glUniform1fv(location, (GLsizei) v.size(), (const GLfloat *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::fvec2> &v) {
        glUniform2fv(location, (GLsizei) v.size(), (const GLfloat *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::fvec3> &v) {
        glUniform3fv(location, (GLsizei) v.size(), (const GLfloat *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::fvec4> &v) {
        glUniform4fv(location, (GLsizei) v.size(), (const GLfloat *) v.data());
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
        glUniform1iv(location, (GLsizei) v.size(), (const GLint *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::ivec2> &v) {
        glUniform2iv(location, (GLsizei) v.size(), (const GLint *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::ivec3> &v) {
        glUniform3iv(location, (GLsizei) v.size(), (const GLint *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::ivec4> &v) {
        glUniform4iv(location, (GLsizei) v.size(), (const GLint *) v.data());
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
        glUniform1dv(location, (GLsizei) v.size(), (const GLdouble *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::dvec2> &v) {
        glUniform2dv(location, (GLsizei) v.size(), (const GLdouble *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::dvec3> &v) {
        glUniform3dv(location, (GLsizei) v.size(), (const GLdouble *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::dvec4> &v) {
        glUniform4dv(location, (GLsizei) v.size(), (const GLdouble *) v.data());
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
        glUniform1uiv(location, (GLsizei) v.size(), (const GLuint *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::uvec2> &v) {
        glUniform2uiv(location, (GLsizei) v.size(), (const GLuint *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::uvec3> &v) {
        glUniform3uiv(location, (GLsizei) v.size(), (const GLuint *) v.data());
    }

    void operator()(GLint location, const std::vector<glm::uvec4> &v) {
        glUniform4uiv(location, (GLsizei) v.size(), (const GLuint *) v.data());
    }

    // fmat

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat2x2> &v) {
        glUniformMatrix2fv(location, (GLsizei) v.size(), transpose, (const GLfloat *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat2x3> &v) {
        glUniformMatrix2x3fv(location, (GLsizei) v.size(), transpose, (const GLfloat *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat2x4> &v) {
        glUniformMatrix2x4fv(location, (GLsizei) v.size(), transpose, (const GLfloat *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat3x2> &v) {
        glUniformMatrix3x2fv(location, (GLsizei) v.size(), transpose, (const GLfloat *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat3x3> &v) {
        glUniformMatrix3fv(location, (GLsizei) v.size(), transpose, (const GLfloat *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat3x4> &v) {
        glUniformMatrix3x4fv(location, (GLsizei) v.size(), transpose, (const GLfloat *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat4x2> &v) {
        glUniformMatrix4x2fv(location, (GLsizei) v.size(), transpose, (const GLfloat *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat4x3> &v) {
        glUniformMatrix4x3fv(location, (GLsizei) v.size(), transpose, (const GLfloat *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::fmat4x4> &v) {
        glUniformMatrix4fv(location, (GLsizei) v.size(), transpose, (const GLfloat *) v.data());
    }

    // dmat

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat2x2> &v) {
        glUniformMatrix2dv(location, (GLsizei) v.size(), transpose, (const GLdouble *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat2x3> &v) {
        glUniformMatrix2x3dv(location, (GLsizei) v.size(), transpose, (const GLdouble *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat2x4> &v) {
        glUniformMatrix2x4dv(location, (GLsizei) v.size(), transpose, (const GLdouble *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat3x2> &v) {
        glUniformMatrix3x2dv(location, (GLsizei) v.size(), transpose, (const GLdouble *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat3x3> &v) {
        glUniformMatrix3dv(location, (GLsizei) v.size(), transpose, (const GLdouble *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat3x4> &v) {
        glUniformMatrix3x4dv(location, (GLsizei) v.size(), transpose, (const GLdouble *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat4x2> &v) {
        glUniformMatrix4x2dv(location, (GLsizei) v.size(), transpose, (const GLdouble *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat4x3> &v) {
        glUniformMatrix4x3dv(location, (GLsizei) v.size(), transpose, (const GLdouble *) v.data());
    }

    void operator()(GLint location, GLboolean transpose, const std::vector<glm::dmat4x4> &v) {
        glUniformMatrix4dv(location, (GLsizei) v.size(), transpose, (const GLdouble *) v.data());
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
    virtual self_type &Clone() const = 0;
};

using Uniform_t = typename UniformValue<void, 0, 0>;

using UniformParameter = typename std::unordered_map<std::string, std::shared_ptr<Uniform_t>>;

template<typename T, glm::length_t L>
class UniformValue<T, L, 0> : public Uniform_t {

private:
    using value_type = typename T;
    using vector_type = typename glm::vec<L, T, glm::defaultp>;
    using self_type = typename UniformValue<T, L, 0>;

    UniformValue(self_type &&) = delete;

    const vector_type value_;

public:

    UniformValue(const vector_type &value) : value_(value) {
    }

    UniformValue(value_type v0) : value_(v0) {
    }

    UniformValue(value_type v0, value_type v1) : value_(v0, v1) {
    }

    UniformValue(value_type v0, value_type v1, value_type v2) : value_(v0, v1, v2) {
    }

    UniformValue(value_type v0, value_type v1, value_type v2, value_type v3) : value_(v0, v1, v2, v3) {
    }

    UniformValue(const self_type &) = default;

    ~UniformValue() final = default;

    void Uniform(GLint location) const final {
        UniformOperator()(location, value_);
    }

    Uniform_t &Clone() const final {
        return *new self_type(value_);
    }

public:

    static std::unique_ptr<self_type> Make(const vector_type &value) {
        return std::make_unique<self_type>(value);
    }

    static std::unique_ptr<self_type> Make(value_type v0) {
        return std::make_unique<self_type>(v0);
    }

    static std::unique_ptr<self_type> Make(value_type v0, value_type v1) {
        return std::make_unique<self_type>(v0, v1);
    }

    static std::unique_ptr<self_type> Make(value_type v0, value_type v1, value_type v2) {
        return std::make_unique<self_type>(v0, v1, v2);
    }

    static std::unique_ptr<self_type> Make(value_type v0, value_type v1, value_type v2, value_type v3) {
        return std::make_unique<self_type>(v0, v1, v2, v3);
    }
};

template<typename T, glm::length_t L>
class UniformValue<T, L, 1> : public Uniform_t {

private:
    using value_type = typename glm::vec<L, T, glm::defaultp>;
    using vector_type = typename std::vector<value_type>;
    using self_type = typename UniformValue<T, L, 1>;

    UniformValue(self_type &&) = delete;

    const vector_type value_;

public:

    UniformValue(const vector_type &value) : value_(value) {
    }

    UniformValue(const std::initializer_list<value_type> &value) : value_(value) {
    }

    UniformValue(const self_type &) = default;

    ~UniformValue() final = default;

    void Uniform(GLint location) const final {
        UniformOperator()(location, value_);
    }

    Uniform_t &Clone() const final {
        return *new self_type(value_);
    }

public:

    static std::unique_ptr<self_type> Make(const vector_type &value) {
        return std::make_unique<self_type>(value);
    }

    static std::unique_ptr<self_type> Make(const std::initializer_list<value_type> &value) {
        return std::make_unique<self_type>(value);
    }
};

template<typename T, glm::length_t C, glm::length_t R>
class UniformValue : public Uniform_t {

private:
    using value_type = typename glm::mat<C, R, T, glm::defaultp>;
    using vector_type = typename std::vector<value_type>;
    using self_type = typename UniformValue<T, C, R>;

    UniformValue(self_type &&) = delete;

    const bool transpose_;
    const vector_type value_;

public:

    UniformValue(bool transpose, const vector_type &value) : transpose_(transpose), value_(value) {
    }

    UniformValue(bool transpose, const std::initializer_list<value_type> &value) : transpose_(transpose), value_(value) {
    }

    UniformValue(const self_type &) = default;

    ~UniformValue() final = default;

    void Uniform(GLint location) const final {
        UniformOperator()(location, transpose_ ? GL_TRUE : GL_FALSE, value_);
    }

    Uniform_t &Clone() const final {
        return *new self_type(transpose_, value_);
    }

public:

    static std::unique_ptr<self_type> Make(bool transpose, const vector_type &value) {
        return std::make_unique<self_type>(transpose, value);
    }

    static std::unique_ptr<self_type> Make(bool transpose, const std::initializer_list<value_type> &value) {
        return std::make_unique<self_type>(transpose, value);
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
