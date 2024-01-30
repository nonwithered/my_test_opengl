#pragma once

#include <log.h>

class EBO {

private:

    static constexpr auto TAG = "EBO";

    static GLuint NewId() {
        GLuint id;
        glGenBuffers(1, &id);
        return id;
    }

    EBO(const EBO &) = delete;

    GLuint id_ = 0;

public:

    EBO() : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
    }

    EBO(EBO &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    ~EBO() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteBuffers(1, &id_);
        id_ = 0;
    }

    void Init(const std::vector<GLuint> &data) {
        LOGI(TAG, "init %u", id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * data.size(), data.data(), GL_STATIC_DRAW);
    }

    class Scope {

    private:

        static constexpr auto TAG = "EBO.Scope";
        Scope(const Scope &) = delete;
        Scope(Scope &&) = delete;

    public:
        Scope(EBO &owner) {
            LOGD(TAG, "bind %u", owner.id_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, owner.id_);
        }
        ~Scope() {
            LOGD(TAG, "unbind");
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    };

    Scope Use() {
        return Scope(*this);
    }
};

class VBO {

private:

    static constexpr auto TAG = "VBO";

    static GLuint NewId() {
        GLuint id;
        glGenBuffers(1, &id);
        return id;
    }

    VBO(const VBO &) = delete;

    GLuint id_ = 0;

public:

    VBO() : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
    }

    VBO(VBO &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    ~VBO() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteBuffers(1, &id_);
        id_ = 0;
    }

    void Init(const std::vector<GLfloat> &data, const std::vector<GLuint> &configs) {
        LOGI(TAG, "init %u", id_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);
        GLsizei size = 0;
        for (auto &config : configs) {
            size += config;
        }
        GLsizei offset = 0;
        for (int i = 0; i != configs.size(); ++i) {
            auto &config = configs[i];
            glVertexAttribPointer(i, config, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), (void *) (offset* sizeof(GLfloat)));
            glEnableVertexAttribArray(i);
            offset += config;
        }
    }

    class Scope {

    private:

        static constexpr auto TAG = "VBO.Scope";
        Scope(const Scope &) = delete;
        Scope(Scope &&) = delete;

    public:
        Scope(VBO &owner) {
            LOGD(TAG, "bind %u", owner.id_);
            glBindBuffer(GL_ARRAY_BUFFER, owner.id_);
        }
        ~Scope() {
            LOGD(TAG, "unbind");
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    };

    Scope Use() {
        return Scope(*this);
    }
};

class VAO {

private:

    static constexpr auto TAG = "VAO";

    static GLuint NewId() {
        GLuint id;
        glGenVertexArrays(1, &id);
        return id;
    }

    VAO(const VAO &) = delete;

    GLuint id_ = 0;

    VBO vbo_;
    EBO ebo_;

public:

    VAO(const std::vector<GLfloat> &data, const std::vector<GLuint> &configs, const std::vector<GLuint> &indices) : id_(NewId()) {
        LOGI(TAG, "ctor %u", id_);
        auto scope = std::make_unique<Scope>(*this);
        auto vbo = vbo_.Use();
        auto ebo= ebo_.Use();
        LOGI(TAG, "init %u", id_);
        ebo_.Init(indices);
        vbo_.Init(data, configs);
        scope.reset();
    }

    VAO(VAO &&that) : id_(that.id_) {
        that.id_ = 0;
    }

    ~VAO() {
        if (id_ == 0) {
            return;
        }
        LOGI(TAG, "dtor %u", id_);
        glDeleteVertexArrays(1, &id_);
        id_ = 0;
    }

    class Scope {

    private:

        static constexpr auto TAG = "VAO.Scope";
        Scope(const Scope &) = delete;
        Scope(Scope &&) = delete;

    public:
        Scope(const VAO &owner) {
            LOGD(TAG, "bind %u", owner.id_);
            glBindVertexArray(owner.id_);
        }
        ~Scope() {
            LOGD(TAG, "unbind");
            glBindVertexArray(0);
        }
    };

    Scope Use() const {
        return Scope(*this);
    }
};
