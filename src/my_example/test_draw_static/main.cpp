#include <my_framework/my_runtime.h>
#include <my_model/my_camera_component.h>
#include <my_model/my_pawn_actor.h>

#include "RectSingleColor.h"
#include "RectMultiColor.h"
#include "RectPictureColor.h"
#include "BoxPictureColor.h"

static constexpr auto TAG = "test_draw_rect";

class TestBackgroundModule : public ScopeModule<LocalPlayerController<2, 4>> {

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

    TestBackgroundModule(std::weak_ptr<LocalPlayerController<2, 4>> controller)
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

class TestPawnModule : public ScopeModule<LocalPlayerController<2, 4>, PawnActor> {

private:

    static constexpr auto rate = 5;
    static constexpr auto sensitivity = 0.05f;

    bool move_front_ = false;
    bool move_back_ = false;
    bool move_left_ = false;
    bool move_right_ = false;
    bool move_up_ = false;

    float pos_x_ = 0.0f;
    float pos_y_ = 0.0f;

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
        auto up_down_ = scope<1>()->transform().translate().z > 1 ? -1 : 0;
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
        float delta_x = pos_x_ - (float) x;
        float delta_y = (float) y - pos_y_;
        pos_x_ = (float) x;
        pos_y_ = (float) y;
        auto pitch = delta_y * sensitivity;
        auto yaw = delta_x * sensitivity;
        scope<0>()->camera<0>()->Find<MovementComponent>()->RotatePitch(pitch);
        scope<1>()->Find<MovementComponent>()->RotateYaw(yaw);
    }

    void Draw(Context &context) {
        auto level = scope<1>()->LookUp<LevelActor>(::TAG);
        scope<0>()->camera<0>()->Draw(context, *level);
        scope<0>()->camera<1>()->Draw(context, *level);
        scope<0>()->camera<2>()->Draw(context, *level);
        scope<0>()->camera<3>()->Draw(context, *level);
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

    void OnFramebufferSize(Context &context, int width, int height) override {
        ScopeModule::OnFramebufferSize(context, width, height);
        if (&context != scope<0>()->window<0>()) {
            return;
        }
        {
            auto layout = scope<0>()->camera<0>()->Find<ViewportLayoutComponent>();
            layout->margin_horizontal(width / 3);
            layout->SetupViewport(width, height);
        }
        {
            auto layout = scope<0>()->camera<1>()->Find<ViewportLayoutComponent>();
            layout->size_height(height / 3);
            layout->size_width((float) width / height);
            layout->SetupViewport(width, height);
        }
        {
            auto layout = scope<0>()->camera<2>()->Find<ViewportLayoutComponent>();
            layout->size_height(height / 3);
            layout->size_width((float) width / height);
            layout->SetupViewport(width, height);
        }
        {
            auto layout = scope<0>()->camera<3>()->Find<ViewportLayoutComponent>();
            layout->size_height(height / 3);
            layout->size_width((float) width / height);
            layout->SetupViewport(width, height);
        }
    }

public:

    TestPawnModule(std::weak_ptr<LocalPlayerController<2, 4>> controller, std::weak_ptr<PawnActor> actor)
    : ScopeModule(controller, actor) {
        BindKeyEvent(GLFW_KEY_W, &TestPawnModule::move_front_);
        BindKeyEvent(GLFW_KEY_S, &TestPawnModule::move_back_);
        BindKeyEvent(GLFW_KEY_A, &TestPawnModule::move_left_);
        BindKeyEvent(GLFW_KEY_D, &TestPawnModule::move_right_);
        BindKeyEvent(GLFW_KEY_SPACE, &TestPawnModule::move_up_);
    }
};

class TestController : public LocalPlayerController<2, 4> {

public:
    TestController(std::weak_ptr<Level> level_) : LocalPlayerController(level_) {
    }

    void OnWindowClose(Context &context) override {
        Quit();
    }

protected:

