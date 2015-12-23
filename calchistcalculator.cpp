#include "calchistcalculator.h"
#include "ui_calchistcalculator.h"

#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>

#include "opencv2/imgproc/types_c.h"


using namespace cv;

namespace ASM {
   inline QImage  cvMatToQImage( const cv::Mat &inMat )
   {
      switch ( inMat.type() )
      {
         // 8-bit, 4 channel
         case CV_8UC4:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

            return image;
         }

         // 8-bit, 3 channel
         case CV_8UC3:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

            return image.rgbSwapped();
         }

         // 8-bit, 1 channel
         case CV_8UC1:
         {
            static QVector<QRgb>  sColorTable;

            // only create our color table once
            if ( sColorTable.isEmpty() )
            {
               for ( int i = 0; i < 256; ++i )
                  sColorTable.push_back( qRgb( i, i, i ) );
            }

            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

            image.setColorTable( sColorTable );

            return image;
         }
      }

      return QImage();
   }

   inline QPixmap cvMatToQPixmap( const cv::Mat &inMat )
   {
      return QPixmap::fromImage( cvMatToQImage( inMat ) );
   }
}


CalcHistCalculator::CalcHistCalculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CalcHistCalculator)
{
    ui->setupUi(this);
}

CalcHistCalculator::~CalcHistCalculator()
{
    delete ui;
}

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


void showPixmap(QGraphicsView* view, QPixmap& pixmap)
{
    QGraphicsScene* scene = new QGraphicsScene(view);
    view->setScene(scene);
    scene->addPixmap(pixmap);
}

void CalcHistCalculator::on_actionOpen_triggered()
{
    QString fname = QFileDialog::getOpenFileName(this, tr("Open file..."), QString(), tr("JPG (*.jpg)"));
    if (!fname.isEmpty())
    {
        this->image = imread(fname.toStdString(), 0);
        image_pix = ASM::cvMatToQPixmap(this->image);
        showPixmap(ui->imageView, image_pix);
        this->image_ready = true;
        if (ui->histogramView->scene())
            ui->histogramView->scene()->clear();
    }

}

void CalcHistCalculator::on_actionExit_triggered()
{
    QApplication::exit();
}

void CalcHistCalculator::on_actionCalculate_triggered()
{
    calcHistogram(this->image, this->histogram);
    QPixmap hist_pix = ASM::cvMatToQPixmap(this->histogram);
    showPixmap(ui->histogramView, hist_pix);
    histogram_ready = true;
}

void CalcHistCalculator::on_actionSave_triggered()
{
    if (!histogram_ready)
        return;
    else
    {
        QString fname = QFileDialog::getSaveFileName(this, "Save histogram...", QString(), tr("JPG (*.jpg)"));
        if (fname.isEmpty())
            return;
        imwrite(fname.toStdString(), histogram);
    }
}

void CalcHistCalculator::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About", "This functionality is under heavy development, ETA unknown.");
}
