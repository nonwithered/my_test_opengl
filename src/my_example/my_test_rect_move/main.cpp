#include <my_framework/my_runtime.h>

// #include "test_rect_move.h"

void Init(Runtime &runtime) {
    
    // runtime.model() += RectModel::Make();

    // runtime.NewWindow(
    //     "my_test",
    //     800,
    //     600,
    //     [](Context &context) -> auto {
    //         return std::make_unique<BackgroundModule>(context);
    //     }
    // );
}

int main() {
    Runtime runtime;

    // Init(runtime);
    
    runtime.Loop();

    return EXIT_SUCCESS;
}
