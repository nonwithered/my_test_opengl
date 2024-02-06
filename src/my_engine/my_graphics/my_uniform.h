#pragma once

#include "my_utils/log.h"

class UniformLocation {

private:
    const GLint location_;

public:
    UniformLocation(GLint location) : location_(location) {
    }

    // GLfloat

    void Uniform(std::array<GLfloat, 1> v) {
        glUniform1f(location_, v[0]);
    }

    void Uniform(std::array<GLfloat, 2> v) {
        glUniform2f(location_, v[0], v[1]);
    }

    void Uniform(std::array<GLfloat, 3> v) {
        glUniform3f(location_, v[0], v[1], v[2]);
    }

    void Uniform(std::array<GLfloat, 4> v) {
        glUniform4f(location_, v[0], v[1], v[2], v[3]);
    }

    void Uniform(const std::vector<std::array<GLfloat, 1>> &v) {
        auto [count, value] = V(v);
        glUniform1fv(location_, count, (const GLfloat *) value.data());
    }

    void Uniform(const std::vector<std::array<GLfloat, 2>> &v) {
        auto [count, value] = V(v);
        glUniform2fv(location_, count, (const GLfloat *) value.data());
    }

    void Uniform(const std::vector<std::array<GLfloat, 3>> &v) {
        auto [count, value] = V(v);
        glUniform3fv(location_, count, (const GLfloat *) value.data());
    }

    void Uniform(const std::vector<std::array<GLfloat, 4>> &v) {
        auto [count, value] = V(v);
        glUniform4fv(location_, count, (const GLfloat *) value.data());
    }

    // GLint

    void Uniform(std::array<GLint, 1> v) {
        glUniform1i(location_, v[0]);
    }

    void Uniform(std::array<GLint, 2> v) {
        glUniform2i(location_, v[0], v[1]);
    }

    void Uniform(std::array<GLint, 3> v) {
        glUniform3i(location_, v[0], v[1], v[2]);
    }

    void Uniform(std::array<GLint, 4> v) {
        glUniform4i(location_, v[0], v[1], v[2], v[3]);
    }

    void Uniform(const std::vector<std::array<GLint, 1>> &v) {
        auto [count, value] = V(v);
        glUniform1iv(location_, count, (const GLint *) value.data());
    }

    void Uniform(const std::vector<std::array<GLint, 2>> &v) {
        auto [count, value] = V(v);
        glUniform2iv(location_, count, (const GLint *) value.data());
    }

    void Uniform(const std::vector<std::array<GLint, 3>> &v) {
        auto [count, value] = V(v);
        glUniform3iv(location_, count, (const GLint *) value.data());
    }

    void Uniform(const std::vector<std::array<GLint, 4>> &v) {
        auto [count, value] = V(v);
        glUniform4iv(location_, count, (const GLint *) value.data());
    }

    // GLdouble

    void Uniform(std::array<GLdouble, 1> v) {
        glUniform1d(location_, v[0]);
    }

    void Uniform(std::array<GLdouble, 2> v) {
        glUniform2d(location_, v[0], v[1]);
    }

    void Uniform(std::array<GLdouble, 3> v) {
        glUniform3d(location_, v[0], v[1], v[2]);
    }

    void Uniform(std::array<GLdouble, 4> v) {
        glUniform4d(location_, v[0], v[1], v[2], v[3]);
    }

    void Uniform(const std::vector<std::array<GLdouble, 1>> &v) {
        auto [count, value] = V(v);
        glUniform1dv(location_, count, (const GLdouble *) value.data());
    }

    void Uniform(const std::vector<std::array<GLdouble, 2>> &v) {
        auto [count, value] = V(v);
        glUniform2dv(location_, count, (const GLdouble *) value.data());
    }

    void Uniform(const std::vector<std::array<GLdouble, 3>> &v) {
        auto [count, value] = V(v);
        glUniform3dv(location_, count, (const GLdouble *) value.data());
    }

