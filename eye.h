#ifndef EYE_H
#define EYE_H

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include "pupildetect.h"
#include "glintdetect.h"
#include "eyetracking.h"
#include "simplehoughpupildetect.h"
#include "simplecontourpupildetect.h"
#include "simpleglintdetect.h"
#include "simpleeyetracking.h"


class Eye
{
public:
    Eye();
    cv::Mat getFrame() const;    
    void setFrame(cv::Mat source);
    cv::Mat getOneEye();
    int getNumEyes();
    void detect();
    cv::Vec3f getPupilCenter();
    ~Eye();

private:
    cv::Mat frame;
    cv::Mat eyeOne;
    int numEyes;
    cv::CascadeClassifier eyesCascade;
    pupilDetect *simpledetect;
    simpleGlintDetect *simpleglint;
    eyeTracking *simpletracking;

    cv::Vec3f pupilCenter;
    std::vector<cv::Point> glintPositions;

    void extractFrame();


};

#endif // EYE_H
