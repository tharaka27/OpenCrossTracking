#include "simplehoughpupildetect.h"

SimpleHoughPupilDetect::SimpleHoughPupilDetect()
{

}


/**
 * @brief Eye::houghTransform
 * @param circles a list of circles found from hough transfrom
 * @param kernelSize    Kernel size of the median filter
 * @param minDistance   change this value to detect circles with different
 * distances to each other
 * @param minRadius minimum radius of the hough transform detected circle
 * @param maxRadius maximum radius of the hough transform detected circle
 */
void SimpleHoughPupilDetect::houghTransform( int kernelSize, int minDistance ,
                                             int minRadius, int maxRadius)
{
    if(frame.empty())
    {
        std::cout << "Frame is empty" << std::endl;
        return;
    }

    cv::Mat temp;

    // apply median blur
    cv::medianBlur(frame, temp, kernelSize);

    // apply hough transform

    /*
     cv::HoughCircles(temp, circles, cv::HOUGH_GRADIENT, 1,
                     minDistance, 100, 30, minRadius, maxRadius);
    */
    cv::HoughCircles(temp, circles, cv::HOUGH_GRADIENT, 2,
                    minDistance, 100, 30, 40, 80);
\
    //cv::HoughCircles(temp, circles, cv::HOUGH_GRADIENT, 2, 10);

    std::cout <<  circles.size() << "s were found" << std::endl;

    temp.release();
}


void SimpleHoughPupilDetect::selectSingleEye()
{
    double previous = 1000;
    double max_area = 0;

    for( size_t i = 0; i < circles.size(); i++ )
    {
        //std::cout << "Came here" << std::endl;

        // get the point, center and the radius
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        int radius = c[2];


        // create a mask with circle inside a box
        // this was done as below as Opencv allows only rectangle ROIs.
        cv::Mat mask = cv::Mat::zeros(frame.size(), CV_8UC1);
        cv::circle(mask, center, radius, CV_RGB(255, 255, 255), CV_FILLED, 8,0);
        cv::Mat imagePart = cv::Mat::zeros(frame.size(), frame.type());
        frame.copyTo(imagePart, mask);

        //TODO: Make sure all the limits are properly checked
        // 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols
        // && 0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows in function 'Mat'

        if(center.x-radius >=0 && center.y-radius>0 &&
                center.x + radius <= imagePart.rows && center.y + radius <= imagePart.cols)
        {
            cv::Rect roi(center.x - radius, center.y - radius, 2*radius, 2*radius);

            double area = radius*radius;
            if(cv::mean(imagePart(roi))[0] < previous && max_area < area)
            {
                max_area = area;
                previous = cv::mean(imagePart(roi))[0];
                pupilCenter[0] = center.x;
                pupilCenter[1] = center.y;
                pupilCenter[2] = radius;
            }
        }

    }

    //TODO circles may not need to clear
    circles.clear();
}


void SimpleHoughPupilDetect::calibrate()
{
    std::cout<< "Simple calibration";
}

void SimpleHoughPupilDetect::detect(cv::Mat img)
{
    frame = img;

    houghTransform(5, 10, 10, 20);

    std::cout << "Circles size " << circles.size() << std::endl;

    selectSingleEye();
}


