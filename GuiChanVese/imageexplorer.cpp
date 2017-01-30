#include "imageexplorer.h"
#include "ui_imageexplorer.h"

ImageExplorer::ImageExplorer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageExplorer)
{
    ui->setupUi(this);
    //ui->imgLbl->setPixmap(QPixmap::fromImage(noImgPath));

    QString sPath = "C:/";

    imgDirModel = new QFileSystemModel(this);
    imgDirModel->setReadOnly(true);
    imgDirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    imgDirModel->setRootPath(sPath);
    ui->imgDir->setModel(imgDirModel);

    imgFileModel = new QFileSystemModel(this);
    imgFileModel->setReadOnly(true);
    imgFileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    imgFileModel->setRootPath(sPath);
    ui->imgFile->setModel(imgFileModel);

    qImage.load(sImage);
    if (qImage.width()>ui->imgLbl->width() && qImage.width()>=qImage.height())
    {
        qImage = qImage.scaledToWidth(ui->imgLbl->width());
    }
    else if (qImage.height()>ui->imgLbl->height() && qImage.height()>=qImage.width())
    {
        qImage = qImage.scaledToHeight(ui->imgLbl->height());
    }

    ui->imgLbl->setPixmap(QPixmap::fromImage(qImage));

}

ImageExplorer::~ImageExplorer()
{
    delete ui;
}

void ImageExplorer::on_imgDir_clicked(const QModelIndex &index)
{
    QString sPath = imgDirModel->fileInfo(index).absoluteFilePath();
    ui->imgFile->setRootIndex(imgFileModel->setRootPath(sPath));
}

void ImageExplorer::on_imgFile_clicked(const QModelIndex &index)
{
    QString ext = imgFileModel->fileInfo(index).completeSuffix();

    if (ext=="jpg" || ext=="png" || ext=="JPG" || ext=="PNG")
    {
        sImage = imgFileModel->fileInfo(index).absoluteFilePath();
        sImageAvailable = true;
        qImage.load(imgFileModel->fileInfo(index).absoluteFilePath());

        if (qImage.width()>ui->imgLbl->width() && qImage.width()>=qImage.height())
        {
            qImage = qImage.scaledToWidth(ui->imgLbl->width());
        }
        else if (qImage.height()>ui->imgLbl->height() && qImage.height()>=qImage.width())
        {
            qImage = qImage.scaledToHeight(ui->imgLbl->height());
        }

        ui->imgLbl->setPixmap(QPixmap::fromImage(qImage));
    }
    else
    {
        QString sPath = noImgPath;
        sImage = sPath;
        sImageAvailable = false;
        qImage.load(sPath);
        ui->imgLbl->setPixmap(QPixmap::fromImage(qImage));
    }
}

void ImageExplorer::getData(QString *sImageCpy, bool *imAvail)
{
    *imAvail = sImageAvailable;
    *sImageCpy = sImage;
}
