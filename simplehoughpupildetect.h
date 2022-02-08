#ifndef SIMPLEHOUGHPUPILDETECT_H
#define SIMPLEHOUGHPUPILDETECT_H

#include "pupildetect.h"
#include <iostream>

class SimpleHoughPupilDetect: public pupilDetect
{

private:
    std::vector<cv::Vec3f> circles;

    void houghTransform( int kernelSize = 5, int minDistance = 10,
                         int minRadius = 10, int maxRadius = 20);

    void selectSingleEye();
public:
    SimpleHoughPupilDetect();

    void detect(cv::Mat frame) ;

    void calibrate();
};

#endif // SIMPLEHOUGHPUPILDETECT_H
