#ifndef OPEN_CV_IMAGE_H
#define OPEN_CV_IMAGE_H

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class Image {

public:
    std::string name;
    int idx;
    cv::Mat mat;

    Image(const std::string &name, int idx, const cv::Mat &mat) : name(name), idx(idx), mat(mat) {}
};

#endif //OPEN_CV_IMAGE_H
