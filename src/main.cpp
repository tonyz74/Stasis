#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <webgpu.h>
#include <wgpu.h>

#include "core/common.h"
#include "window/window.h"
#include "window/glfw3webgpu/glfw3webgpu.h"

WGPUAdapter request_adapter(
    WGPUInstance instance,
    WGPURequestAdapterOptions const* opts
) {
    struct UserData {
        WGPUAdapter adapter = nullptr;
        bool request_ended = false;
    };

    UserData udata;
    
    auto callback = [](
        WGPURequestAdapterStatus status,
        WGPUAdapter adapter,
        const char *msg, void *user_data
    ) {
        UserData *data = (UserData *) user_data;

        if (status == WGPURequestAdapterStatus_Success) {
            data->adapter = adapter;
        }  else {
            PANIC("[render] failed to request adapter!\n");
        }

        data->request_ended = true;
    };

    wgpuInstanceRequestAdapter(instance, opts, callback, &udata);

    if (!udata.request_ended) {
        PANIC("[render] adapter request did not end!\n");
    }

    return udata.adapter;
}

i32 main(void) {
    // Initialize all subsystems
    window_manager::startup();

    // Create window
    window win = window::create((window_opts) {
        .title = "Stasis",
        .width = 800,
        .height = 600
    });

    // Instance
    WGPUInstanceDescriptor desc = {};
    desc.nextInChain = nullptr;

    WGPUInstance instance = wgpuCreateInstance(&desc);

    // Surface
    WGPUSurface surface = glfwGetWGPUSurface(instance, win.inner);

    // Adapter
    WGPURequestAdapterOptions opts = {};
    opts.nextInChain = nullptr;
    opts.compatibleSurface = surface;

    WGPUAdapter adapter = request_adapter(instance, &opts);

    while (!win.should_close()) {
        win.poll_events();
    }

    win.destroy();

    wgpuAdapterDrop(adapter);
    wgpuInstanceDrop(instance);

    // Deinitialize all subsystems
    window_manager::shutdown();

    return 0;
}
