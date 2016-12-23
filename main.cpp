#include <iostream>
#include "Processing.h"
#include "ImageProcessing.h"
#include "ImageMoments.h"

using namespace std;

const string INPUT_DIR = "input_data/";
const string LOGO_DIR = "logo/";
const string NOISE_DIR = "noise/";

const string LOGO_NAME = "eagle";
const string TEXT_NAME = "orlen";

int main() {

    // HUE-SATURATION segmentation parameters:
    double hue_min = 0.55;
    double hue_max = 0.73;
    double sat_min = 0.49;
    double sat_max = 1.0;

    // minimal field of a segmented object
    int minS = 20;

    // calc moments for logo - 'eagle'
    int num_logos = 6;
    double l1avg = 0.0;
    double l3avg = 0.0;
    double l7avg = 0.0;
    for (int l = 0; l < num_logos; ++l) {
        ImageMoments moms(LOGO_DIR + LOGO_NAME + "_" + to_string(l + 1));
        double l1 =  moms.get_moment(1);
        double l3 =  moms.get_moment(3);
        double l7 =  moms.get_moment(7);
//        cout << to_string(l + 1) << ": mom 1 logo =" << l1 << endl;
//        cout << to_string(l + 1) << ": mom 3 logo =" << l3 << endl;
//        cout << to_string(l + 1) << ": mom 7 logo =" << l7 << endl;
        l1avg += l1;
        l3avg += l3;
        l7avg += l7;
    }
    l1avg /= num_logos;
    l3avg /= num_logos;
    l7avg /= num_logos;
    cout << "mom 1 logo average=" << l1avg << endl;
    cout << "mom 3 logo average=" << l3avg << endl;
    cout << "mom 7 logo average=" << l7avg << endl;


    // calc moments for text - 'orlen'
    ImageMoments moms = ImageMoments(LOGO_DIR + TEXT_NAME);
    double t1 =  moms.get_moment(1);
    double t3 =  moms.get_moment(3);
    double t7 =  moms.get_moment(7);

    // calc mean moments for noise
    double n1avg =  0.0;
    double n3avg =  0.0;
    double n7avg =  0.0;
    for (int k = 1; k < 5; ++k) {
        std::cout << "Calculating noise moments..." << endl;

        Mat mat = imread(NOISE_DIR + to_string(k) + ".jpg");

        ImageProcessing::debug = false;

        ImageProcessing::resize(mat);

        Mat hs_mat = ImageProcessing::split_to_hs(mat);

        ImageProcessing::segment(hs_mat,mat,hue_min,hue_max,sat_min,sat_max);

        ImageProcessing::median_filter(mat,3,5);

        ImageProcessing::erosion_dilation(mat,false,false);
        ImageProcessing::erosion_dilation(mat,true,false);

        double n1 =  0.0;
        double n3 =  0.0;
        double n7 =  0.0;

        vector<Bbox> els = ImageProcessing::get_elements(mat, minS, false);
        //imshow("1",els[0].box);
        //waitKey(-1);
        for (int i = 0; i < els.size(); ++i) {
            ImageMoments n_moms(els[i].box);
            double m1 =  n_moms.get_moment(1);
            double m3 =  n_moms.get_moment(3);
            double m7 =  n_moms.get_moment(7);
            n1 += m1;
            n3 += m3;
            n7 += m7;
        }
        n1 /= els.size();
        n3 /= els.size();
        n7 /= els.size();
        n1avg += n1;
        n3avg += n3;
        n7avg += n7;
    }
    n1avg /= 4;
    n3avg /= 4;
    n3avg /= 4;
    cout << "mom 1 noise average=" << n1avg << endl;
    cout << "mom 3 noise average=" << n3avg << endl;
    cout << "mom 7 noise average=" << n7avg << endl;


    //Processing p(INPUT_DIR);
    for (int i = 1; i < 5; ++i) {
        Mat mat = imread(INPUT_DIR + to_string(i) + ".jpg");

        // get standard information aobut an image...
        ImageProcessing::debug = true;
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
        ImageProcessing::segment(hs_mat,mat,hue_min,hue_max,sat_min,sat_max);
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
        vector<Bbox> els = ImageProcessing::get_elements(mat, minS,false);

        //imwrite("out_data/segmented/" + to_string(i) + "_segm_" + to_string(els[idx].S)
        //        + "_idx_"+ to_string(idx) + ".jpg",els[idx].box),
        //imshow(to_string(0),els[0].box);
        //waitKey(-1);

        //imshow(to_string(i),mat);

        int num = 2;
        for (int j = 0; j < num; ++j) {
            ImageMoments im_moms(els[j].box);
            double m1 =  im_moms.get_moment(1);
            double m3 =  im_moms.get_moment(3);
            double m7 =  im_moms.get_moment(7);

            double is_noise = im_moms.get_classification(m1,m3,m7,n1avg,n3avg,n7avg);
            double is_logo  = im_moms.get_classification(m1,m3,m7,l1avg,l3avg,l7avg);
            double is_text  = im_moms.get_classification(m1,m3,m7,t1,t3,t7);

            double logo = is_logo / (is_logo + is_noise) * 100;
            cout << "Segment: " << j << ". There is a logo. " << logo << "% sure." << endl;
            double text = is_text / (is_text + is_noise) * 100;
            cout << "Segment: " << j << ". There is a text. " << text << "% sure." << endl;
        }
        els.clear();
        cout << endl;
    }
    //waitKey(-1);

    return 0;
}