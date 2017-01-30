#ifndef IMAGEEXPLORER_H
#define IMAGEEXPLORER_H

#include <QDialog>
#include "customdefines.h"
#include <QFileSystemModel>

namespace Ui {
class ImageExplorer;
}

class ImageExplorer : public QDialog
{
    Q_OBJECT

public:
    explicit ImageExplorer(QWidget *parent = 0);
    void getData(QString *sImageCpy, bool *imAvail);
    ~ImageExplorer();

private slots:
    void on_imgDir_clicked(const QModelIndex &index);

    void on_imgFile_clicked(const QModelIndex &index);

private:
    Ui::ImageExplorer *ui;

    QFileSystemModel *imgDirModel;
    QFileSystemModel *imgFileModel;

    QImage qImage;

    QString sImage  = noImgPath;

    bool sImageAvailable = false;
};

#endif // IMAGEEXPLORER_H
