#ifndef SIMPLECONTOURPUPILDETECT_H
#define SIMPLECONTOURPUPILDETECT_H

#include <iostream>
#include "pupildetect.h"

class SimpleContourPupilDetect: public pupilDetect
{


public:
    SimpleContourPupilDetect();

    void detect(cv::Mat frame) ;

    void calibrate();


};

#endif // SIMPLECONTOURPUPILDETECT_H
