#include "calchistcalculator.h"
#include "ui_calchistcalculator.h"

#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>

#include "opencv2/imgproc/types_c.h"

#include "histcalcthread.h"


using namespace cv;


CalcHistCalculator::CalcHistCalculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CalcHistCalculator)
{
    ui->setupUi(this);
}

CalcHistCalculator::~CalcHistCalculator()
{
    if (this->thread != nullptr)
    {
        if (this->thread->isRunning())
            this->thread->terminate();
        delete this->thread;
    }
    delete ui;
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
        ui->menuHistogram->setEnabled(true);
    }

}

void CalcHistCalculator::on_actionExit_triggered()
{
    QApplication::exit();
}

void CalcHistCalculator::drawHistogram()
{
    showPixmap(ui->histogramView, hist_pix);
    ui->menuHistogram->setEnabled(true);
    ui->menuFile->setEnabled(true);
    histogram_ready = true;
}

void CalcHistCalculator::on_actionCalculate_triggered()
{    
    if (this->thread != nullptr)
    {
        if (this->thread->isRunning())
            this->thread->terminate();
        delete this->thread;
        this->thread = nullptr;
    }
    thread = new HistCalcThread(this->image, this->histogram, this->hist_pix);
    connect(thread, SIGNAL(finished()), this, SLOT(drawHistogram()));
    ui->menuHistogram->setEnabled(false);
    ui->menuFile->setEnabled(false);
    thread -> start();
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
