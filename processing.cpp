#include "processing.h"

int Processing::image_count = 0;

Processing::Processing(const string input_dir) {
    this->input_dir = input_dir;
    this->images = vector<Image*>();
}

void Processing::readImage(string img_name) {
    Mat m = imread(input_dir + "/" + img_name);
    image_count++;
    images.push_back(new Image(img_name,image_count,m));
}

void Processing::showImage(int i) {
    Image* img = images.at((unsigned long)i-1);
    imshow(img->name, img->mat);
    waitKey(-1);
}
