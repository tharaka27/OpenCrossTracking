#include "eyetracking.h"


eyeTracking::eyeTracking()
{

}

void eyeTracking::setPupilCenter(const cv::Vec3f &value)
{
    pupilCenter = value;
}

void eyeTracking::setGlintPositions(const std::vector<cv::Point> &value)
{
    glintPositions = value;
}

cv::Point2i eyeTracking::getGazePoint() const
{
    return gazePoint;
}


