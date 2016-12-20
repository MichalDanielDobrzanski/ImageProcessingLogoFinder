#include <iostream>
#include "Processing.h"
#include "ImageProcessing.h"

using namespace std;

const string INPUT_DIR = "input_data";

int main() {


    //Processing p(INPUT_DIR);
    for (int i = 1; i < 5; ++i) {
        Mat mat = imread(INPUT_DIR + "/" + to_string(i) + ".jpg");

        ImageProcessing::info(mat);

        ImageProcessing::resize(mat);

        // using LP filters in inefficient - edges are blurry
        //ImageProcessing::filter3(mat,LOW_PASS);

        Mat hs_mat = ImageProcessing::split_to_hs(mat);

        // segmentation - best parameters so far: 0.55, 0.73, 0.49, 1
        ImageProcessing::segment(hs_mat,mat,0.55,0.73,0.49,1);

        ImageProcessing::median_filter(mat,3,5);

        // erosion is not effective as it blurs the text..
        //ImageProcessing::erosion(mat);

        // dilation blurs also
        //ImageProcessing::dilation(mat);

        // opening - box.
        //ImageProcessing::erosion_dilation(mat,true,true);
        //ImageProcessing::erosion_dilation(mat,false,true);
        //imwrite("out_data/" + to_string(i) + "_3box_opening " + ".jpg",mat),

        // opening - plus
        //ImageProcessing::erosion_dilation(mat,true,false);
        //ImageProcessing::erosion_dilation(mat,false,false);
        //imwrite("out_data/" + to_string(i) + "_plus_opening " + ".jpg",mat),

        // closure (dilation -> erosion) - box:
        //ImageProcessing::erosion_dilation(mat,false,true);
        //ImageProcessing::erosion_dilation(mat,true,true);
        //imwrite("out_data/" + to_string(i) + "_3box_closure" + ".jpg",mat),

        // The best so far:
        // closure (dilation -> erosion) - plus:
        ImageProcessing::erosion_dilation(mat,false,false);
        ImageProcessing::erosion_dilation(mat,true,false);
        //imwrite("out_data/" + to_string(i) + "_plus_closure" + ".jpg",mat),


        //imshow(to_string(i),mat);

        cout << endl;
    }

    waitKey(-1);

    //printf( "hue: %6.4lf\n", mat.at<Vec2f>(5,5)[0]);
    //printf( "sat: %6.4lf\n", mat.at<Vec2f>(5,5)[1]);


//    int img = 1;
//    Processing p;
//    p.read_image("input_data/4.jpg");
//    ImageProcessing::info(p[0]);
//    ImageProcessing::resize(p[0]);
////    //ImageProcessing::filter3(p[0],GAUSS);
//    Mat mat = ImageProcessing::split_to_hs(p[0]);
////    //printf( "hue: %6.4lf\n", mat.at<Vec2f>(5,5)[0]);
////    //printf( "sat: %6.4lf\n", mat.at<Vec2f>(5,5)[1]);
//    ImageProcessing::segment(mat,p[0],0.5,0.7,0.5,1);
//    imshow(to_string(img),p[0]);

    //ImageProcessing::show_histogram(mat);


    //ImageProcessing::lighten(p[0],-100);
    //ImageProcessing::contrast(p[0],0.1);

    //ImageProcessing::median_filter(p[0],3,8);

    /*
    vector<Mat> channels = ImageProcessing::split_to_hsv(p[0]);
    string s1("channel[0] - H");
    imshow(s1,channels[0]);
    */

    //ImageProcessing::binary(p[0],100);



    return 0;
}