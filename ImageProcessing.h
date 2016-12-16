#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

/*
 * A class for storing all processing opeations for images
 */
class ImageProcessing {

public:

    static void lighten(Mat& mat, int amount);

};
