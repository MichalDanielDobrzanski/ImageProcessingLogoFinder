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


struct leastMoms {
    double ml; // logo parameters
    int lx;
    int ly;
    int lwidth;
    int lheight;

    double mt; // text parameters
    int tx;
    int ty;
    int twidth;
    int theight;
};

struct LogoMoments {

	double l1avg;
    double l3avg;
    double l7avg;

	double l1avg2;
    double l3avg2;
    double l7avg2;

    double t1;
    double t3;
    double t7;

    double t12;
    double t32;
    double t72;

};

leastMoms process_bounding_box(Bbox& bbox, LogoMoments ms) {
    int width  = bbox.x_max - bbox.x_min;
    int height = bbox.y_max - bbox.y_min;

    int w_dim = 0; // window dimension
    double min_window = 0.3;
    bool dir = false; // 0 = bottom, 1 = right
    if (width > height) {
    	w_dim = height;
    	dir = true;
    	cout << "Went right. " << endl;
    	//cout << "w_dim=" << height << " goin right." << endl;
    } else {
    	w_dim = width;
    	//cout << "w_dim=" << width << " goin bottom." << endl;
    	cout << "Went bottom. " << endl;
    }

    int minimum_window = (int)(min_window * w_dim);  // minimum size of a window.
    cout << "Min window: " << minimum_window << endl;


    // define starting moments;
    leastMoms lms;
    lms.ml = 1;
    lms.mt = 1;

    int curr_x = 0;
    int curr_y = 0;
    
    int dist_to_go = 0;

    if (dir)
    	dist_to_go = width - w_dim;
    else
    	dist_to_go = height - w_dim;


    int step = 0;
    int w_dim_x = w_dim;
    int w_dim_y = w_dim; 
    while (w_dim_x > minimum_window && w_dim_y > minimum_window) { // decrease window size
    	while (step < dist_to_go) {
    	    Mat curr_mat = bbox.box(Rect(curr_x,curr_y,w_dim_x,w_dim_y));

            //imshow("sdfd",curr_mat);
            //waitKey(-1);

        	ImageMoments im_moms(curr_mat);
        	double m1 = im_moms.get_moment(1);
            double m3 = im_moms.get_moment(3);
            double m7 = im_moms.get_moment(7);

            //cout << "going right: m1=" << m1 << " m3=" << m3 << " m7=" << m7 << endl;
            double is_logo  = im_moms.get_classification(m1,m3,m7,ms.l1avg,ms.l3avg,ms.l7avg);
            double is_logo2  = im_moms.get_classification(m1,m3,m7,ms.l1avg2,ms.l3avg2,ms.l7avg2);
            
            double is_logo_min = is_logo > is_logo2 ? is_logo2 : is_logo;
            if (is_logo_min < lms.ml) {
            	lms.ml = is_logo_min;
            	lms.lx = bbox.x_min + curr_x;
            	lms.ly = bbox.y_min + curr_y;
            	lms.lwidth = w_dim_x;
            	lms.lheight = w_dim_y;
            }

            double is_text  = im_moms.get_classification(m1,m3,m7,ms.t1,ms.t3,ms.t7);
            double is_text2  = im_moms.get_classification(m1,m3,m7,ms.t12,ms.t32,ms.t72);

            double is_text_min = is_text > is_text2 ? is_text2 : is_text;
            if (is_text_min < lms.mt) {
            	lms.mt = is_text_min;
            	lms.tx = bbox.x_min + curr_x;
            	lms.ty = bbox.y_min + curr_y;
            	lms.twidth = w_dim_x;
            	lms.theight = w_dim_y;
            }

            if (dir)
            	curr_x++;
            else
            	curr_y++;
    
            step++;
        } 
        if (dir) {
            w_dim_x--;
            dist_to_go = width - w_dim_x - minimum_window;
        } else {
            w_dim_y--;
            dist_to_go = height - w_dim_y - minimum_window;
        }
        step = 0;
        curr_x = 0;
        curr_y = 0;
    }

    //cout << "least dist object-logo: " << lms.ml << " least dist object-text:" << lms.mt << endl;
    return lms;
}


