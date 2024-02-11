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
        transform_.translate() = glm::vec3(-0.5f, 0.5f, 0);
        transform_.rotate() = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1.0f, 0));
        transform_.scale() = glm::vec3(0.5f);
        transform(transform_);
    }

    glm::mat4 transform_projection() override {
        return glm::mat4(1.0f);
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

public:
    TestLevel(const std::string &name) : Level(name) {
    }

};

class TestBackgroundModule : public Module {

protected:

    bool Frame(Context &context) override {
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, context.width(), context.height());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return Module::Frame(context);
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

class TestLevelModule : public ScopeModule<Level> {

public:

    TestLevelModule(std::weak_ptr<Level> p): ScopeModule(std::move(p)) {

        ListenKeyEvent(GLFW_KEY_ESCAPE, true, [this](Context &context) -> bool {
            data()->Finish();
            return false;
        });

        NewModule<TestBackgroundModule>();

        auto camera = Model<TestCamera>::Make();
        data()->actor().insert(camera);
        NewModule<TestDrawModule>(camera);
    }

};

class TestLauncherModule : public LauncherModule {

protected:

    void OnCreate(Global &context) override {
        LauncherModule::OnCreate(context);
        context.level().StartLevel<TestLevel>(std::string(::TAG));
    }

public:

    void OnLevelStart(std::weak_ptr<Level> level) override {
        NewModule<TestLevelModule>(level);
    }
};

int main() {

    auto runtime = Runtime::Make<TestLauncherModule>();

    runtime.window().NewWindow(TAG, 800, 600);

    runtime.Loop();

    return EXIT_SUCCESS;
}
