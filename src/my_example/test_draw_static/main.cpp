#include <my_framework/my_runtime.h>
#include <my_model/my_camera_component.h>
#include <my_model/my_pawn_actor.h>

#include "RectSingleColor.h"
#include "RectMultiColor.h"
#include "RectPictureColor.h"

static constexpr auto TAG = "test_draw_rect";

class TestCamera : public CameraComponent {

protected:

    glm::mat4 transform_projection() override {
        return glm::mat4(1.0f);
    }

};

class TestPawn : public PawnActor {

private:

    static constexpr auto TAG = "TestCamera";

protected:

    void OnCreate() override {
        PawnActor::OnCreate();

        insert(Model<TestCamera>::Make());

        Find<MovementComponent>()->Move(-1.0, -45.0f);
        Find<MovementComponent>()->Rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }

};

class TestBackgroundModule : public ScopeModule<LocalPlayerController<2>> {

protected:

    bool OnFrame(Context &context) override {
        auto [width, height] = context.GetFramebufferSize();   
        if (&context != scope<0>()->window<0>()) {
            glViewport(0, 0, width, height);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return false;
        } 
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return ScopeModule::OnFrame(context);
    }

public:

    TestBackgroundModule(std::weak_ptr<LocalPlayerController<2>> controller)
    : ScopeModule(controller) {
        ListenKeyEvent(GLFW_KEY_ESCAPE, true, [this](Context &context) -> bool {
            if (&context != scope<0>()->window<0>()) {
                return false;
            }
            scope<0>()->Quit();
            return false;
        });
    }

};

class TestDrawModule : public ScopeModule<LocalPlayerController<2>, TestPawn> {

protected:

    bool OnFrame(Context &context) override {
        if (&context != scope<0>()->window<0>()) {
            return false;
        }
        auto level = scope<1>()->LookUp<LevelActor>(::TAG)->level();
        auto uniform = UniformParameter();
        scope<1>()->Find<CameraComponent>()->LookAt(uniform);
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

    TestDrawModule(std::weak_ptr<LocalPlayerController<2>> controller, std::weak_ptr<TestPawn> actor)
    : ScopeModule(controller, actor) {
    }
};

class TestController : public LocalPlayerController<2> {

public:
    TestController(std::weak_ptr<Level> level_) : LocalPlayerController(level_) {
    }

    void OnWindowClose(Context &context) override {
        Quit();
    }

protected:

    virtual void OnCreate() {
        LocalPlayerController<2>::OnCreate();

        module().NewModule<TestBackgroundModule>(controller());

        auto actor = Model<TestPawn>::Make();
        level()->actor().insert(actor);
        module().NewModule<TestDrawModule>(controller(), actor);
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

    void OnStart() override {
        Level::OnStart();
        NewPlayer<TestController>(self());
    }

    void OnResume() override {
        Level::OnResume();
        auto player = FindPlayer<TestController>();
        if (player) {
            player->window<0>() = &RequireWindow(::TAG, 800, 600);
            player->window<1>() = &RequireWindow(::TAG, 600, 800);
        }
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

    runtime.Loop();

    return EXIT_SUCCESS;
}
