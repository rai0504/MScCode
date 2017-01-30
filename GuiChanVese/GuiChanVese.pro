#-------------------------------------------------
#
# Project created by QtCreator 2016-11-30T13:03:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ..\\GuiChanVese
TEMPLATE = app

RC_FILE = ukznicon.rc

INCLUDEPATH += C:\\OpenCV-3.1.0\\opencv\\build\include

LIBS += -LC:\\OpenCV-3.1.0\\mybuild\\lib\\Debug \
    -lopencv_flann310d \
    -lopencv_highgui310d\
    -lopencv_imgcodecs310d \
    -lopencv_imgproc310d \
    -lopencv_ml310d \
    -lopencv_objdetect310d \
    -lopencv_photo310d \
    -lopencv_shape310d \
    -lopencv_stitching310d \
    -lopencv_superres310d \
    -lopencv_ts310d \
    -lopencv_video310d \
    -lopencv_videoio310d \
    -lopencv_videostab310d \
    -lopencv_calib3d310d \
    -lopencv_core310d \
    -lopencv_features2d310d

SOURCES += main.cpp\
        mainwindow.cpp \
    imageexplorer.cpp \
    segment.cpp \
    runningstatistics.cpp \
    graph.cpp \
    pushrelabel.cpp \
    roc.cpp

HEADERS  += mainwindow.h \
    customdefines.h \
    imageexplorer.h \
    segment.h \
    runningstatistics.h \
    edge.h \
    node.h \
    graph.h \
    pushrelabel.h \
    swapinterface.h \
    roc.h

FORMS    += mainwindow.ui \
    imageexplorer.ui

DISTFILES +=

RESOURCES += \
    rsc.qrc
