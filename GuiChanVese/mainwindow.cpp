#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageexplorer.h"
#include "swapinterface.h"
#include "roc.h"

void setParameters(Ui::MainWindow *ui)
{
    // All are read only by default
    ui->lineEditMu->setReadOnly(true);
    ui->lineEditL0->setReadOnly(true);
    ui->lineEditL1->setReadOnly(true);

    if(ui->comboMethod->currentIndex()==eMethod::ElZehiry) // El-Zehiry et al.
    {
        ui->lineEditMu->setText(muString[0]);
        ui->lineEditL0->setText(l0String[0]);
        ui->lineEditL1->setText(l1String[0]);
    }
    else if(ui->comboMethod->currentIndex()==eMethod::Masaka) // Masaka et al.
    {
        ui->lineEditMu->setText(muString[1]);
        ui->lineEditL0->setText(l0String[1]);
        ui->lineEditL1->setText(l1String[1]);
    }
    else if(ui->comboMethod->currentIndex()==eMethod::Ryan) // Proposed
    {
        ui->lineEditMu->setText(muString[2]);
        ui->lineEditL0->setText(l0String[2]);
        ui->lineEditL1->setText(l1String[2]);
    }
    else if(ui->comboMethod->currentIndex()==eMethod::Manual) // Manual
    {
        ui->lineEditMu->setReadOnly(false);
        ui->lineEditL0->setReadOnly(false);
        ui->lineEditL1->setReadOnly(false);

        ui->lineEditMu->setText(muString[3]);
        ui->lineEditL0->setText(l0String[3]);
        ui->lineEditL1->setText(l1String[3]);
    }
}

