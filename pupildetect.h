#ifndef PUPILDETECT_H
#define PUPILDETECT_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

class pupilDetect
{
public:
    pupilDetect();

    virtual void detect(cv::Mat frame) =0 ;

    virtual void calibrate() = 0;

    //virtual cv::Vec3f getPupilCenter() = 0;

    cv::Vec3f getPupilCenter();



protected:
    cv::Mat frame;

    cv::Vec3f pupilCenter;

};

#endif // PUPILDETECT_H
