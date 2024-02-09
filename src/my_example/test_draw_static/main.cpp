#include <my_framework/my_runtime.h>

#include "RectSingleColor.h"
#include "RectMultiColor.h"
#include "RectPictureColor.h"

static constexpr auto TAG = "test_draw_rect";

class TestLevel : public Level {

public:
    TestLevel() : Level(::TAG) {
    }

private:

    void OnCreate() override {
        actor().insert(Model<RectSingleColor>::Make());
        actor().insert(Model<RectMultiColor>::Make());
        actor().insert(Model<RectPictureColor>::Make());
    }

};

class TextLevelModule : public LevelModule {

private:

public:

    TextLevelModule(Context &context, std::weak_ptr<Level> level) : LevelModule(context, std::move(level)) {
        Module::KeyEvent(GLFW_KEY_ESCAPE, true, [this]() -> bool {
            this->level()->Finish();
            return false;
        });
    }

};

class TestLauncherModule : public LauncherModule {

private:

    void OnCreate() override {
        glEnable(GL_DEPTH_TEST);
        auto level = Model<TestLevel>::Make();
        context().global().level().StartLevel(level);
        NewModule(std::make_unique<TextLevelModule>(context(), level));
    }

public:

    TestLauncherModule(Context &context) : LauncherModule(context) {
    }

    // bool Frame() override {
    //     Init();
    //     glViewport(0, 0, context().width(), context().height());
    //     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //     RectSingleColor::Instance().Draw(context(), StaticMeshActor::uniform_t());
    //     RectMultiColor::Instance().Draw(context(), StaticMeshActor::uniform_t());
    //     RectPictureColor::Instance().Draw(context(), StaticMeshActor::uniform_t());
    //     return finish_;
    // }
};

int main() {

    Runtime runtime;

    runtime.NewWindow(
        "test_draw_rect",
        800,
        600,
        [](Context &context) -> auto {
            return std::make_unique<TestLauncherModule>(context);
        }
    );

    runtime.Loop();

    return EXIT_SUCCESS;
}
