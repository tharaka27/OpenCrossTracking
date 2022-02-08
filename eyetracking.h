#ifndef EYETRACKING_H
#define EYETRACKING_H

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>

class eyeTracking
{
protected:
    cv::Vec3f pupilCenter;
    std::vector<cv::Point> glintPositions;
    cv::Point2i gazePoint;

public:
    eyeTracking();

    virtual void calibrate() = 0;

    virtual void track() = 0;

    void setPupilCenter(const cv::Vec3f &value);

    void setGlintPositions(const std::vector<cv::Point> &value);

    cv::Point2i getGazePoint() const;
};

#endif // EYETRACKING_H
