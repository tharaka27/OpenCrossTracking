#ifndef GLINTDETECT_H
#define GLINTDETECT_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

class glintDetect
{
public:
    glintDetect();

    virtual void detect(cv::Mat frame) =0 ;

    virtual void calibrate() = 0;

    std::vector<cv::Point> getGlintCenters();

protected:
    cv::Mat frame;

    std::vector<cv::Point> glintCenters;

};

#endif // GLINTDETECT_H
