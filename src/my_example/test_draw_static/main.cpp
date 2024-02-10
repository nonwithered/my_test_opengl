#include <my_framework/my_runtime.h>
#include <my_model/my_camera.h>

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
        transform_.rotate() = glm::vec3(0.0f, 0.0f, -1.0f);
        transform_.scale() = glm::vec3(0.5f);
        transform(transform_);

        direction_up(glm::vec3(0.0f, 1.0f, 0.0f));
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
    TestLevel() : Level(::TAG) {
    }

};

class TestBackgroundModule : public Module {

protected:

    bool Frame() override {
        glViewport(0, 0, context().width(), context().height());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return Module::Frame();
    }

};

class TestDrawModule : public ScopeModule<CameraActor> {

private:

    std::weak_ptr<Level> level_;

    std::shared_ptr<Level> level() {
        return level_.lock();
    }

protected:

    bool OnFrame() override {
        auto uniform = UniformParameter();

        data()->LookAt(uniform);

        for (auto i = 0; i != level()->actor().size(); ++i) {
            auto actor = level()->actor().at(i);
            auto static_mesh_actor = dynamic_cast<StaticMeshActor *>(actor.get());
            if (static_mesh_actor) {
                static_mesh_actor->Draw(context(), uniform);
            }
        }
        return ScopeModule::OnFrame();
    }

public:

    TestDrawModule(std::weak_ptr<CameraActor> camera, std::weak_ptr<Level> level)
    : ScopeModule(std::move(camera))
    , level_(level) {
    }
};

class TestLevelModule : public ScopeModule<Level> {

protected:

    void OnCreate() override {
        ScopeModule::OnCreate();

        ListenKeyEvent(GLFW_KEY_ESCAPE, true, [this]() -> bool {
            data()->Finish();
            return false;
        });

        NewModule<TestBackgroundModule>();

        auto camera = Model<TestCamera>::Make();
        data()->actor().insert(camera);
        NewModule<TestDrawModule>(camera, data());
    }

public:

    TestLevelModule(std::weak_ptr<Level> level): ScopeModule(std::move(level)) {
    }

};

class TestLauncherModule : public LauncherModule {

protected:

    void OnCreate() override {
        LauncherModule::OnCreate();
        glEnable(GL_DEPTH_TEST);
        context().global().level().StartLevel<TestLevel>();
    }

public:

    void OnLevelStart(std::weak_ptr<Level> level) override {
        NewModule<TestLevelModule>(level);
    }
};

int main() {

    Runtime runtime;

    runtime.NewWindow<TestLauncherModule>(TAG, 800, 600);

    runtime.Loop();

    return EXIT_SUCCESS;
}
