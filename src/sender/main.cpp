#include <cstdlib>
#include <iostream>
#include <Processing.NDI.Lib.h>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>


int main(int argc, char** argv)
{
    // found this thread about slow loading of webcamera using OpenCV with the following solution that was incorporated into OpenCV
    // https://github.com/opencv/opencv/issues/17687#issuecomment-872291073 
    putenv("OPENCV_VIDEOIO_MSMF_ENABLE_HW_TRANSFORMS=0");

    // video capture
    cv::VideoCapture capture;

    if (!capture.open(0)) {
        std::cerr << "Could not open the camera." << std::endl;
        return 1;
    }
    else {
        char tmp[100];
        sprintf_s(tmp, "Camera opened with image dimensions: (%f, %f)", capture.get(cv::CAP_PROP_FRAME_WIDTH), capture.get(cv::CAP_PROP_FRAME_HEIGHT));
        std::cout << tmp << std::endl;
    }

    //initialize
    if (!NDIlib_initialize())
        return 0;
    NDIlib_send_create_t sender_desc;
    sender_desc.p_ndi_name = "Demo sender";
    NDIlib_send_instance_t sender = NDIlib_send_create(&sender_desc);
    std::cout << "NDI sender source name: " << NDIlib_send_get_source_name(sender)->p_ndi_name << std::endl;


    NDIlib_video_frame_v2_t video_frame;
    video_frame.xres = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    video_frame.yres = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    video_frame.FourCC = NDIlib_FourCC_type_BGRA;
    video_frame.p_data = (uint8_t*)malloc(video_frame.xres * video_frame.yres * 4);

    cv::Mat frame;
    cv::namedWindow("Webcam");
    cv::Mat ch[4];
    char metadata[100];
    int frame_counter = 0;

    for (int i = 0; i < 100; i++) {
        capture >> frame;
        std::cout << frame.size().height << frame.size().width << std::endl;
        if (frame.type() != CV_8UC4) {
            std::cout << "It is not a 4 channel image" << std::endl;
            cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);
        }
        std::cout << "Now it has " << frame.channels() << " channels." << std::endl;
        std::cout << "Send frame continuous? " << frame.isContinuous() << std::endl;
        cv::split(frame, ch);
        if (video_frame.p_data)
            memcpy((void*)video_frame.p_data, frame.ptr(), static_cast<size_t>(video_frame.xres * video_frame.yres * 4));
        /*if (video_frame.p_data)
            memset((void*)video_frame.p_data, (i & 1) ? 255 : 0, static_cast<size_t>(video_frame.xres * video_frame.yres * 4));*/
        sprintf_s(metadata, "<framestamp>%d</framestamp>", frame_counter);
        video_frame.p_metadata = metadata;

        NDIlib_send_send_video_v2(sender, &video_frame);

        cv::imshow("Webcam", frame);
        
        cv::waitKey(10);
        frame_counter++;
    }

    capture.release();
    cv::destroyAllWindows();

    NDIlib_destroy();
	
    return 0;
}