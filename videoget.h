#ifndef VIDEOGET_H
#define VIDEOGET_H

#include <iostream>
#include <QThread>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>

class videoGet : public QThread
{
public:
    videoGet();
    ~videoGet();
    cv::VideoCapture cap;
    cv::Mat frame;
    cv::Mat preview;
    bool isStopped;

    cv::Mat getFrame();
    cv::Mat getPreview();
    void stopCapture();

private:
    void run();
};

#endif // VIDEOGET_H
