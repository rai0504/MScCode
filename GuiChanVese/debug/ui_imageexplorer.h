/********************************************************************************
** Form generated from reading UI file 'imageexplorer.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEEXPLORER_H
#define UI_IMAGEEXPLORER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageExplorer
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QTreeView *imgDir;
    QListView *imgFile;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *imgLbl;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *ImageExplorer)
    {
        if (ImageExplorer->objectName().isEmpty())
            ImageExplorer->setObjectName(QStringLiteral("ImageExplorer"));
        ImageExplorer->resize(539, 441);
        layoutWidget = new QWidget(ImageExplorer);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 520, 194));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        imgDir = new QTreeView(layoutWidget);
        imgDir->setObjectName(QStringLiteral("imgDir"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(imgDir->sizePolicy().hasHeightForWidth());
        imgDir->setSizePolicy(sizePolicy);
        imgDir->setMinimumSize(QSize(350, 0));

        horizontalLayout->addWidget(imgDir);

        imgFile = new QListView(layoutWidget);
        imgFile->setObjectName(QStringLiteral("imgFile"));

        horizontalLayout->addWidget(imgFile);

        layoutWidget1 = new QWidget(ImageExplorer);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 230, 511, 194));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        imgLbl = new QLabel(layoutWidget1);
        imgLbl->setObjectName(QStringLiteral("imgLbl"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(imgLbl->sizePolicy().hasHeightForWidth());
        imgLbl->setSizePolicy(sizePolicy1);
        imgLbl->setMinimumSize(QSize(192, 192));
        imgLbl->setMaximumSize(QSize(192, 16777215));
        imgLbl->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(imgLbl);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pushButton = new QPushButton(layoutWidget1);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(layoutWidget1);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        sizePolicy2.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(pushButton_2);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(ImageExplorer);
        QObject::connect(pushButton_2, SIGNAL(clicked()), ImageExplorer, SLOT(close()));
        QObject::connect(pushButton, SIGNAL(clicked()), ImageExplorer, SLOT(accept()));

        QMetaObject::connectSlotsByName(ImageExplorer);
    } // setupUi

    void retranslateUi(QDialog *ImageExplorer)
    {
        ImageExplorer->setWindowTitle(QApplication::translate("ImageExplorer", "Image Explorer", 0));
        imgLbl->setText(QApplication::translate("ImageExplorer", "TextLabel", 0));
        pushButton->setText(QApplication::translate("ImageExplorer", "OK", 0));
        pushButton_2->setText(QApplication::translate("ImageExplorer", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageExplorer: public Ui_ImageExplorer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEEXPLORER_H
