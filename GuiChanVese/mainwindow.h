#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "customdefines.h"
#include "segment.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/core.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Segment *seg;

public slots:
    void onCycleComplete(QString i);
    void onParametersEstimated(double pe, double h, double alpha ,double mu, double l0, double l1);
    void onSegmentationComplete();
    void onClassificationValue(QString);

private slots:
    void on_pushImg_clicked();

    void on_pushGt_clicked();

    void on_comboMethod_currentIndexChanged(int index);

    void on_pushSegment_clicked();

private:
    Ui::MainWindow *ui;

    QImage qImg;
    QImage qGt;
    QImage qSeg;

    QString sImg;
    bool bImgAvailable;

    QString sGt;
    bool bGtAvailable;
};

#endif // MAINWINDOW_H
