#include "segment.h"
#include <QMutex>
#include <QDebug>
#include "roc.h"

Segment::Segment(QObject *parent) : QThread(parent)
{

}

void updateArithemeticMeans(cv::Mat* img, cv::Mat *randomVariable, double *c0, double *c1)
{
    RunningStatistics r0, r1;
    for(int r=0; r<randomVariable->rows; r++)
        for(int c=0; c<randomVariable->cols; c++)
        {
            if (randomVariable->at<uchar>(r,c)==1) r1.push(img->at<float>(r,c));
            else r0.push(img->at<float>(r,c));
        }
    *c0=r0.mean();
    *c1=r1.mean();
}

void estimateParameters(double c0, double c1, double *pe, double *h, double *mu, double *l1, double *alpha)
{
    *mu=1;
    *pe = c0+kp*(c1-c0);
    *h = c0+kh*(c1-c0);
    *alpha = pow((c1-c0)/((*pe)-c0)-1,2);
    *l1 = ((*mu)*flowmax)/((*alpha)*pow((*h)-c0,2)-pow((*h)-c1,2));
}

double eP(double xp, double u, double l0, double l1, double c0, double c1)
{
    if (xp==0) return l0*pow(u-c0,2);
    else return l1*pow(u-c1,2);
}

void setTWeights(Graph *g, cv::Mat *image, double  l0, double l1, double c0, double c1)
{
    // Dense Data Energy Connections
    for (int r=0; r<g->r; r++)
        for (int c=0; c<g->c; c++)
        {
            // add an edge from the source, S
            g->addEdge(g->s, g->toIndex(r, c), eP(1, image->at<float>(r,c), l0, l1, c0, c1));
            // add an edge to the sink, T
            g->addEdge(g->toIndex(r, c), g->t, eP(0, image->at<float>(r,c), l0, l1, c0, c1));
        }
}

void Segment::setNWeights(Graph *g, eInterConnectivity connectivity, double mu)
{
    /*
     *  Epq = | Epq(0,0)    Epq(0,1) | = | A    B | = A|  0      0  | + | 0    D-C | + | 0    B+C-A-D |
     *        | Epq(1,0)    Epq(1,1) |   | C    D |    | C-A    C-A |   | 0    D-C |   | 0       0    |
     *
     *  but A = D = 0 : B+C-A-D = B+C = Epq(0,1) + Epq(1,0) = Wpq + Wpq = 2.Wpq
     */
    for (int r=0; r<g->r; r++)
        for (int c=0; c<g->c; c++)
        {
            if (connectivity==eInterConnectivity::EIGHT_WAY)
            {
                     if (r>0)
                     {
                                         g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), 2*mu*dTheta);    // UP
                         if (c<g->c-1)   g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), 2*mu*dTheta*e);  // UP-RIGHT
                         if (c>0)        g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), 2*mu*dTheta*e);  // UP-LEFT
                     }
                     if (c<g->c-1)       g->addEdge(g->toIndex(r, c), g->toIndex(r, c+1), 2*mu*dTheta);    // RIGHT
                     if (r<g->r-1)
                     {
                                         g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), 2*mu*dTheta);    // BOTTOM
                         if (c<g->c-1)   g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), 2*mu*dTheta*e);  // BOTTOM-RIGHT
                         if (c>0)        g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), 2*mu*dTheta*e);  // BOTTOM-LEFT
                     }
                     if (c>0)            g->addEdge(g->toIndex(r, c), g->toIndex(r, c-1), 2*mu*dTheta);    // LEFT
            }
            else
            {
                    /*FOUR_WAY*/
                     if (r>0)           g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), 2*mu*dTheta);  // UP
                     if (c<g->c-1)      g->addEdge(g->toIndex(r, c), g->toIndex(r, c+1), 2*mu*dTheta);  // RIGHT
                     if (r<g->r-1)      g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), 2*mu*dTheta);  // BOTTOM
                     if (c>0)           g->addEdge(g->toIndex(r, c), g->toIndex(r, c-1), 2*mu*dTheta);  // LEFT
            }

        }
}

void updateBinaryVariables(Graph *g, cv::Mat *segmentationMask)
{
    for(int r=0; r<g->r; r++)
        for(int c=0; c<g->c; c++)
        {
            if (g->node[g->toIndex(r, c)].distance>=g->r*g->c+2) segmentationMask->at<uchar>(r, c) = 0;
            else segmentationMask->at<uchar>(r, c) = 1;
        }
}

void getMorphContour(cv::Mat* binaryVariable, cv::Mat* contour)
{
    int apertureSize = 3;
    cv::Mat temp;
    cv::blur(*binaryVariable, temp, cv::Size(apertureSize,apertureSize));
    int morph_elem = 2; // 0- rect, 1-cross, 2-ellipse
    int morph_size = 1; // outline width
    cv::Mat element = cv::getStructuringElement( morph_elem, cv::Size( 2*morph_size + 1, 2*morph_size+1 ), cv::Point(morph_size, morph_size));
    cv::morphologyEx(temp, *contour, CV_MOP_GRADIENT, element);
}

