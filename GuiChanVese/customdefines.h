#ifndef CUSTOMDEFINES_H
#define CUSTOMDEFINES_H

#include <QString>
#include <vector>

const QString noImgPath = "../noimage.png";
const QString noGtPath = "../noground.png";
const QString noSegPath = "../nosegmentation.png";

enum eMethod
{
    ElZehiry=0,
    Masaka,
    Ryan,
    Manual,
};

enum eInitialise
{
    Circle=0,
    Otsu,
    KMeans,
    EMGMM,
};

enum eInterConnectivity
{
    FOUR_WAY = 0,
    EIGHT_WAY
};

const double PI = 3.14159265359;

const std::vector<QString> methodNames = {"El-Zehiry et al.", "Masaka et al.", "Proposed", "Manual"};
const std::vector<QString> initNames = {"Centre-Circle", "Otsu", "K-means", "EMGMM"};
const std::vector<QString> muString = {"6502.5", "0.01", "TBC", "Enter val."};
const std::vector<QString> l0String = {"1", "85", "TBC", "Enter val."};
const std::vector<QString> l1String = {"1", "1", "TBC", "Enter val."};
const std::vector<QString> dilationElement = {"Rectangle", "Cross", "Ellipse"};

const double defaultEps = 1e-3;
const int defaultIterations = 30;

const double kp = 0.154494;
const double kh = 0.412737;
//const double length_normalise = 1/(PI/graphConnectivity*2);
const double flowmax = 2*sqrt(2) + 4;
const double e = 1/sqrt(2.0);
//const double dTheta = M_PI/8;

#endif // CUSTOMDEFINES_H
