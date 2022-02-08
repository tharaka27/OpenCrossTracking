#include "videoget.h"

videoGet::videoGet()
{
    cap.open(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280); //1280 1920
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720); //720 1080
    isStopped = false;
}

videoGet::~videoGet()
{
    frame.release();
    preview.release();
}

void videoGet::run()
{
    while(!isStopped){
        if(!cap.isOpened())
        {
            std::cout << "camera is not open" << std::endl;
            continue;
        }
        cap.read(frame);
        cv::resize(frame, preview, cv::Size(), 0.5, 0.5);
    }
}

cv::Mat videoGet::getFrame()
{
    return frame;
}
cv::Mat videoGet::getPreview()
{
    return preview;
}

void videoGet::stopCapture()
{
    isStopped = true;
}
