#ifndef SIMPLEGLINTDETECT_H
#define SIMPLEGLINTDETECT_H

#include "glintdetect.h"
class simpleGlintDetect: public glintDetect
{
private:
    cv::Vec3f pupilCenter;

public:
    simpleGlintDetect();

    void detect(cv::Mat frame) ;

    void calibrate();

    void setPupilCenter(const cv::Vec3f &value);

};

#endif // SIMPLEGLINTDETECT_H
