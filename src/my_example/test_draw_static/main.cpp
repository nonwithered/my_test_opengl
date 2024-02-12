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
        return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    }

};

class TestPawn : public PawnActor {

private:

    static constexpr auto TAG = "TestCamera";

protected:

    void OnCreate() override {
        PawnActor::OnCreate();

        insert(Model<TestCamera>::Make());
    }

};

class TestBackgroundModule : public ScopeModule<LocalPlayerController<2>> {

protected:

    bool OnFrame(Context &context) override {
        if (&context != scope<0>()->window<0>()) {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return false;
        } 
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

class TestPawnModule : public ScopeModule<LocalPlayerController<2>, TestPawn> {

private:

    static constexpr auto rate = 0.5;
    static constexpr auto sensitivity = 0.05f;

    bool move_front_ = false;
    bool move_back_ = false;
    bool move_left_ = false;
    bool move_right_ = false;
    bool move_up_ = false;

    double pos_x_ = 0.0;
    double pos_y_ = 0.0;

    void BindKeyEvent(int key, bool TestPawnModule::* field) {
        ListenKeyEvent(key, true, [=](Context &context) -> bool {
            if (&context != scope<0>()->window<0>()) {
                return false;
            }
            this->*field = true;
        });
        ListenKeyEvent(key, false, [=](Context &context) -> bool {
            if (&context != scope<0>()->window<0>()) {
                return false;
            }
            this->*field = false;
        });
    }

    void Move(Context &context) {
        auto distance = rate * context.global().interval();
        auto front_back_ = 0;
        auto left_right_ = 0;
        if (move_front_) {
            ++front_back_;
        }
        if (move_back_) {
            --front_back_;
        }
        if (move_left_) {
            ++left_right_;
        }
        if (move_right_) {
            --left_right_;
        }
        scope<1>()->Find<MovementComponent>()->Move(front_back_ * distance);
        scope<1>()->Find<MovementComponent>()->Move(left_right_ * distance, 90.0f);
        auto up_down_ = 0;
        if (move_up_) {
            up_down_ = 1;
        }
        scope<1>()->Find<MovementComponent>()->MoveUp(up_down_ * distance);
    }

    void Rotate(Context &context) {
        if (&context != scope<0>()->window<0>()) {
            return;
        }
        auto [x, y] = context.GetCursorPos();
        auto delta_x = pos_x_ - x;
        auto delta_y = y - pos_y_;
        pos_x_ = x;
        pos_y_ = y;
        auto pitch = delta_y * sensitivity;
        auto yaw = delta_x * sensitivity;
        scope<1>()->Find<MovementComponent>()->RotatePitch(pitch);
        scope<1>()->Find<MovementComponent>()->RotateYaw(yaw);
    }

    void Draw(Context &context) {
        auto [width, height] = context.GetFramebufferSize();
        glViewport(0, 0, width, height);
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
    }

protected:

    bool OnFrame(Context &context) override {
        if (&context != scope<0>()->window<0>()) {
            return false;
        }
        Move(context);
        Rotate(context);
        Draw(context);
        return ScopeModule::OnFrame(context);
    }

public:

    TestPawnModule(std::weak_ptr<LocalPlayerController<2>> controller, std::weak_ptr<TestPawn> actor)
    : ScopeModule(controller, actor) {
        BindKeyEvent(GLFW_KEY_W, &TestPawnModule::move_front_);
        BindKeyEvent(GLFW_KEY_S, &TestPawnModule::move_back_);
        BindKeyEvent(GLFW_KEY_A, &TestPawnModule::move_left_);
        BindKeyEvent(GLFW_KEY_D, &TestPawnModule::move_right_);
        BindKeyEvent(GLFW_KEY_SPACE, &TestPawnModule::move_up_);
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
        actor->Find<MovementComponent>()->Move(-1.0, -45.0f);
        actor->Find<MovementComponent>()->RotatePitch(90.0f);
        actor->Find<MovementComponent>()->RotateYaw(90.0f);
        actor->Find<MovementComponent>()->Move(-1.0);
        level()->actor().insert(actor);
        module().NewModule<TestPawnModule>(controller(), actor);
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
            player->window<1>() = &RequireWindow(::TAG, 600, 800);
            player->window<0>() = &RequireWindow(::TAG, 800, 600);
            // player->window<0>() = &RequireWindow(::TAG);
            player->window<0>()->SetInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
