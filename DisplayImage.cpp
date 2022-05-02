#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "GaussianBlurSelf.h"
// pnsr method is provided in extra
using namespace cv;

std::string imageName = "pics/cameraman.tif"; // only for test purpose

int main(int argc, char** argv )
{
    srand(time(NULL));
    Mat image;
    image = imread( imageName, IMREAD_GRAYSCALE );
    image.convertTo(image,CV_32FC1);
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    int rowNImg = image.rows;
    int colNImg = image.cols;
    Mat res(image.rows,image.cols,image.type());
    auto accelerator = GaussianBlurSelf(8,1,BORDER_REPLICATE);
    accelerator.process(image,res);



    Mat groundTruth;
    GaussianBlur(image,groundTruth,{3,3},0,0,BORDER_REPLICATE);
    imwrite("groundTruth.png",groundTruth);
    cout<<"PSNR: " << PSNR(groundTruth,res);
//    for (int y=0;y<res.rows;y++){
//        for (int x=0;x<res.cols;x++){
//            if (abs(res.at<float>(y,x) - groundTruth.at<float>(y,x)) > 20) {
//                cout << res.at<float>(y, x) << " " << x << " " << y << endl;
//                res.at<float>(y, x) = 255;
//            }
//        }
//    }
    imwrite("self.png",res);
    waitKey(0);
    return 0;
}

