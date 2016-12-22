#include <iostream>
#include "Processing.h"
#include "ImageProcessing.h"

using namespace std;

const string INPUT_DIR = "input_data";

int main() {


    //Processing p(INPUT_DIR);
    for (int i = 1; i < 5; ++i) {
        Mat mat = imread(INPUT_DIR + "/" + to_string(i) + ".jpg");

        // get standard information aobut an image...
        ImageProcessing::info(mat);

        // resize image if larger than 1000 x 1000
        ImageProcessing::resize(mat);

        // using LP filters in inefficient - edges become blurry
        //ImageProcessing::filter3(mat,LOW_PASS);

        // convert RGB to Hue and Saturation channel
        Mat hs_mat = ImageProcessing::split_to_hs(mat);
//        vector<Mat> channels;
//        split(hs_mat, channels);
//        imwrite("out_data/hs/" + to_string(i) + "_split_to_hs_hue" + ".jpg",channels[0]),
//        imwrite("out_data/hs/" + to_string(i) + "_split_to_hs_sat" + ".jpg",channels[1]),

        // segmentation of red color by adjusting min and max thresholds for Hue and Saturation.
        // Best parameters so far: 0.55, 0.73, 0.49, 1
        ImageProcessing::segment(hs_mat,mat,0.55,0.73,0.49,1);
        //imwrite("out_data/segmentation/" + to_string(i) + "_segment_0_55_0_73_0_49_1_0" + ".jpg",mat),

        // median filtering to reduce noise
        ImageProcessing::median_filter(mat,3,5);
        //imwrite("out_data/median/" + to_string(i) + "_median_3_5" + ".jpg",mat),

        // single erosion is not effective as it blurs the text..
        //ImageProcessing::erosion(mat);

        // single dilation blurs also
        //ImageProcessing::dilation(mat);

        // opening - box.
        //ImageProcessing::erosion_dilation(mat,true,true);
        //ImageProcessing::erosion_dilation(mat,false,true);
        //imwrite("out_data/open_closure" + to_string(i) + "_3box_opening " + ".jpg",mat),

        // opening - plus
        //ImageProcessing::erosion_dilation(mat,true,false);
        //ImageProcessing::erosion_dilation(mat,false,false);
        //imwrite("out_data/open_closure" + to_string(i) + "_plus_opening " + ".jpg",mat),

        // closure (dilation -> erosion) - box:
        //ImageProcessing::erosion_dilation(mat,false,true);
        //ImageProcessing::erosion_dilation(mat,true,true);
        //imwrite("out_data/open_closure" + to_string(i) + "_3box_closure" + ".jpg",mat),

        // The best one so far:
        // closure (dilation -> erosion) - plus:
        ImageProcessing::erosion_dilation(mat,false,false);
        ImageProcessing::erosion_dilation(mat,true,false);
        //imwrite("out_data/open_closure" + to_string(i) + "_plus_closure" + ".jpg",mat),


        // get sorted list of objects in a picture
        int minS = 20; // minimal field
        vector<Bbox> els = ImageProcessing::get_elements(mat, minS,false);
        //int idx = 1;
        //imwrite("out_data/segmented/" + to_string(i) + "_segm_" + to_string(els[idx].S)
        //        + "_idx_"+ to_string(idx) + ".jpg",els[idx].box),

        //waitKey(-1);

        //imshow(to_string(i),mat);
        els.clear();
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