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

WGPUDevice request_device(
    WGPUAdapter adapter,
    const WGPUDeviceDescriptor *descriptor
) {
    struct UserData {
        WGPUDevice device;
        bool request_ended = false;
    };

    UserData udata;

    auto callback = [](
        WGPURequestDeviceStatus status,
        WGPUDevice adapter,
        const char *msg, void *user_data
    ) {
        UserData *data = (UserData *) user_data;

        if (status == WGPURequestDeviceStatus_Success) {
            data->device = adapter;
        }  else {
            PANIC("[render] failed to request device!\n");
        }

        data->request_ended = true;
    };

    wgpuAdapterRequestDevice(adapter, descriptor, callback, &udata);

    if (!udata.request_ended) {
        PANIC("[render] device request did not end!\n");
    }

    return udata.device;
}


void on_device_error(WGPUErrorType err_ty, const char *msg, void *udata) {
    PANIC("[render] uncaptured device error: type %d (%s)!\n", err_ty, msg);
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

    usize feature_count = wgpuAdapterEnumerateFeatures(adapter, nullptr);
    auto features = new WGPUFeatureName[feature_count];
    wgpuAdapterEnumerateFeatures(adapter, features);

    printf("features: ");
    for (usize i = 0; i < feature_count; i++) {
        printf("%u, ", features[i]);
    }
    printf("\n");

    
    WGPUDeviceDescriptor device_desc;
    device_desc.label = "Render.Device";
    device_desc.nextInChain = nullptr;
    device_desc.requiredFeaturesCount = 0;
    device_desc.requiredLimits = nullptr;
    device_desc.defaultQueue.nextInChain = nullptr;
    device_desc.defaultQueue.label = "Render.DefaultQueue";

    WGPUDevice device = request_device(adapter, &device_desc);
    wgpuDeviceSetUncapturedErrorCallback(device, on_device_error, nullptr);

    WGPUQueue queue = wgpuDeviceGetQueue(device);



    WGPUCommandBuffer cmd_bufs[1];

    while (!win.should_close()) {
        win.poll_events();


        WGPUCommandEncoderDescriptor encoder_desc = {};
        encoder_desc.nextInChain = nullptr;
        encoder_desc.label = "Command Encoder";

        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(
            device, &encoder_desc
        );

        WGPUCommandBufferDescriptor cmd_buf_desc = {};
        cmd_buf_desc.label = "Command Buffer";
        WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &cmd_buf_desc);
        cmd_bufs[0] = command;

        wgpuQueueSubmit(queue, 1, cmd_bufs);
    }

    win.destroy();

    wgpuAdapterDrop(adapter);
    wgpuInstanceDrop(instance);

    // Deinitialize all subsystems
    window_manager::shutdown();

    return 0;
}
