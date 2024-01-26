#include <Processing.NDI.Lib.h>
#include <chrono>
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char** argv)
{
    //initialize
    if (!NDIlib_initialize()) {
        std::cerr << "Failed to initialize NDI library." << std::endl;
        return 0;
    }

    NDIlib_recv_create_v3_t recv_desc;
    recv_desc.color_format = NDIlib_recv_color_format_BGRX_BGRA;
    
    NDIlib_recv_instance_t receiver = NDIlib_recv_create_v3(&recv_desc);
    if (!receiver) {
        std::cerr << "Failed to create a receiver." << std::endl;
        return 0;
    }

    NDIlib_source_t sender;
    sender.p_ndi_name = "ANAND-RAZER (Demo sender)";

    NDIlib_video_frame_v2_t video_frame;
    
    NDIlib_recv_connect(receiver, &sender);

    const auto start = std::chrono::high_resolution_clock::now();
    int counter = 0;
    char filename[25];
    while (std::chrono::high_resolution_clock::now() - start < std::chrono::seconds(30)) {
        switch (NDIlib_recv_capture_v3(receiver, &video_frame, NULL, NULL, 1000)) {
        case NDIlib_frame_type_video:
            std::cout << "Received image dimensions: " << video_frame.xres << " x " << video_frame.yres << std::endl;
            std::cout << "    with metadata: " << video_frame.p_metadata << std::endl;
            sprintf_s(filename, "%04d.jpg", counter);
            stbi_write_jpg(filename, video_frame.xres, video_frame.yres, 4, video_frame.p_data, 50);
            NDIlib_recv_free_video_v2(receiver, &video_frame);
            counter++;
        case NDIlib_frame_type_none:
            std::cerr << "Received nothing." << std::endl;
        }
    }

    NDIlib_recv_destroy(receiver);

    NDIlib_destroy();
    
    return 0;
}