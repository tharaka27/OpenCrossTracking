#ifndef SIMPLEEYETRACKING_H
#define SIMPLEEYETRACKING_H

#include "eyetracking.h"

class simpleEyeTracking : public eyeTracking
{
public:
    simpleEyeTracking();

    void calibrate();

    void track();
};

#endif // SIMPLEEYETRACKING_H