int main() {

    // HUE-SATURATION segmentation parameters:
    double hue_min = 0.55;
    double hue_max = 0.73;
    double sat_min = 0.49;
    double sat_max = 1.0;

    // minimal field of a segmented object
    int minS = 20;

    // calc moments for logo - 'eagle'
    LogoMoments moments;
    int num_logos = 2; // do not include at two pones
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
    moments.l1avg = l1avg;
    moments.l3avg = l3avg;
    moments.l7avg = l7avg;

    // calc moments for logo - 'eagle'
    double l1avg2 = 0.0;
    double l3avg2 = 0.0;
    double l7avg2 = 0.0;
    for (int l = 2; l < 4; ++l) {
        ImageMoments moms(LOGO_DIR + LOGO_NAME + "_" + to_string(l + 1));
        double l12 =  moms.get_moment(1);
        double l32 =  moms.get_moment(3);
        double l72 =  moms.get_moment(7);
//        cout << to_string(l + 1) << ": mom 1 logo =" << l1 << endl;
//        cout << to_string(l + 1) << ": mom 3 logo =" << l3 << endl;
//        cout << to_string(l + 1) << ": mom 7 logo =" << l7 << endl;
        l1avg2 += l12;
        l3avg2 += l32;
        l7avg2 += l72;
    }
    l1avg2 /= 2;
    l3avg2 /= 2;
    l7avg2 /= 2;
    cout << "mom2 1 logo average=" << l1avg2 << endl;
    cout << "mom2 3 logo average=" << l3avg2 << endl;
    cout << "mom2 7 logo average=" << l7avg2 << endl;
    moments.l1avg2 = l1avg2;
    moments.l3avg2 = l3avg2;
    moments.l7avg2 = l7avg2;


    // calc moments for text - 'orlen'
    ImageMoments moms = ImageMoments(LOGO_DIR + TEXT_NAME);
    double t1 =  moms.get_moment(1);
    double t3 =  moms.get_moment(3);
    double t7 =  moms.get_moment(7);

    cout << "mom 1 text=" << t1 << endl;
    cout << "mom 3 text=" << t3 << endl;
    cout << "mom 7 text=" << t7 << endl;
    moments.t1 = t1;
    moments.t3 = t3;
    moments.t7 = t7;

    moms = ImageMoments(LOGO_DIR + TEXT_NAME + "_2");
    double t12 =  moms.get_moment(1);
    double t32 =  moms.get_moment(3);
    double t72 =  moms.get_moment(7);

    cout << "mom2 1 text=" << t12 << endl;
    cout << "mom2 3 text=" << t32 << endl;
    cout << "mom2 7 text=" << t72 << endl;
    moments.t12 = t12;
    moments.t32 = t32;
    moments.t72 = t72;



    // calc mean moments for noise
    /*
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
    */


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

        double logo_thr = 0.00016;
        double text_thr = 0.2;
        int num = 3;


        Mat mat_found = imread(INPUT_DIR + to_string(i) + ".jpg");
        ImageProcessing::resize(mat_found);

        for (int j = 0; j < num; ++j) {
            cout << "Segment: " << j << endl;

            leastMoms lms = process_bounding_box(els[j],moments);

            //imshow("Found logo at segment: " + to_string(j),els[j].box(Rect(lms.lx,lms.ly,lms.lwidth,lms.lheight)));
            //imshow("Found text at segment: " + to_string(j),els[j].box(Rect(lms.tx,lms.ty,lms.twidth,lms.theight)));
            //waitKey(-1);

            // ImageMoments im_moms(els[j].box);
            // double m1 =  im_moms.get_moment(1);
            // double m3 =  im_moms.get_moment(3);
            // double m7 =  im_moms.get_moment(7);

            // //double is_noise = im_moms.get_classification(m1,m3,m7,n1avg,n3avg,n7avg);
            // double is_logo  = im_moms.get_classification(m1,m3,m7,l1avg,l3avg,l7avg);
            // double is_text  = im_moms.get_classification(m1,m3,m7,t1,t3,t7);
            cout << "least dist object-logo: " << lms.ml << " least dist object-text:" << lms.mt << endl;

            if (lms.ml <= logo_thr) {
            	cout << "Found logo." << endl;
            	rectangle(mat_found, Point(lms.lx, lms.ly), Point(lms.lx + lms.lwidth, lms.ly + lms.lheight), 
            		0, 2, 8, 0);
            }

            if (lms.mt <= text_thr) {
            	cout << "Found text." << endl;
            	rectangle(mat_found, Point(lms.tx, lms.ty), Point(lms.tx + lms.twidth, lms.ty + lms.theight), 
            		128, 2, 8, 0);
            }


            // imwrite("out_data/recognized/" + "image_" + to_string(i) + "_" + to_string(els[idx].S)



            //double logo = is_logo / (is_logo + is_noise) * 100;
            //cout << "Segment: " << j << ". There is a logo. " << logo << "% sure." << endl;
            //double text = is_text / (is_text + is_noise) * 100;
            //cout << "Segment: " << j << ". There is a text. " << text << "% sure." << endl;
        }

        imwrite("out_data/recognized/image_" + to_string(i) + ".jpg", mat_found);


        els.clear();
        cout << endl;
    }
    //waitKey(-1);

    return 0;
}