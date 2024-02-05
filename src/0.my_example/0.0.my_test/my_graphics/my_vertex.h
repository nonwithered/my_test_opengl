#pragma once

#include "my_utils/log.h"

#include "my_utils/my_compute.h"

class ElementArrayBuffer {

private:

    static constexpr auto TAG = "ElementArrayBuffer";

    static GLuint NewId() {
        GLuint id;
        glGenBuffers(1, &id);
        return id;
    }

    ElementArrayBuffer(const ElementArrayBuffer &) = delete;

    GLuint id_ = 0;

public:

    ElementArrayBuffer() : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
    }

    ElementArrayBuffer(ElementArrayBuffer &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    ~ElementArrayBuffer() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteBuffers(1, &id_);
        id_ = 0;
    }

    void Init(const void *data, GLenum type, GLsizei count) {
        LOGI(TAG, "init %u", id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeOf(type) * count, data, GL_STATIC_DRAW);
    }

    class Scope {

    private:

        static constexpr auto TAG = "ElementArrayBuffer.Scope";
        Scope(const Scope &) = delete;
        Scope(Scope &&) = delete;

        GLuint id_ = 0;

    public:
        Scope(GLuint id) : id_(id) {
            LOGD(TAG, "bind %u", id_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        }
        ~Scope() {
            if (!id_) {
                return;
            }
            id_ = 0;
            LOGD(TAG, "unbind");
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    };

    Scope Use() {
        return Scope(id_);
    }
};

class ArrayBuffer {

private:

    static constexpr auto TAG = "ArrayBuffer";

    static GLuint NewId() {
        GLuint id;
        glGenBuffers(1, &id);
        return id;
    }

    ArrayBuffer(const ArrayBuffer &) = delete;

    GLuint id_ = 0;

public:

    ArrayBuffer() : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
    }

    ArrayBuffer(ArrayBuffer &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    ~ArrayBuffer() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteBuffers(1, &id_);
        id_ = 0;
    }

    void Init(const void *data, GLsizei count, const std::vector<std::pair<GLenum, GLsizei>> &attrib) {
        LOGI(TAG, "init %u", id_);
        std::vector<GLsizei> attrib_size(attrib.size());
        for (auto i = 0; i != attrib.size(); ++i) {
            auto [attrib_type, attrib_count] = attrib[i];
            attrib_size[i] = (SizeOf(attrib_type) * attrib_count);
        }
        GLsizei type_size = 0;
        for (auto i : attrib_size) {
            type_size += i;
        }
        glBufferData(GL_ARRAY_BUFFER, type_size * count, data, GL_STATIC_DRAW);
        GLsizeiptr offset = 0;
        for (auto i = 0; i != attrib.size(); ++i) {
            auto [attrib_type, attrib_count] = attrib[i];
            glVertexAttribPointer(i, attrib_count, SizeOf(attrib_type), GL_FALSE, type_size, (const void *) offset);
            glEnableVertexAttribArray(i);
            offset += attrib_size[i];
        }
    }

    class Scope {

    private:

        static constexpr auto TAG = "ArrayBuffer.Scope";
        Scope(const Scope &) = delete;
        Scope(Scope &&) = delete;

        GLuint id_ = 0;

    public:
        Scope(GLuint id) : id_(id) {
            LOGD(TAG, "bind %u", id_);
            glBindBuffer(GL_ARRAY_BUFFER, id_);
        }
        ~Scope() {
            if (!id_) {
                return;
            }
            id_ = 0;
            LOGD(TAG, "unbind");
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    };

    Scope Use() {
        return Scope(id_);
    }
};

class VertexArray {

private:

    static constexpr auto TAG = "VertexArray";

    static GLuint NewId() {
        GLuint id;
        glGenVertexArrays(1, &id);
        return id;
    }

    VertexArray(const VertexArray &) = delete;

    GLuint id_ = 0;

    ArrayBuffer vbo_;
    ElementArrayBuffer ebo_;

    GLenum index_type_;

public:

    VertexArray(
        const void *data, GLsizei count, const std::vector<std::pair<GLenum, GLsizei>> &attrib,
        const void *index_data, GLenum index_type, GLsizei index_count)
    : id_(NewId()), index_type_(index_type) {
        LOGI(TAG, "ctor %u", id_);
        auto scope = Use();
        auto vbo = vbo_.Use();
        auto ebo= ebo_.Use();
        LOGI(TAG, "init %u", id_);
        ebo_.Init(index_data, index_type, index_count);
        vbo_.Init(data, count, attrib);
        scope.~Scope();
    }

    VertexArray(VertexArray &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    ~VertexArray() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteVertexArrays(1, &id_);
        id_ = 0;
    }

    GLenum index_type() const {
        return index_type_;
    }

    class Scope {

    private:

        static constexpr auto TAG = "VertexArray.Scope";
        Scope(const Scope &) = delete;
        Scope(Scope &&) = delete;

        GLuint id_ = 0;

    public:
        Scope(GLuint id) : id_(id) {
            LOGD(TAG, "bind %u", id_);
            glBindVertexArray(id_);
        }
        ~Scope() {
            if (!id_) {
                return;
            }
            id_ = 0;
            LOGD(TAG, "unbind");
            glBindVertexArray(0);
        }
    };

    Scope Use() {
        return Scope(id_);
    }
};

class Vertex {

private:

    static constexpr auto TAG = "Vertex";

    VertexArray vao_;
    std::vector<std::tuple<GLenum, GLsizei>> elements_;

public:
    Vertex(
        VertexArray vao,
        std::vector<std::tuple<GLenum, GLsizei>> elements)
    : vao_(std::move(vao))
    , elements_(std::move(elements)) {
    }

    Vertex(const Vertex &) = default;
    Vertex(Vertex &&) = default;
    ~Vertex() = default;

    void Draw() {
        auto &vao = vao_;
        auto scope = vao.Use();
        GLenum index_type = vao.index_type();
        GLsizei index_type_size = SizeOf(index_type);
        GLsizeiptr offset = 0;
        for (auto &element : elements_) {
            auto [mode, count] = element;
            glDrawElements(mode, count, index_type, (const void *) (offset * index_type_size));
            offset += count;
        }
    }

};
