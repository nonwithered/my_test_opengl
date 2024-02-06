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

    void operator()(GLint location, const std::array<GLfloat, 1> &v) {
        glUniform1f(location, v[0]);
    }

    void operator()(GLint location, const std::array<GLfloat, 2> &v) {
        glUniform2f(location, v[0], v[1]);
    }

    void operator()(GLint location, const std::array<GLfloat, 3> &v) {
        glUniform3f(location, v[0], v[1], v[2]);
    }

    void operator()(GLint location, const std::array<GLfloat, 4> &v) {
        glUniform4f(location, v[0], v[1], v[2], v[3]);
    }

    void operator()(GLint location, const std::vector<std::array<GLfloat, 1>> &v) {
        auto [count, value] = V(v);
        glUniform1fv(location, count, (const GLfloat *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLfloat, 2>> &v) {
        auto [count, value] = V(v);
        glUniform2fv(location, count, (const GLfloat *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLfloat, 3>> &v) {
        auto [count, value] = V(v);
        glUniform3fv(location, count, (const GLfloat *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLfloat, 4>> &v) {
        auto [count, value] = V(v);
        glUniform4fv(location, count, (const GLfloat *) value.data());
    }

    // GLint

    void operator()(GLint location, const std::array<GLint, 1> &v) {
        glUniform1i(location, v[0]);
    }

    void operator()(GLint location, const std::array<GLint, 2> &v) {
        glUniform2i(location, v[0], v[1]);
    }

    void operator()(GLint location, const std::array<GLint, 3> &v) {
        glUniform3i(location, v[0], v[1], v[2]);
    }

    void operator()(GLint location, const std::array<GLint, 4> &v) {
        glUniform4i(location, v[0], v[1], v[2], v[3]);
    }

    void operator()(GLint location, const std::vector<std::array<GLint, 1>> &v) {
        auto [count, value] = V(v);
        glUniform1iv(location, count, (const GLint *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLint, 2>> &v) {
        auto [count, value] = V(v);
        glUniform2iv(location, count, (const GLint *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLint, 3>> &v) {
        auto [count, value] = V(v);
        glUniform3iv(location, count, (const GLint *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLint, 4>> &v) {
        auto [count, value] = V(v);
        glUniform4iv(location, count, (const GLint *) value.data());
    }

    // GLdouble

    void operator()(GLint location, const std::array<GLdouble, 1> &v) {
        glUniform1d(location, v[0]);
    }

    void operator()(GLint location, const std::array<GLdouble, 2> &v) {
        glUniform2d(location, v[0], v[1]);
    }

    void operator()(GLint location, const std::array<GLdouble, 3> &v) {
        glUniform3d(location, v[0], v[1], v[2]);
    }

    void operator()(GLint location, const std::array<GLdouble, 4> &v) {
        glUniform4d(location, v[0], v[1], v[2], v[3]);
    }

    void operator()(GLint location, const std::vector<std::array<GLdouble, 1>> &v) {
        auto [count, value] = V(v);
        glUniform1dv(location, count, (const GLdouble *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLdouble, 2>> &v) {
        auto [count, value] = V(v);
        glUniform2dv(location, count, (const GLdouble *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLdouble, 3>> &v) {
        auto [count, value] = V(v);
        glUniform3dv(location, count, (const GLdouble *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLdouble, 4>> &v) {
        auto [count, value] = V(v);
        glUniform4dv(location, count, (const GLdouble *) value.data());
    }

    // GLuint

    void operator()(GLint location, const std::array<GLuint, 1> &v) {
        glUniform1ui(location, v[0]);
    }

    void operator()(GLint location, const std::array<GLuint, 2> &v) {
        glUniform2ui(location, v[0], v[1]);
    }

    void operator()(GLint location, const std::array<GLuint, 3> &v) {
        glUniform3ui(location, v[0], v[1], v[2]);
    }

    void operator()(GLint location, const std::array<GLuint, 4> &v) {
        glUniform4ui(location, v[0], v[1], v[2], v[3]);
    }

    void operator()(GLint location, const std::vector<std::array<GLuint, 1>> &v) {
        auto [count, value] = V(v);
        glUniform1uiv(location, count, (const GLuint *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLuint, 2>> &v) {
        auto [count, value] = V(v);
        glUniform2uiv(location, count, (const GLuint *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLuint, 3>> &v) {
        auto [count, value] = V(v);
        glUniform3uiv(location, count, (const GLuint *) value.data());
    }

    void operator()(GLint location, const std::vector<std::array<GLuint, 4>> &v) {
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

    template<typename T, size_t n>
    static std::pair<GLsizei, std::vector<const T *>> V(const std::vector<std::array<T, n>> &v) {
        GLsizei count = (GLsizei) v.size();
        std::vector<const T *> value(count);
        for (auto i = 0; i != count; ++i) {
            value[i] = v[i].data();
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

template<typename T, glm::length_t N>
class UniformValue<T, N, 0> : public UniformValue<void, 0, 0> {

private:
    using value_type = typename std::array<T, N>;
    using self_type = typename UniformValue<T, N, 0>;

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

template<typename T, glm::length_t N>
class UniformValue<T, N, 1> : public UniformValue<void, 0, 0> {

private:
    using value_type = typename std::vector<std::array<T, N>>;
    using self_type = typename UniformValue<T, N, 1>;

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
