#include "calchistcalculator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalcHistCalculator w;
    w.show();

    return a.exec();
}
