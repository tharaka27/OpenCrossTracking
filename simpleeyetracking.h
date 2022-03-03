#ifndef SIMPLEEYETRACKING_H
#define SIMPLEEYETRACKING_H

#include "eyetracking.h"
#include <math.h>

class simpleEyeTracking : public eyeTracking
{
public:
    simpleEyeTracking();

    void calibrate();

    void track();

    void sortPoints(std::vector<cv::Point> &pointList);




private:
    //bool sortbyY(const cv::Point &first,
    //             const cv::Point &second);

    //bool sortbyX(const cv::Point &first,
    //             const cv::Point &second);
     void warpPerspective();
};

#endif // SIMPLEEYETRACKING_H
