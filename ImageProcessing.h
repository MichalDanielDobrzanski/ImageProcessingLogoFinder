#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

/*
 * A class for storing all processing opeations for images
 */
class ImageProcessing {

public:

    // add constant value to all of the pixels
    static void lighten(Mat& mat, int amount);

    // change contrast:
    static void contrast(Mat& mat, float mult);

    /* median filtering
     * - specify the width and height of a window by 'w_size' value
     * - pick value from median vector at 'idx'
     */
    static void median_filter(Mat& mat, int w_size, int idx);

};
