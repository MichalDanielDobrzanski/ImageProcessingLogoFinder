#include "ImageProcessing.h"

void ImageProcessing::lighten(Mat &mat, int amount) {
    CV_Assert(mat.depth() != sizeof(uchar));
    switch (mat.channels())  {
        case 1:
            for (int i = 0; i < mat.rows; ++i)
                for (int j = 0; j < mat.cols; ++j) {
                    if (mat.at<uchar>(i, j) + amount > 255) {
                        mat.at<uchar>(i, j) = 255;
                    }
                    else if (mat.at<uchar>(i, j) + amount < 0) {
                        mat.at<uchar>(i, j) = 0;
                    }
                    else {
                        mat.at<uchar>(i, j) += amount;
                    }
                }
            break;
        case 3:
            cv::Mat_<cv::Vec3b> _I = mat;
            for (int i = 0; i < mat.rows; ++i)
                for (int j = 0; j < mat.cols; ++j){
                    for (int k = 0; k < mat.channels(); ++k) {
                        if (_I(i, j)[k] + amount > 255) {
                            _I(i, j)[k] = 255;
                        }
                        else if (_I(i, j)[k] + amount < 0){
                            _I(i, j)[k] = 0;
                        }
                        else {
                            _I(i, j)[k] += amount;
                        }
                    }
                }
            break;
    }

}
