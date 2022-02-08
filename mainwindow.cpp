#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    if( !eyes_cascade.load( "/home/laptop/OpenCrossTracking/eye_casscade.xml" ) )
    {
        std::cout << "--(!)Error loading eyes cascade\n";
        return;
    };
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_open_webcam_clicked()
{
    cap.open(0);

    if(!cap.isOpened())
    {
        std::cout << "camera is not open" << std::endl;
    }
    else
    {
        std::cout << "camera is open" << std::endl;

        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(100);
    }
}

void MainWindow::on_pushButton_close_webcam_clicked()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
    cap.release();

    cv::Mat image = cv::Mat::zeros(frame.size(),CV_8UC3);

    qt_image = QImage((const unsigned char*) (image.data), image.cols, image.rows, QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(qt_image));

    ui->label->resize(ui->label->pixmap()->size());

    std::cout << "camera is closed" << std::endl;
}

void MainWindow::update_window()
{
    //cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920); //1280
    //cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080); //720
    cap >> Original;

    cv::cvtColor(Original, Original, CV_BGR2RGB);

    int down_width = 300;
    int down_height = 200;
    cv::Mat resized_down;
        //resize down
    cv::resize(Original, frame, Size(down_width, down_height), cv::INTER_LINEAR);

    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    ui->label->setPixmap(QPixmap::fromImage(qt_image));

    ui->label->resize(ui->label->pixmap()->size());

    //String eyes_cascade_name = samples::findFile("eye_casscade.xml");



    cv::Mat returned = detectAndDisplay(frame);
    QImage qt_image2 = QImage((const unsigned char*) (returned.data),
                              returned.cols, returned.rows, QImage::Format_Grayscale8);
    ui->label_2->setPixmap(QPixmap::fromImage(qt_image2));

    ui->label_2->resize(ui->label_2->pixmap()->size());


    //cv::Mat returned2 = detectAndDisplayCopy(frame);
    cv::Mat returned2 = detectAndDisplayCopy(Original);

    QImage qt_image3 = QImage((const unsigned char*) (returned2.data),
                              returned2.cols, returned2.rows, returned2.step, QImage::Format_Grayscale8);


    //ui->label_3->setPixmap(QPixmap::fromImage(qt_image3));

    QImage resizedImage = qt_image3.scaled(ui->label_3->size());
    ui->label_3->setPixmap(QPixmap::fromImage(resizedImage));

    //ui->label_3->resize(ui->label_3->pixmap()->size());
    ui->label_3->resize(300,200);

    ui->label_4->setPixmap(QPixmap::fromImage(resizedImage));

    //ui->label_3->resize(ui->label_3->pixmap()->size());
    ui->label_4->resize(300,200);

    QString sizeS = "Width:" + QString::number(Original.size().width) +  " Height:" +
            QString::number(Original.size().height);

    ui->label_6->setText( sizeS);

    //cv::imshow("Original", Original);
}


cv::Mat MainWindow::detectAndDisplay( Mat frame )
{
    cv::Mat frame_gray;
    cv::cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    cv::equalizeHist( frame_gray, frame_gray );

    // detect eyes
    std::vector<Rect> eyes;
    eyes_cascade.detectMultiScale( frame_gray, eyes );

    for ( size_t j = 0; j < eyes.size(); j++ )
    {
        cv::Point p1(eyes[j].x, eyes[j].y);
        cv::Scalar green(0,255,0); // Green
        int thicknessLine = 2;
        cv::circle(frame_gray, p1, 5, green, thicknessLine);
        cv::rectangle(frame_gray, eyes[j], green);
    }
    return frame_gray;
}


cv::Mat MainWindow::detectAndDisplayCopy( Mat frame )
{
    /*
    cv::Mat frame_gray;
    cv::cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    cv::equalizeHist( frame_gray, frame_gray );

    // detect eyes
    std::vector<Rect> eyes;
    eyes_cascade.detectMultiScale( frame_gray, eyes );

    std::vector<Eye> eyeList;

    for ( size_t j = 0; j < eyes.size(); j++ )
    {
        std::cout << eyes.size() << std::endl;
        std::cout << j << std::endl;
        //eyeList.push_back(Eye(cv::Mat(frame_gray, eyes[j])));

        cv::Mat dest;
        frame_gray(eyes[j]).copyTo(dest);

        eyeList.push_back(dest);
    }

    if(eyeList.size() ==0)
    {
        return cv::Mat::zeros(300,200, CV_8U);
    }
    else
    {
        return eyeList[0].getFrame();
    }
    */
    return cv::Mat::zeros(300,200, CV_8U);
}

