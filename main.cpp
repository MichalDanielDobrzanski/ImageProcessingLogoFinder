#include <iostream>
#include "processing.h"

using namespace std;

const string INPUT_DIR = "input_data";

int main() {

    Processing p(INPUT_DIR);
    p.readImage("1.jpg");

    p.showImage(1);


    return 0;
}