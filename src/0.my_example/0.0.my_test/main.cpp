#include "my_launch.h"

int main()
{
    Runtime runtime;

    runtime.NewWindow(
        "my_test",
        800,
        600,
        [](Context &w) {
            w.NewModule(std::make_unique<BackgroundModule>());
        }
    );
    
    runtime.Loop();

    return EXIT_SUCCESS;
}
