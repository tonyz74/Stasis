#ifndef STASIS_WINDOW_H
#define STASIS_WINDOW_H

#include "core/common.h"
#include <GLFW/glfw3.h>


struct window_manager {
    static void startup();
    static void shutdown();
};


struct window_opts {
    const char* title;
    u32 width, height; 
};

struct window {

    static window create(window_opts opts);
    void destroy();

    bool should_close();
    void poll_events();

    window_opts opts;
    GLFWwindow *inner;
};

#endif // !STASIS_WINDOW_H
