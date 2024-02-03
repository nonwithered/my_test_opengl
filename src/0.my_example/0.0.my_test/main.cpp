#include "my_temp/test_rect_move.h"

int main()
{
    Runtime runtime;

    runtime.NewWindow(
        "my_test",
        800,
        600,
        [](Context &context) {
            context.NewModule(std::make_unique<LauncherModule>(context));
        }
    );
    
    runtime.Loop();

    return EXIT_SUCCESS;
}
