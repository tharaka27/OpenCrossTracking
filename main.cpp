//#include <QGuiApplication>
//#include <QQmlApplicationEngine>
#include "mainwindow.h"
#include <QApplication>

#include <string>
#include <iostream>
#include <sstream>
#include "videoget.h"
using namespace std;




int main(int argc, char *argv[])
{
    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
    */

    videoGet* cap = new videoGet();

    cap->start();
    Eye eye;

    while(true)
    {

        cv::Mat frame = cap->getFrame();

        if(!frame.empty())
        {

            eye.setFrame(frame);

            eye.detect();

            cv::Vec3f pupilCenter = eye.getPupilCenter();

            cv::Mat preview = eye.getOneEye();

            if(!preview.empty()){
                cv::imshow("preview",preview);
                cv::waitKey(10);
            }

            //preview = frame;

            cv::Point center = cv::Point(pupilCenter[0], pupilCenter[1]);

            // circle center
            circle( preview, center, 1, Scalar(0,100,100), 3, LINE_AA);

            // circle outline
            int radius = pupilCenter[2];
            circle( preview, center, radius, Scalar(255,0,255), 3, LINE_AA);

            std::string str = "radius is " + std::to_string(pupilCenter[2]);
            cv::Point origin(10,30);
            cv::Scalar colour(255,255,255);
            cv::putText(preview, str, origin, cv::FONT_HERSHEY_COMPLEX ,1,colour );

            if(!preview.empty())
                cv::imshow("Eye Feed", preview);
            cv::imshow("Feed",frame);
            cv::waitKey(10);

        }
        frame.release();
    }

    free(cap);

    return 0;
}
