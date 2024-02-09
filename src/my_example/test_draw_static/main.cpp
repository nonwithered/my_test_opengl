#include <my_framework/my_runtime.h>

#include "RectSingleColor.h"

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

        {
            auto transform = RectSingleColor::Instance().transform();
            transform.translate(glm::vec3(-0.5f, 0.5f, 0));
            transform.scale(glm::vec3(0.5f));
            RectSingleColor::Instance().transform(transform);
        }
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

        std::unordered_map<std::string, std::unique_ptr<Uniform_t>> uniform;
        uniform.emplace("model", new UniformMatrix4fv(std::vector<glm::mat4> { glm::mat4(1.0f) }, false));
        // uniform.emplace("model", new UniformMatrix4fv(std::vector<glm::mat4> { RectSingleColor::Instance().transform_global() }, false));

        RectSingleColor::Instance().Draw(context(), uniform);
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
