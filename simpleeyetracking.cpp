#include "simpleeyetracking.h"
#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

simpleEyeTracking::simpleEyeTracking()
{

}

void simpleEyeTracking::calibrate()
{

}

bool sortbyX(const cv::Point &first, const cv::Point &second)
{
    return first.x < second.x;
}

bool sortbyY(const cv::Point &first, const cv::Point &second)
{
    return first.y < second.y;
}

bool sortbydistance(const cv::Point &first, const cv::Point &second)
{
    return first.x*first.x + first.y*first.y < second.y*second.y + second.x*second.x;
}

void simpleEyeTracking::sortPoints(std::vector<cv::Point> &pointList)
{
    std::sort(pointList.begin(), pointList.end(), sortbydistance);

    std::sort(pointList.begin()+1, pointList.end()-1, sortbyX);
}

float returnMax(float x, float y)
{
    if (x>y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

void simpleEyeTracking::warpPerspective()
{
    if(glintPositions.size() < 4)
    {
        return;
    }

    // The input pts must be ordered clockwise top-left, top-right, bottom-right, bottom-left
    cv::Point topLeft = glintPositions[0];
    cv::Point topRight = glintPositions[2];
    cv::Point bottomLeft = glintPositions[1];
    cv::Point bottomRight = glintPositions[3];


    cv::Mat test = cv::Mat::zeros( 400, 400, CV_8UC3);

    cv::circle(test, cv::Point(pupilCenter[0], pupilCenter[1]), 1, cv::Scalar(255,255,255));
    cv::circle(test, cv::Point(topLeft.x, topLeft.y), 2, cv::Scalar(0,0,255)); // Red
    cv::circle(test, cv::Point(topRight.x, topRight.y), 2, cv::Scalar(255,0,0)); // blue
    cv::circle(test, cv::Point(bottomLeft.x, bottomLeft.y), 2, cv::Scalar(0,255,0)); // green
    cv::circle(test, cv::Point(bottomRight.x, bottomRight.y), 2, cv::Scalar(255,255,255)); // white
    cv::imshow("polygon", test);
    cv::waitKey(10);


    float widthA = std::sqrt( (bottomRight.x - bottomLeft.x)* (bottomRight.x - bottomLeft.x)
            +  (bottomRight.y - bottomLeft.y) * ( bottomRight.y - bottomLeft.y));
    float widthB = std::sqrt( (topRight.x - topRight.x)*(topRight.x - topRight.x) +
            (topRight.y - topRight.y) * (topRight.y - topRight.y));
    float maxWidth = returnMax(widthA, widthB);

    float heightA = std::sqrt( (topRight.x - bottomRight.x)* (topRight.x - bottomRight.x)
            +  (topRight.y - bottomRight.y) * ( topRight.y - bottomRight.y) );
    float heightB = std::sqrt( (topLeft.x - bottomLeft.x)*(topLeft.x - bottomLeft.x) +
            (topLeft.y - bottomLeft.y) * (topLeft.y - bottomLeft.y) );
    float maxHeight = returnMax(heightA, heightB);

    std::vector<cv::Point2f> scene;
    std::vector<cv::Point2f> object;

    scene.push_back(cv::Point2f(topLeft.x, topLeft.y));
    scene.push_back(cv::Point2f(topRight.x, topRight.y));
    scene.push_back(cv::Point2f(bottomRight.x, bottomRight.y));
    scene.push_back(cv::Point2f(bottomLeft.x, bottomLeft.y));

    object.push_back(cv::Point2f( 0, 0 ));
    object.push_back(cv::Point2f( maxWidth -1 , 0  ));
    object.push_back(cv::Point2f( maxWidth -1, maxHeight -1 ));
    object.push_back(cv::Point2f( 0, maxHeight - 1 ));

    //cv::Mat warped;

    cv::Mat mask;
    cv::Mat H;
    H = cv::findHomography(scene, object, mask);
    std::cout << "Homography\n" << H << std::endl;

    if(H.empty())
    {
        std::cout << "Empty Map" << std::endl;
        return;
    }

    std::vector<cv::Point2f> prediction;
    //cv::Mat source = cv::Mat(1, 3, CV_64F, prediction_base);
    std::vector<cv::Point2f> source;
    //source.push_back(cv::Point2f( 4, 5 ));
    source.push_back(cv::Point2f( pupilCenter[0], pupilCenter[1] ));
    cv::perspectiveTransform(source, prediction, H );

    gazePoint.x = prediction[0].x;
    gazePoint.y = prediction[0].y;


    std::cout << "----------------->prediction is" << prediction[0].x <<"," << prediction[0].y << std::endl;

    const int rows = 1024;  // 720
    const int cols = 1280; // 1280

    cv::Mat test2 = cv::Mat::zeros(rows, cols , CV_8UC1);

    gazePoint.x = cols - gazePoint.x / maxWidth * cols;
    gazePoint.y = gazePoint.y / maxHeight * rows ;
    cv::circle(test2, gazePoint, 3, cv::Scalar(255,0,0));
    cv::imshow("datapoints", test2 );
    cv::waitKey(10);

}

void simpleEyeTracking::track()
{

    // identify four locations
    sortPoints(glintPositions);

    // check whether the eye is outside the polygon
    warpPerspective();

    // calculate the position in the screen

}
