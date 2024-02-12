#include <my_framework/my_runtime.h>
#include <my_model/my_camera_actor.h>

#include "RectSingleColor.h"
#include "RectMultiColor.h"
#include "RectPictureColor.h"

static constexpr auto TAG = "test_draw_rect";

class TestCamera : public CameraActor {

private:

    static constexpr auto TAG = "TestCamera";

protected:

    void OnCreate() override {

        auto transform_ = transform();
        transform_.translate() = glm::vec3(-0.5f, 0.5f, 0.0f);
        {
            glm::mat4 rotate(1.0f);
            rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            transform_.rotate() = rotate;
        }
        transform_.scale() = glm::vec3(0.5f);
        transform(transform_);

        name(TAG);
    }

    glm::mat4 transform_projection() override {
        return glm::mat4(1.0f);
    }

};

class TestBackgroundModule : public ScopeModule<PlayerController> {

protected:

    bool OnFrame(Context &context) override {
        auto [width, height] = context.GetFramebufferSize();    
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return ScopeModule::OnFrame(context);
    }

public:

    TestBackgroundModule(std::weak_ptr<PlayerController> p)
    : ScopeModule(std::move(p)) {
        ListenKeyEvent(GLFW_KEY_ESCAPE, true, [this](Context &context) -> bool {
            data()->Quit();
            return false;
        });
    }

};

class TestDrawModule : public ScopeModule<CameraActor> {

protected:

    bool OnFrame(Context &context) override {
        auto level = data()->LookUp<LevelActor>(::TAG)->level();
        auto uniform = UniformParameter();
        data()->LookAt(uniform);
        for (auto i = 0; i != level->actor().size(); ++i) {
            auto actor = level->actor().at(i);
            auto mesh_actor = dynamic_cast<MeshActor *>(actor.get());
            if (mesh_actor) {
                mesh_actor->Draw(context, uniform);
            }
        }
        return ScopeModule::OnFrame(context);
    }

public:

    TestDrawModule(std::weak_ptr<CameraActor> p)
    : ScopeModule(std::move(p)) {
    }
};

class TestController : public LocalPlayerController {

public:
    TestController(std::weak_ptr<Level> level_) : LocalPlayerController(level_) {
    }

    void OnWindowClose(Context &context) override {
        Quit();
    }

protected:

    virtual void OnCreate() {
        module().NewModule<TestBackgroundModule>(self());

        auto camera = Model<TestCamera>::Make();
        level()->actor().insert(camera);
        module().NewModule<TestDrawModule>(camera);
    }
};

class TestLevel : public Level {

protected:

    void OnCreate() override {
        Level::OnCreate();
        actor().insert(Model<RectSingleColor>::Make());
        actor().insert(Model<RectMultiColor>::Make());
        actor().insert(Model<RectPictureColor>::Make());
    }

    void OnStart(Global &context) override {
        NewPlayer<TestController>(self());
    }

public:
    TestLevel(const std::string &name) : Level(name) {
    }

};

class TestLauncherModule : public LauncherModule {

protected:

    void OnCreate(Global &context) override {
        LauncherModule::OnCreate(context);
        context.level().StartLevel<TestLevel>(std::string(::TAG));
    }
};

int main() {

    auto runtime = Runtime::Make<TestLauncherModule>();

    runtime.window().NewWindow(TAG);

    runtime.Loop();

    return EXIT_SUCCESS;
}
