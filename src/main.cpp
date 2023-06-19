#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <webgpu.h>
#include <wgpu.h>

#include "core/common.h"
#include "window/window.h"
#include "window/glfw3webgpu/glfw3webgpu.h"

#include "platform/fs.h"

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


    WGPUSwapChainDescriptor swapchain_desc = {};
    swapchain_desc.label = "Render.SwapChain";
    swapchain_desc.nextInChain = nullptr; 
    swapchain_desc.width = 800;
    swapchain_desc.height = 600;

    WGPUTextureFormat swapchain_format = wgpuSurfaceGetPreferredFormat(
        surface, adapter
    );
    swapchain_desc.format = swapchain_format;
    swapchain_desc.usage = WGPUTextureUsage_RenderAttachment;
    swapchain_desc.presentMode = WGPUPresentMode_Fifo;
    WGPUSwapChain swapchain = wgpuDeviceCreateSwapChain(device, surface, &swapchain_desc);

    WGPUCommandBuffer cmd_bufs[1];


    file f = file::open("/users/tony/desktop/programming/stasis/run.py", file_mode::READ).unwrap();
    int max_len = f.get_size();
    u8 *output_buf = new u8[max_len + 1];
    int len = f.read_all(output_buf, max_len);
    output_buf[len] = 0;
    INFO("read file, result: \n%s\n", output_buf);
    f.close();


    f = file::open("/users/tony/desktop/test_write.txt", file_mode::WRITE).unwrap();

    u8 buf[] = "HELLO WORLD!!!\n";
    f.write_all(buf, sizeof(buf) - 1);

    f.close();

    while (!win.should_close()) {
        win.poll_events();

        WGPUTextureView frame_texture = wgpuSwapChainGetCurrentTextureView(swapchain);

        if (!frame_texture) {
            PANIC("[render] could not get texture for frame!\n");
        }

        WGPUCommandEncoderDescriptor encoder_desc = {};
        encoder_desc.nextInChain = nullptr;
        encoder_desc.label = "Render.CommandEncoder";

        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(
            device, &encoder_desc
        );  

        WGPUCommandBufferDescriptor cmd_buf_desc = {};
        cmd_buf_desc.label = "Render.CommandBuffer";


        WGPURenderPassColorAttachment render_pass_color_attachment = {};
        render_pass_color_attachment.view = frame_texture;
        render_pass_color_attachment.resolveTarget = nullptr;
        render_pass_color_attachment.loadOp = WGPULoadOp_Clear;
        render_pass_color_attachment.storeOp = WGPUStoreOp_Store;
        render_pass_color_attachment.clearValue = WGPUColor { 0.9, 0.1, 0.2, 1.0 };

        WGPURenderPassDescriptor render_pass_desc = {};
        render_pass_desc.nextInChain = nullptr;
        render_pass_desc.colorAttachmentCount = 1;
        render_pass_desc.colorAttachments = &render_pass_color_attachment;
        render_pass_desc.depthStencilAttachment = nullptr;

        WGPURenderPassEncoder render_pass = wgpuCommandEncoderBeginRenderPass(
            encoder, &render_pass_desc
        );
        wgpuRenderPassEncoderEnd(render_pass);


        WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &cmd_buf_desc);
        cmd_bufs[0] = command;
        // Already dropped due to finish, don't worry
        // wgpuCommandEncoderDrop(encoder);

        // Drops the command buffers automatically
        wgpuQueueSubmit(queue, 1, cmd_bufs);

        wgpuTextureViewDrop(frame_texture);
        wgpuSwapChainPresent(swapchain);
    }

    win.destroy();

    wgpuSwapChainDrop(swapchain);
    wgpuAdapterDrop(adapter);
    wgpuInstanceDrop(instance);

    // Deinitialize all subsystems
    window_manager::shutdown();

    return 0;
}
