#ifndef DEFINES_H
#define DEFINES_H
#define FG  0
#define BG  1

enum eInterConnectivity
{
    FOUR_WAY = 0,
    EIGHT_WAY
};

enum eBrushElement
{
    RECTANGLE=0,
    CIRCLE
};

const cv::String sample_set = "sample_set/";
const cv::String test_set = "test_set/";
const cv::String working_directory = test_set;
const cv::String seed_directory = "seeds/emcanny/seed_1/";
const cv::String output_directory = "output/";

const cv::Scalar fgMarker = cv::Scalar(0, 0, 255);  // red = foreground
const cv::Scalar bgMarker = cv::Scalar(255, 0, 0);  // blue = background
const cv::Scalar eraseMarker = cv::Scalar(0, 0, 0); // remove the seed colours

const int maxBrushSize = 50;

const int number_of_clusters = 2;

const int xDistributionScale = 1;
const int yDistributionScale = 1;

const bool saveBinaryVariableState = false;

const int ESC = 27;

#endif // DEFINES_H
