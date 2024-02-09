#include <my_framework/my_runtime.h>

#include "RectSingleColor.h"
#include "RectMultiColor.h"

class LauncherModule : public Module {

private:

    static constexpr auto TAG = "LauncherModule";
    bool init_ = false;
    bool finish_ = false;

    void Init() {
        if (init_) {
            return;
        }
        init_ = true;
        glEnable(GL_DEPTH_TEST);
    }

public:

    LauncherModule(Context &context) : Module(context) {
        KeyEvent(GLFW_KEY_ESCAPE, true, [this]() -> bool {
            finish_ = true;
            return false;
        });
    }

    bool Frame() override {
        Init();
        glViewport(0, 0, context().width(), context().height());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RectSingleColor::Instance().Draw(context(), StaticMeshActor::uniform_t());
        RectMultiColor::Instance().Draw(context(), StaticMeshActor::uniform_t());
        return finish_;
    }
};

int main() {

    Runtime runtime;

    runtime.NewWindow(
        "test_draw_rect",
        800,
        600,
        [](Context &context) -> auto {
            return std::make_unique<LauncherModule>(context);
        }
    );

    runtime.Loop();

    return EXIT_SUCCESS;
}
