#ifndef CALCHISTCALCULATOR_H
#define CALCHISTCALCULATOR_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <QMainWindow>

using namespace cv;

namespace Ui {
class CalcHistCalculator;
}

class CalcHistCalculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalcHistCalculator(QWidget *parent = 0);
    ~CalcHistCalculator();

private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_actionCalculate_triggered();

    void on_actionSave_triggered();

    void on_actionAbout_triggered();

private:
    Ui::CalcHistCalculator *ui;
    Mat image, histogram;
    QPixmap image_pix, hist_pix;
    bool image_ready = false, histogram_ready = false;
};

#endif // CALCHISTCALCULATOR_H
