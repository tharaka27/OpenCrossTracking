#include "simpleglintdetect.h"



simpleGlintDetect::simpleGlintDetect()
{

}


void simpleGlintDetect::calibrate()
{
    std::cout<< "Simple glint calibration";
}

void simpleGlintDetect::detect(cv::Mat img)
{
    frame = img;

    if(frame.empty())
    {
        return ;
    }

    // get the point, center and the radius
    cv::Point center = cv::Point(pupilCenter[0], pupilCenter[1]);
    int radius = pupilCenter[2];
    int sampleRadius = radius * 1.5; // previous 1.5

    // create a mask with circle inside a box
    // this was done as below as Opencv allows only rectangle ROIs.
    cv::Mat mask = cv::Mat::zeros(frame.size(), CV_8UC1);
    cv::circle(mask, center, sampleRadius , CV_RGB(255, 255, 255), CV_FILLED, 8,0);
    cv::Mat imagePart = cv::Mat::zeros(frame.size(), frame.type());
    frame.copyTo(imagePart, mask);

    //cv::imshow("ROI", imagePart);
    //cv::waitKey(10);

    cv::threshold(imagePart, imagePart, 180, 255, 0);

    cv::imshow("ROI", imagePart);
    cv::waitKey(10);

    // detect the contours on the binary image using cv2.CHAIN_APPROX_NONE
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(imagePart, contours, hierarchy, cv::RETR_TREE,
                     cv::CHAIN_APPROX_SIMPLE); // prevoius cv::CHAIN_APPROX_NONE

    // get the moments
    std::vector<cv::Moments> mu(contours.size());
    for(int i=0; i < contours.size(); i++)
    {
        mu[i] = cv::moments(contours[i], false);
    }

    // get the centroid of figures
    std::vector<cv::Point> mc(contours.size());
    for(int i=0; i < contours.size(); i++)
    {
        mc[i] = cv::Point(mu[i].m10/ mu[i].m00, mu[i].m01/ mu[i].m00);
    }

    // draw contours on the original image

    cv::Mat coloredMask;
    cv::cvtColor(imagePart, coloredMask, cv::COLOR_GRAY2BGR);
    cv::drawContours(coloredMask, contours, -1, cv::Scalar(0, 255, 0), 2);
    cv::imshow("Contours drawn", coloredMask);
    cv::waitKey(5);


    // push first 4 elements to the glint centers vector
    glintCenters.clear();
    if(contours.size() >= 4)
    {
        for(int i=0; i< 4; i++)
        {
            glintCenters.push_back(mc[i]);
        }
    }
}

void simpleGlintDetect::setPupilCenter(const cv::Vec3f &value)
{
    pupilCenter = value;
}
