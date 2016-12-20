#include "Processing.h"

int Processing::image_count = 0;

Processing::Processing(void) {
    this->input_dir = "";
    this->images = vector<Image*>();
}

Processing::Processing(const string input_dir) {
    this->input_dir = input_dir;
    this->images = vector<Image*>();
}

Processing::~Processing() {
    for (int i = 0; i < images.size(); ++i) {
        delete(images.back());
    }
}


void Processing::read_image(string img_name) {
    Mat m = Mat();
    input_dir.size() == 0 ? m = imread(img_name) : m = imread(input_dir + "/" + img_name);
    image_count++;
    images.push_back(new Image(img_name,image_count,m));
}



