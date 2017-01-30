/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_2;
    QLabel *energyLbl;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushImg;
    QLineEdit *imgPath;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushGt;
    QLineEdit *gtPath;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QComboBox *comboN;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QComboBox *comboMethod;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3;
    QComboBox *comboInitial;
    QHBoxLayout *horizontalLayout_7;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout;
    QLabel *label_4;
    QComboBox *comboDilElement;
    QLabel *label_5;
    QSpinBox *spinDilSize;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_4;
    QFormLayout *formLayout_2;
    QLabel *labelMu;
    QLineEdit *lineEditMu;
    QLabel *labelL0;
    QLineEdit *lineEditL0;
    QLabel *labelL1;
    QLineEdit *lineEditL1;
    QGroupBox *groupBox_5;
    QFormLayout *formLayout_3;
    QLabel *labelEps;
    QLineEdit *lineEditEps;
    QLabel *labelN;
    QLineEdit *lineEditN;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushSegment;
    QHBoxLayout *horizontalLayout_6;
    QLabel *imgLbl;
    QLabel *gtLbl;
    QLabel *segLbl;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1010, 520);
        MainWindow->setMinimumSize(QSize(1010, 520));
        MainWindow->setMaximumSize(QSize(1010, 520));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 991, 504));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        energyLbl = new QLabel(layoutWidget);
        energyLbl->setObjectName(QStringLiteral("energyLbl"));
        QFont font;
        font.setPointSize(12);
        energyLbl->setFont(font);
        energyLbl->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(energyLbl);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushImg = new QPushButton(layoutWidget);
        pushImg->setObjectName(QStringLiteral("pushImg"));

        horizontalLayout_3->addWidget(pushImg);

        imgPath = new QLineEdit(layoutWidget);
        imgPath->setObjectName(QStringLiteral("imgPath"));
        imgPath->setReadOnly(true);

        horizontalLayout_3->addWidget(imgPath);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushGt = new QPushButton(layoutWidget);
        pushGt->setObjectName(QStringLiteral("pushGt"));

        horizontalLayout_2->addWidget(pushGt);

        gtPath = new QLineEdit(layoutWidget);
        gtPath->setObjectName(QStringLiteral("gtPath"));
        gtPath->setReadOnly(true);

        horizontalLayout_2->addWidget(gtPath);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_4 = new QHBoxLayout(groupBox);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_4->addWidget(label);

        comboN = new QComboBox(groupBox);
        comboN->setObjectName(QStringLiteral("comboN"));

        horizontalLayout_4->addWidget(comboN);


        horizontalLayout_5->addWidget(groupBox);

        groupBox_2 = new QGroupBox(layoutWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        comboMethod = new QComboBox(groupBox_2);
        comboMethod->setObjectName(QStringLiteral("comboMethod"));

        horizontalLayout->addWidget(comboMethod);

        horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);

        comboInitial = new QComboBox(groupBox_2);
        comboInitial->setObjectName(QStringLiteral("comboInitial"));

        horizontalLayout->addWidget(comboInitial);


        horizontalLayout_5->addWidget(groupBox_2);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setSizeConstraint(QLayout::SetDefaultConstraint);
        groupBox_3 = new QGroupBox(layoutWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        formLayout = new QFormLayout(groupBox_3);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_4);

        comboDilElement = new QComboBox(groupBox_3);
        comboDilElement->setObjectName(QStringLiteral("comboDilElement"));

        formLayout->setWidget(0, QFormLayout::FieldRole, comboDilElement);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);

        spinDilSize = new QSpinBox(groupBox_3);
        spinDilSize->setObjectName(QStringLiteral("spinDilSize"));
        spinDilSize->setWrapping(true);
        spinDilSize->setReadOnly(false);
        spinDilSize->setMaximum(7);

        formLayout->setWidget(1, QFormLayout::FieldRole, spinDilSize);

        verticalSpacer = new QSpacerItem(50, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        formLayout->setItem(2, QFormLayout::FieldRole, verticalSpacer);


        horizontalLayout_7->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(layoutWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        formLayout_2 = new QFormLayout(groupBox_4);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        labelMu = new QLabel(groupBox_4);
        labelMu->setObjectName(QStringLiteral("labelMu"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, labelMu);

        lineEditMu = new QLineEdit(groupBox_4);
        lineEditMu->setObjectName(QStringLiteral("lineEditMu"));
        lineEditMu->setReadOnly(true);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, lineEditMu);

        labelL0 = new QLabel(groupBox_4);
        labelL0->setObjectName(QStringLiteral("labelL0"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, labelL0);

        lineEditL0 = new QLineEdit(groupBox_4);
        lineEditL0->setObjectName(QStringLiteral("lineEditL0"));
        lineEditL0->setReadOnly(true);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, lineEditL0);

        labelL1 = new QLabel(groupBox_4);
        labelL1->setObjectName(QStringLiteral("labelL1"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, labelL1);

        lineEditL1 = new QLineEdit(groupBox_4);
        lineEditL1->setObjectName(QStringLiteral("lineEditL1"));
        lineEditL1->setReadOnly(true);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lineEditL1);


        horizontalLayout_7->addWidget(groupBox_4);

        groupBox_5 = new QGroupBox(layoutWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        formLayout_3 = new QFormLayout(groupBox_5);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        labelEps = new QLabel(groupBox_5);
        labelEps->setObjectName(QStringLiteral("labelEps"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, labelEps);

        lineEditEps = new QLineEdit(groupBox_5);
        lineEditEps->setObjectName(QStringLiteral("lineEditEps"));
        lineEditEps->setReadOnly(false);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, lineEditEps);

        labelN = new QLabel(groupBox_5);
        labelN->setObjectName(QStringLiteral("labelN"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, labelN);

        lineEditN = new QLineEdit(groupBox_5);
        lineEditN->setObjectName(QStringLiteral("lineEditN"));
        lineEditN->setReadOnly(false);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, lineEditN);

        verticalSpacer_2 = new QSpacerItem(139, 25, QSizePolicy::Minimum, QSizePolicy::Fixed);

        formLayout_3->setItem(2, QFormLayout::SpanningRole, verticalSpacer_2);


        horizontalLayout_7->addWidget(groupBox_5);

        pushSegment = new QPushButton(layoutWidget);
        pushSegment->setObjectName(QStringLiteral("pushSegment"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushSegment->sizePolicy().hasHeightForWidth());
        pushSegment->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(pushSegment);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        imgLbl = new QLabel(layoutWidget);
        imgLbl->setObjectName(QStringLiteral("imgLbl"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(imgLbl->sizePolicy().hasHeightForWidth());
        imgLbl->setSizePolicy(sizePolicy1);
        imgLbl->setMinimumSize(QSize(192, 192));
        imgLbl->setMaximumSize(QSize(192, 192));
        imgLbl->setPixmap(QPixmap(QString::fromUtf8(":/images/noground.png")));
        imgLbl->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(imgLbl);

        gtLbl = new QLabel(layoutWidget);
        gtLbl->setObjectName(QStringLiteral("gtLbl"));
        sizePolicy1.setHeightForWidth(gtLbl->sizePolicy().hasHeightForWidth());
        gtLbl->setSizePolicy(sizePolicy1);
        gtLbl->setMinimumSize(QSize(192, 192));
        gtLbl->setMaximumSize(QSize(192, 192));
        gtLbl->setPixmap(QPixmap(QString::fromUtf8(":/images/noground.png")));
        gtLbl->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(gtLbl);

        segLbl = new QLabel(layoutWidget);
        segLbl->setObjectName(QStringLiteral("segLbl"));
        sizePolicy1.setHeightForWidth(segLbl->sizePolicy().hasHeightForWidth());
        segLbl->setSizePolicy(sizePolicy1);
        segLbl->setMinimumSize(QSize(192, 192));
        segLbl->setMaximumSize(QSize(192, 192));
        segLbl->setPixmap(QPixmap(QString::fromUtf8(":/images/nosegmentation.png")));
        segLbl->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(segLbl);


        verticalLayout_2->addLayout(horizontalLayout_6);


        horizontalLayout_8->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        plainTextEdit = new QPlainTextEdit(layoutWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(plainTextEdit->sizePolicy().hasHeightForWidth());
        plainTextEdit->setSizePolicy(sizePolicy2);
        plainTextEdit->setReadOnly(true);

        verticalLayout->addWidget(plainTextEdit);


        horizontalLayout_8->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Chan-Vese Energy Optimisation via Graph Cuts for Image Segmentation", 0));
        energyLbl->setText(QApplication::translate("MainWindow", "Ecv(C,c0,c1) = uL(c) + S(f(x)-c0)2 + S(f(x)-c1)2", 0));
        pushImg->setText(QApplication::translate("MainWindow", "Image", 0));
        pushGt->setText(QApplication::translate("MainWindow", "Ground Truth", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Graph Structure", 0));
        label->setText(QApplication::translate("MainWindow", "Neighbourhood Density", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Parameter Estimation Method", 0));
        label_2->setText(QApplication::translate("MainWindow", "Method", 0));
        label_3->setText(QApplication::translate("MainWindow", "Initialisation", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Dilation", 0));
        label_4->setText(QApplication::translate("MainWindow", "Element", 0));
        label_5->setText(QApplication::translate("MainWindow", "Size", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Parameter Settings", 0));
        labelMu->setText(QApplication::translate("MainWindow", "mu = ", 0));
        labelL0->setText(QApplication::translate("MainWindow", "l0 = ", 0));
        labelL1->setText(QApplication::translate("MainWindow", "l1 = ", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Simulation Settings", 0));
        labelEps->setText(QApplication::translate("MainWindow", "eps =", 0));
        labelN->setText(QApplication::translate("MainWindow", "n = ", 0));
        pushSegment->setText(QApplication::translate("MainWindow", "Segment", 0));
        imgLbl->setText(QString());
        gtLbl->setText(QString());
        segLbl->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
