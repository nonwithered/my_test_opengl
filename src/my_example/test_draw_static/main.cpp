#include <my_framework/my_runtime.h>

#include "RectSingleColor.h"
#include "RectMultiColor.h"
#include "RectPictureColor.h"

static constexpr auto TAG = "test_draw_rect";

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

class TestDrawModule : public ScopeModule<Level> {

protected:

    bool OnFrame() override {
        for (auto i = 0; i != data()->actor().size(); ++i) {
            auto actor = data()->actor().at(i);
            auto static_mesh_actor = dynamic_cast<StaticMeshActor *>(actor.get());
            if (static_mesh_actor) {
                static_mesh_actor->Draw(context(), UniformParameter());
            }
        }
        return ScopeModule::OnFrame();
    }

public:

    TestDrawModule(std::weak_ptr<Level> level) : ScopeModule(std::move(level)) {
    }
};

class TestLevelModule : public ScopeModule<Level> {

private:

    void OnCreate() override {
        ScopeModule::OnCreate();

        NewModule<TestBackgroundModule>();
        NewModule<TestDrawModule>(data());

        ListenKeyEvent(GLFW_KEY_ESCAPE, true, [this]() -> bool {
            data()->Finish();
            return false;
        });
    }

public:

    TestLevelModule(std::weak_ptr<Level> level) : ScopeModule(std::move(level)) {
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