    void OnCreate() override {
        LocalPlayerController::OnCreate();

        module().NewModule<TestBackgroundModule>(controller());

        auto pawn = level()->actor().NewActor<PawnActor>();
        pawn->Find<MovementComponent>()->MoveUp(1.0f);

        pawn->NewActor<BoxPictureColor>();
        {
            auto mesh = pawn->Find<BoxPictureColor>();
            auto transform = mesh->transform();
            transform.scale() = glm::vec3(0.5f, 0.5f, 1.0f);
            mesh->transform(transform);
        }
        {
            auto camera_ = pawn->NewActor<PerspectiveCameraComponent>();
            camera<0>() = camera_;
            camera_->vision({ 0.1f, 100.0f, });
            camera_->sight(45.0f);
            camera_->NewActor<MovementComponent>();
            camera_->Find<MovementComponent>()->Move(-5.0f);
            camera_->Find<MovementComponent>()->MoveUp(5.0f);

            auto layout = camera_->Find<ViewportLayoutComponent>();
            layout->align_horizontal(ViewportLayoutComponent::AlignHorizontal::RIGHT);
        }

        {
            auto camera_ = pawn->NewActor<OrthoCameraComponent>();
            camera<1>() = camera_;
            camera_->vision({ 0.1f, 100.0f, });
            camera_->sight({ -10.0f, 10.0f, 10.0f, -10.0f, });
            camera_->NewActor<MovementComponent>();
            camera_->Find<MovementComponent>()->Move(-5.0f);

            auto layout = camera_->Find<ViewportLayoutComponent>();
            layout->align_horizontal(ViewportLayoutComponent::AlignHorizontal::RIGHT);
            layout->align_vertical(ViewportLayoutComponent::AlignVertical::TOP);
            layout->measure_primary(ViewportLayoutComponent::MeasurePrimary::HEIGHT);
        }

        {
            auto camera_ = pawn->NewActor<OrthoCameraComponent>();
            camera<2>() = camera_;
            camera_->vision({ 0.1f, 100.0f, });
            camera_->sight({ -10.0f, 10.0f, 10.0f, -10.0f, });
            camera_->NewActor<MovementComponent>();
            camera_->Find<MovementComponent>()->RotateYaw(90.0f);
            camera_->Find<MovementComponent>()->Move(-5.0f);

            auto layout = camera_->Find<ViewportLayoutComponent>();
            layout->align_horizontal(ViewportLayoutComponent::AlignHorizontal::RIGHT);
            layout->align_vertical(ViewportLayoutComponent::AlignVertical::BOTTOM);
            layout->measure_primary(ViewportLayoutComponent::MeasurePrimary::HEIGHT);
        }

        {
            auto camera_ = pawn->NewActor<OrthoCameraComponent>();
            camera<3>() = camera_;
            camera_->vision({ 0.1f, 100.0f, });
            camera_->sight({ -10.0f, 10.0f, 10.0f, -10.0f, });
            camera_->NewActor<MovementComponent>();
            camera_->Find<MovementComponent>()->RotatePitch(-90.0f);
            camera_->Find<MovementComponent>()->Move(-5.0f);

            auto layout = camera_->Find<ViewportLayoutComponent>();
            layout->align_horizontal(ViewportLayoutComponent::AlignHorizontal::RIGHT);
            layout->measure_primary(ViewportLayoutComponent::MeasurePrimary::HEIGHT);
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
            transform.rotate() = glm::rotate(glm::mat4(1.0f), glm::radians(15.0f), (glm::vec3) Transform::direction_default()) * transform.rotate();
            mesh->transform(transform);
        }
        {
            auto mesh = actor().NewActor<RectPictureColor>("RectPictureColor");
            auto transform= mesh->transform();
            transform.translate() = glm::vec3(5, -5, 0);
            transform.scale() = glm::vec3(5);
            mesh->transform(transform);
        }
        {
            auto mesh = actor().NewActor<BoxPictureColor>("BoxPictureColor");
            auto transform= mesh->transform();
            transform.translate() = glm::vec3(-5, -5, 5);
            transform.scale() = glm::vec3(5);
            mesh->transform(transform);
        }
    }

    void OnStart() override {
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