void defaultText(Ui::MainWindow *ui)
{
    ui->plainTextEdit->appendHtml("<h1>Proposed Parameter Estimation for Discrete Chan-Vese Energy Function for Fluorescence Image Segmentation</h1>");
    ui->plainTextEdit->appendHtml("<p><i>Ryan Naidoo(208513825)</i></p>");
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    seg = new Segment(this);
    connect(seg,SIGNAL(cycleComplete(QString)),this,SLOT(onCycleComplete(QString)));
    connect(seg,SIGNAL(parametersEstimated(double,double,double,double,double,double)),this,SLOT(onParametersEstimated(double,double,double,double,double,double)));
    connect(seg,SIGNAL(segmentationComplete()),this,SLOT(onSegmentationComplete()));
    connect(seg,SIGNAL(classificationValue(QString)),this,SLOT(onClassificationValue(QString)));

    defaultText(ui);

    // energy function
    QString energy = "E<sub>CV</sub>(C,c<sub>0</sub>,c<sub>1</sub>) = <b>&mu;</b>L(C) + <b>&lambda;<sub>0</sub></b>&int;<sub>&Omega;0</sub>(f(x)-c<sub>0</sub>)<sup>2</sup> + <b>&lambda;<sub>1</sub></b>&int;<sub>&Omega;1</sub>(f(x)-c<sub>1</sub>)<sup>2</sup>";
    ui->energyLbl->setText(energy);

    // default picture when no images are selected
    qImg.load("../noimage.png");
    ui->imgLbl->setPixmap(QPixmap::fromImage(qImg));
    qGt.load(noGtPath);
    ui->gtLbl->setPixmap(QPixmap::fromImage(qGt));
    qSeg.load(noSegPath);
    ui->segLbl->setPixmap(QPixmap::fromImage(qSeg));

    // default graph density
    ui->comboN->addItem("4");
    ui->comboN->addItem("8");
    ui->comboN->setCurrentIndex(1);

    // default method
    for(int i=0; i<methodNames.size() ; i++)
    {
        ui->comboMethod->addItem(methodNames[i]);
    }
    ui->comboMethod->setCurrentIndex(2);

    // default initialisation
    for(int i=0; i<initNames.size() ; i++)
    {
        ui->comboInitial->addItem(initNames[i]);
    }
    ui->comboInitial->setCurrentIndex(3);

    // default dilation element
    for(int i=0; i<dilationElement.size() ; i++)
    {
        ui->comboDilElement->addItem(dilationElement[i]);
    }
    ui->comboDilElement->setCurrentIndex(2);

    // parameter labels
    ui->labelMu->setText("<p>&mu; =</p>");
    ui->labelL0->setText("<p>&lambda;<sub>0</sub> =</p>");
    ui->labelL1->setText("<p>&lambda;<sub>1</sub> =</p>");
    ui->labelEps->setText("<p>&epsilon; =</p>");
    ui->labelN->setText("<p>n<sub>max</sub> =</p>");

    setParameters(ui);
    ui->lineEditEps->setText(QString::number(defaultEps));
    ui->lineEditN->setText(QString::number(defaultIterations));

    ui->spinDilSize->setMinimum(0);
    ui->spinDilSize->setMaximum(7);
    ui->spinDilSize->setWrapping(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushImg_clicked()
{
    ImageExplorer myDialog;
    myDialog.setFixedSize(myDialog.size());
    myDialog.setModal(true);

    int result = myDialog.exec();

    if (result==QDialog::Accepted)
    {
        // get data
        myDialog.getData(&sImg, &bImgAvailable);

        // image
        if (bImgAvailable)
        {
            qImg.load(sImg);
            ui->imgPath->setText(sImg);
            seg->sRawImage = sImg;
        }
        else
        {
            qImg.load(noImgPath);
            ui->imgPath->setText("");
        }

        if (qImg.width()>ui->imgLbl->width() && qImg.width()>=qImg.height())
        {
            qImg = qImg.scaledToWidth(ui->imgLbl->width());
        }
        else if (qImg.height()>ui->imgLbl->height() && qImg.height()>=qImg.width())
        {
            qImg = qImg.scaledToHeight(ui->imgLbl->height());
        }
        ui->imgLbl->setPixmap(QPixmap::fromImage(qImg));

        // segmentation
        qSeg.load(noSegPath);
        if (qSeg.width()>ui->segLbl->width() && qSeg.width()>qSeg.height())
        {
            qSeg = qSeg.scaledToWidth(ui->segLbl->width());
        }
        else if (qSeg.height()>ui->segLbl->height() && qSeg.height()>qSeg.width())
        {
            qSeg = qSeg.scaledToHeight(ui->segLbl->height());
        }
        ui->segLbl->setPixmap(QPixmap::fromImage(qSeg));

        // clear data
        ui->plainTextEdit->clear();
    }
    else
    {
        defaultText(ui);

        // reset variables and UI to default state
        bImgAvailable = false;
        ui->imgPath->setText("");
        // image
        qImg.load(noImgPath);
        if (qImg.width()>ui->imgLbl->width() && qImg.width()>qImg.height())
        {
            qImg = qImg.scaledToWidth(ui->imgLbl->width());
        }
        else if (qImg.height()>ui->imgLbl->height() && qImg.height()>qImg.width())
        {
            qImg = qImg.scaledToHeight(ui->imgLbl->height());
        }
        ui->imgLbl->setPixmap(QPixmap::fromImage(qImg));

//        // ground truth
//        qGt.load(noGtPath);
//        if (qGt.width()>ui->gtLbl->width() && qGt.width()>=qGt.height())
//        {
//            qGt = qGt.scaledToWidth(ui->gtLbl->width());
//        }
//        else if (qGt.height()>ui->gtLbl->height() && qGt.height()>=qGt.width())
//        {
//            qGt = qGt.scaledToHeight(ui->gtLbl->height());
//        }
//        ui->gtLbl->setPixmap(QPixmap::fromImage(qGt));

        // segmentation
        qSeg.load(noSegPath);
        if (qSeg.width()>ui->segLbl->width() && qSeg.width()>qSeg.height())
        {
            qSeg = qSeg.scaledToWidth(ui->segLbl->width());
        }
        else if (qSeg.height()>ui->segLbl->height() && qSeg.height()>qSeg.width())
        {
            qSeg = qSeg.scaledToHeight(ui->segLbl->height());
        }
        ui->segLbl->setPixmap(QPixmap::fromImage(qSeg));

        // clear data
        ui->plainTextEdit->clear();
    }
}

void MainWindow::on_pushGt_clicked()
{
    ImageExplorer myDialog;
    myDialog.setFixedSize(myDialog.size());
    myDialog.setModal(true);

    int result = myDialog.exec();

    if (result==QDialog::Accepted)
    {
        // get data
        myDialog.getData(&sGt, &bGtAvailable);

        // image
        if (bGtAvailable)
        {
            qGt.load(sGt);
            ui->gtPath->setText(sGt);
        }
        else
        {
            qGt.load(noGtPath);
            ui->gtPath->setText("");
        }

        if (qGt.width()>ui->gtLbl->width() && qGt.width()>=qGt.height())
        {
            qGt = qGt.scaledToWidth(ui->gtLbl->width());
        }
        else if (qGt.height()>ui->gtLbl->height() && qGt.height()>=qGt.width())
        {
            qGt = qGt.scaledToHeight(ui->gtLbl->height());
        }
        ui->gtLbl->setPixmap(QPixmap::fromImage(qGt));

        // segmentation
        qSeg.load(noSegPath);
        if (qSeg.width()>ui->segLbl->width() && qSeg.width()>qSeg.height())
        {
            qSeg = qSeg.scaledToWidth(ui->segLbl->width());
        }
        else if (qSeg.height()>ui->segLbl->height() && qSeg.height()>qSeg.width())
        {
            qSeg = qSeg.scaledToHeight(ui->segLbl->height());
        }
        ui->segLbl->setPixmap(QPixmap::fromImage(qSeg));

        // clear data
        ui->plainTextEdit->clear();
    }
    else
    {
        defaultText(ui);

        // reset variables and UI to default state
        bGtAvailable = false;
        ui->gtPath->setText("");
//        // image
//        qImg.load(noImgPath);
//        if (qImg.width()>ui->imgLbl->width() && qImg.width()>qImg.height())
//        {
//            qImg = qImg.scaledToWidth(ui->imgLbl->width());
//        }
//        else if (qImg.height()>ui->imgLbl->height() && qImg.height()>qImg.width())
//        {
//            qImg = qImg.scaledToHeight(ui->imgLbl->height());
//        }
//        ui->imgLbl->setPixmap(QPixmap::fromImage(qImg));

        // ground truth
        qGt.load(noGtPath);
        if (qGt.width()>ui->gtLbl->width() && qGt.width()>=qGt.height())
        {
            qGt = qGt.scaledToWidth(ui->gtLbl->width());
        }
        else if (qGt.height()>ui->gtLbl->height() && qGt.height()>=qGt.width())
        {
            qGt = qGt.scaledToHeight(ui->gtLbl->height());
        }
        ui->gtLbl->setPixmap(QPixmap::fromImage(qGt));

        // segmentation
        qSeg.load(noSegPath);
        if (qSeg.width()>ui->segLbl->width() && qSeg.width()>qSeg.height())
        {
            qSeg = qSeg.scaledToWidth(ui->segLbl->width());
        }
        else if (qSeg.height()>ui->segLbl->height() && qSeg.height()>qSeg.width())
        {
            qSeg = qSeg.scaledToHeight(ui->segLbl->height());
        }
        ui->segLbl->setPixmap(QPixmap::fromImage(qSeg));

        // clear data
        ui->plainTextEdit->clear();
    }
}

void MainWindow::on_comboMethod_currentIndexChanged(int index)
{
    // fill parameter values
    ui->lineEditMu->setText(muString[index]);
    ui->lineEditL0->setText(l0String[index]);
    ui->lineEditL1->setText(l1String[index]);

    if (index == eMethod::Manual)
    {
        ui->lineEditMu->setReadOnly(false);
        ui->lineEditL0->setReadOnly(false);
        ui->lineEditL1->setReadOnly(false);
    }
    else
    {
        ui->lineEditMu->setReadOnly(true);
        ui->lineEditL0->setReadOnly(true);
        ui->lineEditL1->setReadOnly(true);
    }
}

void MainWindow::on_pushSegment_clicked()
{
    qSeg.load(noSegPath);
    ui->segLbl->setPixmap(QPixmap::fromImage(qSeg));

    // write to main line edit
    ui->plainTextEdit->clear();

    if (!bImgAvailable)
    {
        ui->plainTextEdit->appendHtml("<font color=red><b>No image selected.</b></font>");
        return;
    }

    QRegExp reDouble("[+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?");  // a digit (\d), zero or more times (*)
    QRegExp reInt("\\d*");

    // put some stuff on the line Edit
    ui->plainTextEdit->appendHtml("<b>Neighbourhood Connectivity: </b>" + ui->comboN->currentText());
    ui->plainTextEdit->appendHtml("<b>Parameter Estimation Method: </b>" + ui->comboMethod->currentText());
    ui->plainTextEdit->appendHtml("<b>Initialisation: </b>" + ui->comboInitial->currentText());

    if(reDouble.exactMatch(ui->lineEditEps->text()))
    {
        ui->plainTextEdit->appendHtml("<b>Termination Criterion: </b> &epsilon; =" + ui->lineEditEps->text());
    }
    else
    {
        ui->plainTextEdit->appendHtml("<font color=red><b>Improper termination criterion!</b></font>");
        return;
    }

    if(reInt.exactMatch(ui->lineEditN->text()))
    {
        ui->plainTextEdit->appendHtml("<b>Max. Iterations: </b> n<sub>max</sub> =" + ui->lineEditN->text());
    }
    else
    {
        ui->plainTextEdit->appendHtml("<font color=red><b>Improper iteration upper bound!</b></font>");
        return;
    }
    ui->plainTextEdit->appendHtml("<br/><b>Parameter Settings</b>");

    //---------------------------------------------------------------------------------------------

    bool ok = false;
    seg->method = ui->comboMethod->currentText();
    seg->sRawImage = sImg;
    if(ui->comboN->currentIndex()==0)
    {
        seg->connectivity = eInterConnectivity::FOUR_WAY;
    }
    else if(ui->comboN->currentIndex()==1)
    {
        seg->connectivity = eInterConnectivity::EIGHT_WAY;
    }

    seg->algo = ui->comboInitial->currentText();
    seg->dilate_size = ui->spinDilSize->text().toInt(&ok);
    seg->dilate_element = ui->comboDilElement->currentIndex();
    if (ui->comboMethod->currentIndex()==eMethod::Ryan)
    {
        seg->initialise();
    }
    else
    {
        seg->unproposedInitialise();
        seg->mu = ui->lineEditMu->text().toDouble(&ok);
        seg->l0 = ui->lineEditL0->text().toDouble(&ok);
        seg->l1 = ui->lineEditL1->text().toDouble(&ok);
    }

    if(reDouble.exactMatch(ui->lineEditMu->text()))
    {
        ui->plainTextEdit->appendHtml("&mu; =" + ui->lineEditMu->text());
    }
    else
    {
        ui->plainTextEdit->appendHtml("<font color=red><b>Improper &mu;!</b></font>");
        return;
    }

    if(reDouble.exactMatch(ui->lineEditL0->text()))
    {
        ui->plainTextEdit->appendHtml("&lambda;<sub>0</sub> =" + ui->lineEditL0->text());
    }
    else
    {
        ui->plainTextEdit->appendHtml("<font color=red><b>Improper &lambda;<sub>0</sub>!</b></font>");
        return;
    }

    if(reDouble.exactMatch(ui->lineEditL1->text()))
    {
        ui->plainTextEdit->appendHtml("&lambda;<sub>1</sub> =" + ui->lineEditL1->text());
    }
    else
    {
        ui->plainTextEdit->appendHtml("<font color=red><b>Improper &lambda;<sub>1</sub>!</b></font>");
        return;
    }

    if(ui->spinDilSize->text().toInt(&ok)!=0)
    {
        ui->plainTextEdit->appendHtml("Dilation Element: " + ui->comboDilElement->currentText());
        ui->plainTextEdit->appendHtml("Dilation size = " + ui->spinDilSize->text());
    }

    ui->plainTextEdit->appendHtml("<br/><b>Segmentation Started</b>");
    seg->convergence_criterion = ui->lineEditEps->text().toDouble(&ok);
    seg->maximum_iterations = ui->lineEditN->text().toDouble(&ok);
    seg->start();
}

void MainWindow::onCycleComplete(QString i)
{
    ui->plainTextEdit->appendHtml("<p>" + i + "</p>");
}

void MainWindow::onParametersEstimated(double pe, double h, double alpha, double mu, double l0, double l1)
{
//    ui->plainTextEdit->appendHtml("<p>p<sub>e</sub> = " + QString::number(pe) + "</p>");
//    ui->plainTextEdit->appendHtml("<p>h = " + QString::number(h) + "</p>");
//    ui->plainTextEdit->appendHtml("<p>&alpha; = " + QString::number(alpha) + "</p>");
//    ui->plainTextEdit->appendHtml("<p>&mu; = " + QString::number(mu) + "</p>");
//    ui->plainTextEdit->appendHtml("<p>&lambda;<sub>0</sub> = " + QString::number(l0) + "</p>");
//    ui->plainTextEdit->appendHtml("<p>&lambda;<sub>1</sub> = " + QString::number(l1) + "</p>");
    ui->lineEditMu->setText(QString::number(mu));
    ui->lineEditL0->setText(QString::number(l0));
    ui->lineEditL1->setText(QString::number(l1));
}

void MainWindow::onSegmentationComplete()
{
    ui->plainTextEdit->appendHtml("<p>Complete</p>");
    qSeg = cvMatToQImage(seg->segmented);

    if (qSeg.width()>ui->segLbl->width() && qSeg.width()>=qSeg.height())
    {
        qSeg = qSeg.scaledToWidth(ui->segLbl->width());
    }
    else if (qSeg.height()>ui->segLbl->height() && qSeg.height()>=qSeg.width())
    {
        qSeg = qSeg.scaledToHeight(ui->segLbl->height());
    }

    ui->segLbl->setPixmap(QPixmap::fromImage(qSeg));

    // do classification if gt is given
    if (bGtAvailable)
    {
        ui->plainTextEdit->appendHtml("<br/><b>Classification Efficiency</b>");
        seg->classifyAgainst(ui->gtPath->text());
    }
}

void MainWindow::onClassificationValue(QString s)
{
    ui->plainTextEdit->appendHtml("<p>" + s + "</p>");
}
