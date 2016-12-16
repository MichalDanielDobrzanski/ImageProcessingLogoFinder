#pragma once
#include <string>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "Image.h"

using namespace std;
using namespace cv;

class Processing {

private:

    string input_dir;
    vector<Image*> images;

public:

    static int image_count;

    Processing(const string input_dir);

    void readImage(string img_name);

    void showImage(int i);

};