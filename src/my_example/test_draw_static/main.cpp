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
        transform_.rotate() = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transform_.scale() = glm::vec3(0.5f);
        transform(transform_);

        name(TAG);
    }

    glm::mat4 transform_projection() override {
        return glm::mat4(1.0f);
    }

};

class TestBackgroundModule : public ScopeModule<Level> {

protected:

    bool OnFrame(Context &context) override {
        glViewport(0, 0, context.width(), context.height());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return ScopeModule::OnFrame(context);
    }

public:
    TestBackgroundModule(std::weak_ptr<Level> p): ScopeModule(std::move(p)) {
        ListenKeyEvent(GLFW_KEY_ESCAPE, true, [this](Context &context) -> bool {
            data()->Finish();
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
        module().NewModule<TestBackgroundModule>(level());

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

    void OnStart(Global &context, Level::vector_player_t &controller) override {
        controller.push_back(std::make_unique<TestController>(self()));
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

    runtime.window().NewWindow(TAG, 800, 600);

    runtime.Loop();

    return EXIT_SUCCESS;
}