    void Uniform(const std::vector<std::array<GLdouble, 4>> &v) {
        auto [count, value] = V(v);
        glUniform4dv(location_, count, (const GLdouble *) value.data());
    }

    // GLuint

    void Uniform(std::array<GLuint, 1> v) {
        glUniform1ui(location_, v[0]);
    }

    void Uniform(std::array<GLuint, 2> v) {
        glUniform2ui(location_, v[0], v[1]);
    }

    void Uniform(std::array<GLuint, 3> v) {
        glUniform3ui(location_, v[0], v[1], v[2]);
    }

    void Uniform(std::array<GLuint, 4> v) {
        glUniform4ui(location_, v[0], v[1], v[2], v[3]);
    }

    void Uniform(const std::vector<std::array<GLuint, 1>> &v) {
        auto [count, value] = V(v);
        glUniform1uiv(location_, count, (const GLuint *) value.data());
    }

    void Uniform(const std::vector<std::array<GLuint, 2>> &v) {
        auto [count, value] = V(v);
        glUniform2uiv(location_, count, (const GLuint *) value.data());
    }

    void Uniform(const std::vector<std::array<GLuint, 3>> &v) {
        auto [count, value] = V(v);
        glUniform3uiv(location_, count, (const GLuint *) value.data());
    }

    void Uniform(const std::vector<std::array<GLuint, 4>> &v) {
        auto [count, value] = V(v);
        glUniform4uiv(location_, count, (const GLuint *) value.data());
    }

    // fmat

    void Uniform(GLboolean transpose, const std::vector<glm::fmat2x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2fv(location_, count, transpose, (const GLfloat *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::fmat2x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2x3fv(location_, count, transpose, (const GLfloat *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::fmat2x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2x4fv(location_, count, transpose, (const GLfloat *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::fmat3x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3x2fv(location_, count, transpose, (const GLfloat *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::fmat3x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3fv(location_, count, transpose, (const GLfloat *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::fmat3x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3x4fv(location_, count, transpose, (const GLfloat *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::fmat4x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4x2fv(location_, count, transpose, (const GLfloat *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::fmat4x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4x3fv(location_, count, transpose, (const GLfloat *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::fmat4x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4fv(location_, count, transpose, (const GLfloat *) value.data());
    }

    // dmat

    void Uniform(GLboolean transpose, const std::vector<glm::dmat2x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2dv(location_, count, transpose, (const GLdouble *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::dmat2x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2x3dv(location_, count, transpose, (const GLdouble *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::dmat2x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix2x4dv(location_, count, transpose, (const GLdouble *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::dmat3x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3x2dv(location_, count, transpose, (const GLdouble *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::dmat3x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3dv(location_, count, transpose, (const GLdouble *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::dmat3x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix3x4dv(location_, count, transpose, (const GLdouble *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::dmat4x2> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4x2dv(location_, count, transpose, (const GLdouble *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::dmat4x3> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4x3dv(location_, count, transpose, (const GLdouble *) value.data());
    }

    void Uniform(GLboolean transpose, const std::vector<glm::dmat4x4> &v) {
        auto [count, value] = V(v);
        glUniformMatrix4dv(location_, count, transpose, (const GLdouble *) value.data());
    }

private:

    template<typename T, size_t n>
    std::pair<GLsizei, std::vector<const T *>> V(const std::vector<std::array<T, n>> &v) {
        GLsizei count = (GLsizei) v.size();
        std::vector<const T *> value(count);
        for (auto i = 0; i != count; ++i) {
            value[i] = v[i].data();
        }
        return std::pair(count, std::move(value));
    }

    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
    std::pair<GLsizei, std::vector<const T *>> V(const std::vector<glm::mat<C, R, T, Q>> &v) {
        GLsizei count = (GLsizei) v.size();
        std::vector<const T *> value(count);
        for (auto i = 0; i != count; ++i) {
            value[i] = glm::value_ptr(v[i]);
        }
        return std::pair(count, std::move(value));
    }

};