void getCannyContour(cv::Mat* binaryVariable, cv::Mat* contour)
{
    cv::Mat temp = *binaryVariable*255;
    cv::Canny(temp,*contour,50, 200);
}

void convertTo3ChannelGrayScale(cv::Mat* img, cv::Mat* output)
{
    cv::Mat channel[3];
    img->copyTo(channel[0]);
    img->copyTo(channel[1]);
    img->copyTo(channel[2]);
    cv::merge(channel, 3, *output);
}

void drawContourOnImage(cv::Mat* img, cv::Mat* contour, cv::Mat* output)
{
    cv::Mat temp1, splitchannel[3];
    convertTo3ChannelGrayScale(img, &temp1);
    cv::split(temp1, splitchannel);

    cv::Mat inverseContour = cv::Mat::ones(img->rows, img->cols, CV_32FC1);
    cv::Mat tempContour;
    contour->convertTo(tempContour,CV_32FC1);
    inverseContour = inverseContour - tempContour;
    splitchannel[0] = splitchannel[0].mul(inverseContour);
    splitchannel[0].copyTo(splitchannel[1]);
    splitchannel[0].copyTo(splitchannel[2]);

    splitchannel[2] = splitchannel[2]+tempContour;

    cv::merge(splitchannel, 3, *output);
}

void Segment::drawMaskOnImage(cv::Mat* img, cv::Mat* mask)
{
    cv::Mat temp1, splitchannel[3];
    convertTo3ChannelGrayScale(img, &temp1);
    cv::split(temp1, splitchannel);

    if(method==methodNames[2])
    {
        cv::Mat tempMask;
        mask->convertTo(tempMask,CV_32FC1);
        splitchannel[2] = splitchannel[2]+tempMask;

        cv::merge(splitchannel, 3, segmented);
        segmented *= 255;
    }
    else
    {
        cv::Mat tempMask;
        mask->convertTo(tempMask,CV_32FC1);
        splitchannel[2] = splitchannel[2]+255*tempMask;

        cv::merge(splitchannel, 3, segmented);
    }


    segmented.convertTo(segmented,CV_8UC3);
}

void Segment::initialise()
{
    length_normalise = 1/(PI/connectivity*2);
    dTheta = PI/connectivity;

    rawImage.release();
    binaryVariables.release();
    segmented.release();

    rawImage = cv::imread(sRawImage.toLocal8Bit().constData(), CV_LOAD_IMAGE_GRAYSCALE);
    if (rawImage.empty())
    {
        qDebug() << "Error reading image..." << endl << "exiting..." << endl;
        return;
    }

    if (algo.compare(initNames[1]))
    {
        // OTSU binarization
        binaryVariables = cv::Mat::zeros(rawImage.rows, rawImage.cols, CV_8UC1);
        cv::threshold(rawImage, binaryVariables, 0, 1, CV_THRESH_BINARY+CV_THRESH_OTSU);
    }
    else if (algo.compare(initNames[2]))
    {
        // K-MEANS (k=2)
        binaryVariables = cv::Mat::zeros(rawImage.rows, rawImage.cols, CV_8UC1);
        cv::Mat inputData;
        rawImage.convertTo(inputData,CV_32FC1);
        inputData = inputData.reshape(1,inputData.rows*inputData.cols);
        cv::Mat centers;
        cv::kmeans(inputData,2,binaryVariables, cv::TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 0.1),3,cv::KMEANS_RANDOM_CENTERS,centers);
        binaryVariables = binaryVariables.reshape(1,rawImage.rows);
        binaryVariables.convertTo(binaryVariables,CV_8UC1);
    }
    else if (algo.compare(initNames[3]))
    {
        // EM-GMM
        binaryVariables = cv::Mat::zeros(rawImage.rows, rawImage.cols, CV_8UC1);
        cv::Ptr<cv::ml::EM> emGMM = cv::ml::EM::create();
        emGMM->setClustersNumber(2);
        emGMM->setCovarianceMatrixType(cv::ml::EM::COV_MAT_DEFAULT);
        emGMM->setTermCriteria(cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 300, 0.1));
        emGMM->clear();

        cv::Mat inputDataE;
        rawImage.convertTo(inputDataE,CV_32FC1);
        inputDataE = inputDataE.reshape(1,rawImage.rows*rawImage.cols);
        emGMM->trainEM(inputDataE, cv::noArray(), binaryVariables, cv::noArray());
        binaryVariables = binaryVariables.reshape(1,rawImage.rows);
        binaryVariables.convertTo(binaryVariables,CV_8UC1);
    }

    /* Normalise for further processing */
    rawImage.convertTo(rawImage,CV_32FC1);
    rawImage = rawImage/255;

    if (dilate_size!=0)
    {
        int dilation_type;
        if( dilate_element == 0 ){ dilation_type = cv::MORPH_RECT; }
        else if( dilate_element == 1 ){ dilation_type = cv::MORPH_CROSS; }
        else if( dilate_element == 2) { dilation_type = cv::MORPH_ELLIPSE; }
        cv::Mat element = cv::getStructuringElement( dilation_type,
                                                     cv::Size( 2*dilate_size + 1, 2*dilate_size+1 ),
                                                     cv::Point( dilate_size, dilate_size ) );
        cv::dilate(binaryVariables,binaryVariables,element);
        updateArithemeticMeans(&rawImage, &binaryVariables, &c0, &c1);
    }

    updateArithemeticMeans(&rawImage, &binaryVariables, &c0, &c1);
    // order means in ascending order
    if (c0>c1)
    {
        binaryVariables = cv::Mat::ones(rawImage.rows,rawImage.cols,CV_8UC1)-binaryVariables; // this is to ensure the fg is labelled as one
        double temp = c1;
        c1 = c0;
        c0 = temp;
    }

    estimateParameters(c0,c1,&pe,&h,&mu,&l1,&alpha);
    mu *= length_normalise;
    l0 = alpha*l1;

    emit parametersEstimated(pe, h, alpha, mu, l0, l1);
}

