#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

/*
 * A class for storing all processing operations for images
 */
class ImageProcessing {

public:
    const static int IMAGE_SIZE = 1000;

    // resize
    static void resize(Mat& mat);

    // add constant value to all pixels
    static void lighten(Mat& mat, int amount);

    // change contrast:
    static void contrast(Mat& mat, float mult);

    /* median filtering
     * - specify the width and height of a window by 'w_size' value
     * - pick value from median vector at 'idx'
     */
    static void median_filter(Mat& mat, int w_size, int idx);

    static vector<Mat> split_to_hsv(Mat& mat);

    // binarization over specific threshold
    static void binary(Mat &mat, int threshold);

    // utilities
    static void info(Mat& mat);
};
