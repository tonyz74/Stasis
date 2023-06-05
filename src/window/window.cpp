#include "window.h"
#include <GLFW/glfw3.h>



// window manager

void window_manager::startup() {
    i32 code = glfwInit();
    if (!code) {
        PANIC("[window] Failed to initialize window manager!");
    }
}

void window_manager::shutdown() {
    glfwTerminate();
}


// window

window window::create(window_opts opts) {
    window self;

    self.opts = opts;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    self.inner = glfwCreateWindow(
        opts.width, opts.height,
        opts.title, NULL, NULL
    );

    if (self.inner == NULL) {
        PANIC("[window] Failed to create window!"); 
    }

    return self;
}

void window::destroy() {
    glfwDestroyWindow(inner);
}

bool window::should_close() {
    return glfwWindowShouldClose(inner);
}

void window::poll_events() {
    glfwPollEvents();
}