void Segment::unproposedInitialise()
{
    length_normalise = 1/(PI/connectivity*2);
    dTheta = PI/connectivity;

    rawImage.release();
    binaryVariables.release();
    segmented.release();

    rawImage = cv::imread(sRawImage.toLocal8Bit().constData(), CV_LOAD_IMAGE_GRAYSCALE);
    if (rawImage.empty())
    {
        qDebug() << "Error reading image..." << endl << "exiting..." << endl;
        return;
    }

    binaryVariables = cv::Mat::zeros(rawImage.rows, rawImage.cols, CV_8UC1);
    if (algo.compare(initNames[0]))
    {
        cv::circle(binaryVariables, cv::Point(rawImage.rows/2, rawImage.cols/2), min(rawImage.rows, rawImage.cols)/2-10, cv::Scalar(1), -1, 8, 0);
    }
    else if (algo.compare(initNames[1]))
    {
        // OTSU binarization

        cv::threshold(rawImage, binaryVariables, 0, 1, CV_THRESH_BINARY+CV_THRESH_OTSU);
    }
    else if (algo.compare(initNames[2]))
    {
        // K-MEANS (k=2)
        cv::Mat inputData;
        rawImage.convertTo(inputData,CV_32FC1);
        inputData = inputData.reshape(1,inputData.rows*inputData.cols);
        cv::Mat centers;
        cv::kmeans(inputData,2,binaryVariables, cv::TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 0.1),3,cv::KMEANS_RANDOM_CENTERS,centers);
        binaryVariables = binaryVariables.reshape(1,rawImage.rows);
        binaryVariables.convertTo(binaryVariables,CV_8UC1);
    }
    else if (algo.compare(initNames[3]))
    {
        // EM-GMM
        cv::Ptr<cv::ml::EM> emGMM = cv::ml::EM::create();
        emGMM->setClustersNumber(2);
        emGMM->setCovarianceMatrixType(cv::ml::EM::COV_MAT_DEFAULT);
        emGMM->setTermCriteria(cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 300, 0.1));
        emGMM->clear();

        cv::Mat inputDataE;
        rawImage.convertTo(inputDataE,CV_32FC1);
        inputDataE = inputDataE.reshape(1,rawImage.rows*rawImage.cols);
        emGMM->trainEM(inputDataE, cv::noArray(), binaryVariables, cv::noArray());
        binaryVariables = binaryVariables.reshape(1,rawImage.rows);
        binaryVariables.convertTo(binaryVariables,CV_8UC1);
    }

    /* Normalise for further processing */
    rawImage.convertTo(rawImage,CV_32FC1);

    if (dilate_size!=0)
    {
        int dilation_type;
        if( dilate_element == 0 ){ dilation_type = cv::MORPH_RECT; }
        else if( dilate_element == 1 ){ dilation_type = cv::MORPH_CROSS; }
        else if( dilate_element == 2) { dilation_type = cv::MORPH_ELLIPSE; }
        cv::Mat element = cv::getStructuringElement( dilation_type,
                                                     cv::Size( 2*dilate_size + 1, 2*dilate_size+1 ),
                                                     cv::Point( dilate_size, dilate_size ) );
        cv::dilate(binaryVariables,binaryVariables,element);
        updateArithemeticMeans(&rawImage, &binaryVariables, &c0, &c1);
    }

    updateArithemeticMeans(&rawImage, &binaryVariables, &c0, &c1);
    // order means in ascending order
    if (c0>c1)
    {
        binaryVariables = cv::Mat::ones(rawImage.rows,rawImage.cols,CV_8UC1)-binaryVariables; // this is to ensure the fg is labelled as one
        double temp = c1;
        c1 = c0;
        c0 = temp;
    }

}

