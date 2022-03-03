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
    //std::sort(pointList.begin(), pointList.end(), sortbyY );

    //std::sort(pointList.begin(), pointList.end(), sortbyX );

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
    cv::circle(test, cv::Point(topLeft.x, topLeft.y), 2, cv::Scalar(0,0,255));
    cv::circle(test, cv::Point(topRight.x, topRight.y), 2, cv::Scalar(255,0,0));
    cv::circle(test, cv::Point(bottomLeft.x, bottomLeft.y), 2, cv::Scalar(0,255,0));
    cv::circle(test, cv::Point(bottomRight.x, bottomRight.y), 2, cv::Scalar(255,255,255));
    cv::imshow("polygon", test);
    cv::waitKey(10);

    /*
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




    float src_base[4][2] =
    {
        {glintPositions[0].x, glintPositions[0].y },
        {glintPositions[1].x, glintPositions[1].y },
        {glintPositions[3].x, glintPositions[3].y },
        {glintPositions[2].x, glintPositions[2].y }
    };

    float dst_base[4][2] =
    {
        {0, 0},
        {maxWidth -1 , 0 },
        {maxWidth -1, maxHeight -1},
        {0, maxHeight - 1}
    };

    cv::Mat dst = cv::Mat(4, 2, CV_32F, dst_base);
    //cv::Mat src = cv::Mat(4, 2, CV_32F, src_base);

    std::vector<cv::Point2f> scene;
    std::vector<cv::Point2f> object;

    scene.push_back(cv::Point2f(glintPositions[0].x, glintPositions[0].y));
    scene.push_back(cv::Point2f(glintPositions[1].x, glintPositions[1].y));
    scene.push_back(cv::Point2f(glintPositions[3].x, glintPositions[3].y));
    scene.push_back(cv::Point2f(glintPositions[2].x, glintPositions[2].y));

    object.push_back(cv::Point2f( 0, 0 ));
    object.push_back(cv::Point2f( maxWidth -1 , 0  ));
    object.push_back(cv::Point2f( maxWidth -1, maxHeight -1 ));
    object.push_back(cv::Point2f( 0, maxHeight - 1 ));
    //std::cout << dst << std::endl;
    //std::cout << rect << std::endl;
    std::cout << "Max width " << maxWidth << std::endl;
    std::cout << "Max height " << maxHeight << std::endl;

    //cv::Mat M = cv::getPerspectiveTransform(rect, dst);

    //std::cout << src << std::endl;
    //std::cout << M << std::endl;
    //std::cout << dst << std::endl;

    //cv::Mat prediction = cv::Mat(1, 3, CV_64F, prediction_base);
    cv::Mat warped;

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

    const int rows = 720;
    const int cols = 1280;
    /*

    cv::Mat test = cv::Mat::zeros(rows, cols , CV_8UC1);
    std::string p = "x:" + std::to_string(gazePoint.x)
            + " y:" + std::to_string(gazePoint.y);

    //cv::putText(test, p, cv::Point(100,100), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0,0));
    gazePoint.x = rows - (gazePoint.x / maxWidth * rows);
    gazePoint.y = gazePoint.y / maxHeight * cols;
    cv::circle(test, gazePoint, 3, cv::Scalar(255,0,0));
    cv::imshow("datapoints", test );
    cv::waitKey(10);

    cv::Mat test = cv::Mat::zeros(maxHeight, maxWidth, CV_8UC1);
    cv::circle(test, gazePoint, 3, cv::Scalar(255,0,0));
    cv::imshow("datapoints", test );
    cv::waitKey(10);
    */
}

void simpleEyeTracking::track()
{
    //std::cout << "Heeello world" << std::endl;

    // identify four locations
    sortPoints(glintPositions);

    /*
    for(int i=0; i< glintPositions.size(); i++ )
    {
        std::cout << i << "th element is x: "
                  << glintPositions[i].x
                  << " y " << glintPositions[i].y
                  << std::endl;
    }
    */

    // check whether the eye is outside the polygon
    warpPerspective();

    // calculate the position in the screen

}
