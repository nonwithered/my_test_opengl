#include <my_framework/my_runtime.h>
#include <my_model/my_camera_component.h>
#include <my_model/my_pawn_actor.h>

#include "RectSingleColor.h"
#include "RectMultiColor.h"
#include "RectPictureColor.h"

static constexpr auto TAG = "test_draw_rect";

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

class TestPawnModule : public ScopeModule<LocalPlayerController<2>, PawnActor> {

private:

    static constexpr auto rate = 5;
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
            return false;
        });
        ListenKeyEvent(key, false, [=](Context &context) -> bool {
            if (&context != scope<0>()->window<0>()) {
                return false;
            }
            this->*field = false;
            return false;
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
        float delta_x = pos_x_ - x;
        float delta_y = y - pos_y_;
        pos_x_ = x;
        pos_y_ = y;
        auto pitch = delta_y * sensitivity;
        auto yaw = delta_x * sensitivity;
        scope<1>()->Find<MovementComponent>()->RotatePitch(pitch);
        scope<1>()->Find<MovementComponent>()->RotateYaw(yaw);
    }

    void Draw(Context &context) {
        auto [w, h] = context.GetFramebufferSize();
        std::array<int, 4> port = { 0, 0, w, h, };
        auto level = scope<1>()->LookUp<LevelActor>(::TAG);
        scope<1>()->Find<CameraComponent>()->Draw(context, *level, port);
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

    TestPawnModule(std::weak_ptr<LocalPlayerController<2>> controller, std::weak_ptr<PawnActor> actor)
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

        auto pawn = level()->actor().NewActor<PawnActor>();
        pawn->Find<MovementComponent>()->MoveUp(1.0f);
        pawn->Find<MovementComponent>()->RotatePitch(90);
        // {
        //     auto camera = pawn->NewActor<OrthoCameraComponent>();
        //     camera->vision(0.1, 30);
        //     camera->sight(-20, 20, -20, 20);
        // }
        {
            auto camera = pawn->NewActor<PerspectiveCameraComponent>();
            camera->vision(0.1, 100);
            camera->sight(45);
        }

        module().NewModule<TestPawnModule>(controller(), pawn);
    }
};

class TestLevel : public Level {

protected:

    void OnCreate() override {
        Level::OnCreate();
        {
            auto mesh = actor().NewActor<RectSingleColor>("RectSingleColor");
            auto transform= mesh->transform();
            transform.translate() = glm::vec3(-5, 5, 0);
            transform.scale() = glm::vec3(5);
            mesh->transform(transform);
        }
        {
            auto mesh = actor().NewActor<RectMultiColor>("RectMultiColor");
            auto transform= mesh->transform();
            transform.translate() = glm::vec3(5, 5, 0);
            transform.scale() = glm::vec3(5);
            mesh->transform(transform);
        }
        {
            auto mesh = actor().NewActor<RectPictureColor>("RectPictureColor");
            auto transform= mesh->transform();
            transform.translate() = glm::vec3(5, -5, 0);
            transform.scale() = glm::vec3(5);
            mesh->transform(transform);
        }
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
