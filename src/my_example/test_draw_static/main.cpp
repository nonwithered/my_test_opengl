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
        Level::OnCreate();
        actor().insert(Model<RectSingleColor>::Make());
        actor().insert(Model<RectMultiColor>::Make());
        actor().insert(Model<RectPictureColor>::Make());
    }

};

class TextLevelModule : public ScopeModule<Level> {

private:

    void OnCreate() override {
        ScopeModule::OnCreate();
        ListenKeyEvent(GLFW_KEY_ESCAPE, true, [this]() -> bool {
            data()->Finish();
            return false;
        });
    }

public:

    TextLevelModule(std::weak_ptr<Level> level) : ScopeModule(std::move(level)) {
    }

private:

    bool OnFrame() override {
        glViewport(0, 0, context().width(), context().height());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (auto i = 0; i != data()->actor().size(); ++i) {
            auto actor = data()->actor().at(i);
            auto static_mesh_actor = dynamic_cast<StaticMeshActor *>(actor.get());
            if (static_mesh_actor) {
                static_mesh_actor->Draw(context(), StaticMeshActor::uniform_t());
            }
        }
        return ScopeModule::OnFrame();
    }

};

class TestLauncherModule : public LauncherModule {

private:

    void OnCreate() override {
        LauncherModule::OnCreate();
        glEnable(GL_DEPTH_TEST);
        auto level = Model<TestLevel>::Make();
        context().global().level().StartLevel(level);
        NewModule(std::make_unique<TextLevelModule>(level));
    }

public:

    TestLauncherModule() = default;
};

int main() {

    Runtime runtime;

    runtime.NewWindow(
        TAG,
        800,
        600,
        [](Context &context) -> auto {
            return std::make_unique<TestLauncherModule>();
        }
    );

    runtime.Loop();

    return EXIT_SUCCESS;
}
