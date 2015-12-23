#include "histcalcthread.h"

using namespace cv;

void calcHistogram(Mat& gray, Mat& histImage)
{
    // Initialize parameters
    int histSize = 256;    // bin size
    float range[] = { 0, 255 };
    const float *ranges[] = { range };

    // Calculate histogram
    MatND hist;
    calcHist( &gray, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false );

    // Plot the histogram
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    histImage = Mat( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    for( int i = 1; i < histSize; i++ )
    {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
    }
}


HistCalcThread::HistCalcThread(cv::Mat& Image, cv::Mat& Histogram, QPixmap &Pixmap): QThread(),
    histogram(Histogram),
    image(Image),
    pixmap(Pixmap)
{ }

void HistCalcThread::run()
{
    calcHistogram(image, histogram);
    this->pixmap = ASM::cvMatToQPixmap(histogram);
}
