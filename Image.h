#pragma once
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

/*
 * A class for storing information about image
 */
class Image {

public:
    std::string name;
    int idx;
    cv::Mat mat;

    Image(const std::string &name, int idx, const cv::Mat &mat) : name(name), idx(idx), mat(mat) {}

    virtual ~Image() {
        mat.release();
    }



};
