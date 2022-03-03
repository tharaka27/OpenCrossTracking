#include "eye.h"

Eye::Eye()
{
    if( !eyesCascade.load( "/home/laptop/OpenCrossTracking/eye_casscade.xml" ) )
    {
        std::cout << "--(!)Error loading eyes cascade\n";
        return;
    };

    simpledetect = new SimpleHoughPupilDetect();
    //simpledetect = new SimpleContourPupilDetect();

    simpleglint = new simpleGlintDetect();

    simpletracking = new simpleEyeTracking();
}

void Eye::setFrame(cv::Mat source)
{
    frame = source;
    extractFrame();
}

Eye::~Eye()
{
    free(simpledetect);
    free(simpleglint);
    free(simpletracking);
    frame.release();
    eyeOne.release();
}

cv::Mat Eye::getFrame() const
{
    return frame;
}

void Eye::extractFrame()
{
    // convert and equalize the image
    cv::Mat frame_gray;
    cv::cvtColor( frame, frame_gray, cv::COLOR_BGR2GRAY );
    //cv::equalizeHist( frame_gray, frame_gray );
    //std::cout << "Frame gray " << frame_gray.size() << std::endl;

    // detect eyes
    std::vector<cv::Rect> eyes;
    eyesCascade.detectMultiScale( frame_gray, eyes );

    numEyes = eyes.size();

    if(eyes.size() > 0)
    {
        eyeOne = frame_gray(eyes[0]);
    }
    frame_gray.release();

}

cv::Mat Eye::getOneEye()
{
    return eyeOne;
}

int Eye::getNumEyes()
{
    return numEyes;
}



void Eye::detect()
{
    // detect the pupil
    simpledetect->detect(eyeOne);

    //TODO: apply calibration functions

    // get the pupil center location
    pupilCenter = simpledetect->getPupilCenter();

    //std::cout << "Pupil x: " << pupilCenter[0] << " y: " << pupilCenter[1] << std::endl;

    simpleglint->setPupilCenter(pupilCenter);

    simpleglint->detect(eyeOne);

    glintPositions = simpleglint->getGlintCenters();


    if (glintPositions.size() >= 4)
    {

        std::cout << "Glint 1 x:" << glintPositions[0].x << " y:" << glintPositions[0].y;
         std::cout << " Glint 2 x:" << glintPositions[1].x << " y:" << glintPositions[1].y;
          std::cout << " Glint 3 x:" << glintPositions[2].x << " y:" << glintPositions[2].y;
           std::cout << " Glint 4 x:" << glintPositions[3].x << " y:" << glintPositions[3].y;
            std::cout << std::endl;

        for(int i=0; i< glintPositions.size(); i++)
        {
            if(glintPositions[i].x < 0 || glintPositions[i].x > eyeOne.rows
                    || glintPositions[i].y < 0 || glintPositions[i].y > eyeOne.cols )
                return;
        }
        /*
        cv::Mat test = cv::Mat::zeros(eyeOne.size(), CV_8UC3);
        //eyeOne.copyTo(test);
        const cv::Point* elementPoints[1] = { &glintPositions[0] };
        int numberOfPoints = (int)glintPositions.size();
        //cv::fillPoly (test, elementPoints, &numberOfPoints, 1, cv::Scalar (0, 0, 0), 8);
        cv::circle(test, cv::Point(pupilCenter[0], pupilCenter[1]), 1, cv::Scalar(255,255,255));
        cv::circle(test, cv::Point(glintPositions[0].x, glintPositions[0].y), 2, cv::Scalar(0,0,255));
        cv::circle(test, cv::Point(glintPositions[1].x, glintPositions[1].y), 2, cv::Scalar(255,0,0));
        cv::circle(test, cv::Point(glintPositions[2].x, glintPositions[2].y), 2, cv::Scalar(0,255,0));
        cv::circle(test, cv::Point(glintPositions[3].x, glintPositions[3].y), 2, cv::Scalar(255,255,255));
        cv::imshow("polygon", test);
        cv::waitKey(10);
        */

        // pass the pupil center positions to the eye
        // tracking algorithm
        simpletracking->setPupilCenter(pupilCenter);

        // pass the glint positions to the eye tracking
        // algorithm
        simpletracking->setGlintPositions(glintPositions);

        // commence eye tracking
        simpletracking->track();

        // get gaze points
        cv::Point2f gazePoints = simpletracking->getGazePoint();

        //std::string p = "x:" + std::to_string(gazePoints.x) +" y:" + std::to_string(gazePoints.y);
        //cv::putText(test, p, cv::Point(10,10), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0,0));
        //cv::imshow("polygon", test);
        //cv::waitKey(10);

    }

}

cv::Vec3f Eye::getPupilCenter()
{
    return pupilCenter;
}




