void Segment::run()
{
    int iteration = 0;
    double mean_difference = c0+c1;
    while (mean_difference>convergence_criterion && iteration<maximum_iterations)
    {
        // construct the graph
        g = new Graph(rawImage.rows, rawImage.cols);
        setTWeights(g, &rawImage, l0, l1, c0, c1);
        setNWeights(g, connectivity, mu);

        // minimise the graph and place the mask on the random binary variables image
        PushRelabel pr(g);
        pr.calculateMaxFlow();
        updateBinaryVariables(g, &binaryVariables);

        // update means (using random binary variables image)
        double old_c0=c0, old_c1=c1;
        updateArithemeticMeans(&rawImage, &binaryVariables, &c0, &c1);
        mean_difference = abs(old_c0-c0) + abs(old_c1-c1);
        iteration++;
        QString s;
        s = "Iteration " + QString::number(iteration) + ": c<sub>0</sub> = " + QString::number(c0) + ", c<sub>1</sub> = "
                + QString::number(c1) + ", &Delta; = " + QString::number(mean_difference);
        emit cycleComplete(s);
    } // while

    drawMaskOnImage(&rawImage, &binaryVariables);

    // this is to ensure the higher pixels are labelled as one
    updateArithemeticMeans(&rawImage, &binaryVariables, &c0, &c1);
    if (c0>c1)
    {
        binaryVariables = cv::Mat::ones(rawImage.rows,rawImage.cols,CV_8UC1)-binaryVariables;
        double temp = c1;
        c1 = c0;
        c0 = temp;
    }

    // send image over
    QString s;
    s = "Final: c<sub>0</sub> = " + QString::number(c0) + ", c<sub>1</sub> = " + QString::number(c1);
    emit cycleComplete(s);

    emit segmentationComplete();

    //cout <<"\n" << endl;
    //myfile << "Final: c0 = " << c0 << "\nFinal: c1 = " << c1 << "\n\n";
    //rawImage.release();
    //binaryVariables.release();
}

void Segment::classifyAgainst(QString gtPath)
{
    gtImage = cv::imread(gtPath.toLocal8Bit().constData(), CV_LOAD_IMAGE_GRAYSCALE);
    if (gtImage.empty())
    {
        qDebug() << "Error reading ground truth image..." << endl << "exiting..." << endl;
        return;
    }

    ROC R(&binaryVariables,&gtImage);
    emit classificationValue("<p>TP = " + QString::number(R.tp()) + "</p>");
    emit classificationValue("<p>TN = " + QString::number(R.tn()) + "</p>");
    emit classificationValue("<p>FP = " + QString::number(R.fp()) + "</p>");
    emit classificationValue("<p>FN = " + QString::number(R.fn()) + "</p>");
    emit classificationValue("<p>Population = " + QString::number(R.population()) + "</p><br/>");

    emit classificationValue("<p>Sensitivity = " + QString::number(R.sensitivity()) + "</p>");
    emit classificationValue("<p>Specificity = " + QString::number(R.specificity()) + "</p>");
    emit classificationValue("<p>Precision = " + QString::number(R.precision()) + "</p>");
    emit classificationValue("<p>Recall = " + QString::number(R.recall()) + "</p><br/>");

    emit classificationValue("<p>NPV = " + QString::number(R.npv()) + "</p>");
    emit classificationValue("<p>FPR = " + QString::number(R.fpr()) + "</p>");
    emit classificationValue("<p>FDR = " + QString::number(R.fdr()) + "</p>");
    emit classificationValue("<p>Miss Rate = " + QString::number(R.missRate()) + "</p><br/>");

    emit classificationValue("<p>Accuracy = " + QString::number(R.accuracy()) + "</p>");
    emit classificationValue("<p>F1 = " + QString::number(R.f1Score()) + "</p>");
    emit classificationValue("<p>F(2) = " + QString::number(R.fBetaScore(2)) + "</p>");
    emit classificationValue("<p>F(0.5) = " + QString::number(R.fBetaScore(0.5)) + "</p>");
    emit classificationValue("<p>MCC = " + QString::number(R.mcc()) + "</p>");
    emit classificationValue("<p>Informedness = " + QString::number(R.informedness()) + "</p>");
    emit classificationValue("<p>Markedness = " + QString::number(R.markedness()) + "</p><br/>");
}
