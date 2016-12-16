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
            Mat_<Vec3b> mat3 = mat;
            for (int i = 0; i < mat.rows; ++i)
                for (int j = 0; j < mat.cols; ++j){
                    for (int k = 0; k < mat.channels(); ++k) {
                        if (mat3(i, j)[k] + amount > 255) {
                            mat3(i, j)[k] = 255;
                        }
                        else if (mat3(i, j)[k] + amount < 0){
                            mat3(i, j)[k] = 0;
                        }
                        else {
                            mat3(i, j)[k] += amount;
                        }
                    }
                }
            break;
    }

}

void ImageProcessing::contrast(Mat &mat, float mult) {
    CV_Assert(mat.depth() != sizeof(uchar));
    switch (mat.channels())  {
        case 1:
            for (int i = 0; i < mat.rows; ++i)
                for (int j = 0; j < mat.cols; ++j) {
                    if (mat.at<uchar>(i, j) * mult > 255) {
                        mat.at<uchar>(i, j) = 255;
                    }
                    else if (mat.at<uchar>(i, j) * mult < 0) {
                        mat.at<uchar>(i, j) = 0;
                    }
                    else {
                        mat.at<uchar>(i, j) *= mult;
                    }
                }
            break;
        case 3:
            Mat_<Vec3b> mat3 = mat;
            for (int i = 0; i < mat.rows; ++i)
                for (int j = 0; j < mat.cols; ++j){
                    for (int k = 0; k < mat.channels(); ++k) {
                        if (mat3(i, j)[k] * mult > 255) {
                            mat3(i, j)[k] = 255;
                        }
                        else if (mat3(i, j)[k] * mult < 0) {
                            mat3(i, j)[k] = 0;
                        }
                        else {
                            mat3(i, j)[k] *= mult;
                        }
                    }
                }
            break;
    }

}

// utilities for median filtering
struct vals {
    int mean;
    int R;
    int G;
    int B;
};

bool sorting(vals i, vals j) { return (i.mean<j.mean); }


void ImageProcessing::median_filter(Mat &mat, int w_size, int idx) {
    if (w_size > 1 && w_size < mat.cols && w_size < mat.rows &&
            idx > 0 && idx < w_size * w_size) {

        CV_Assert(mat.depth() != sizeof(uchar));
        switch (mat.channels()) {
            case 3:
                Mat_<Vec3b> mat3 = mat;
                Mat_<Vec3b> mat3res = Mat(mat.cols,mat.rows,CV_8UC3);

                int offset = w_size / 2;

                for (int i = 0; i < mat.rows; ++i) {
                    for (int j = 0; j < mat.cols; ++j) {

                        vector<vals> med((unsigned long) (w_size * w_size));

                        int el = 0;
                        // iterate over pixels around me:
                        for (int k = 0; k < w_size; k++) {
                            for (int l = 0; l < w_size; l++) {

                                int c_row = i - offset + l;
                                int c_col = j - offset + k;

                                // in order not to go out of the borders
                                if ((c_row >= 0 && c_row < mat.rows) &&
                                        (c_col >= 0  && c_col < mat.cols)) {

                                    // mean:
                                    int pixVal = 0;
                                    for (int ch = 0; ch < mat.channels(); ch++) {
                                        pixVal += mat3(c_row, c_col)[ch];
                                    }
                                    med[el].mean = pixVal / 3;

                                    med[el].R = mat3(c_row, c_col)[0];
                                    med[el].G = mat3(c_row, c_col)[1];
                                    med[el].B = mat3(c_row, c_col)[2];

                                    el++;
                                }
                            }
                        }

                        // remove unused elements in a median vector
                        int unused = (int) (med.size() - el);
                        for (int n = unused; n > 0 ; --n) {
                            med.pop_back();
                        }

                        // sort
                        std::sort(std::begin(med), std::end(med), sorting);

                        // compensate for unused pixels
                        mat3res(i, j)[0] = (uchar) med[idx - unused].R;
                        mat3res(i, j)[1] = (uchar) med[idx - unused].G;
                        mat3res(i, j)[2] = (uchar) med[idx - unused].B;
                    }
                }
                mat = mat3res;
                break;
        }
    }
}