#include <iostream>
#include "Processing.h"
#include "ImageProcessing.h"

using namespace std;

const string INPUT_DIR = "input_data";

int main() {

    Processing p(INPUT_DIR);

    for (int i = 0; i < 4; ++i) {
        p.readImage(to_string(i + 1) + ".jpg");
    }
    cout << "Images count: " << p.image_count << endl;

    //p.showImage(3);

    cout << "channels: " << p[0].channels() << endl;

    ImageProcessing::lighten(p[0],-100);

    p.showImage(0);

    return 0;
}