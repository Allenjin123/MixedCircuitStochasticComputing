//
// Created by alaallen on 3/16/22.
//

#ifndef DISPLAYIMAGE_TILEIMAGE_H
#define DISPLAYIMAGE_TILEIMAGE_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;
// tile: iterate over the image
// copy tiles from source image to local source tile,
// process it and write to corresponding area in the destination image.
float PSNR(const Mat& origin, const Mat& degrade){
    double min, max;
    minMaxLoc(origin, &min, &max);
    Mat s1;
    absdiff(origin,degrade,s1); // s1 = | origin - degrade |
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // | origin - degrade |^2
    float sse = sum(s1)[0];
    float mse = sse / (float ) origin.total();
    return 10 * log10(max*max/mse);
}


class TiledAlgorithm
{
private:
    int mBorderType;
public:
    TiledAlgorithm(int tileSize, int padding, int borderType)
            : mTileSize(tileSize)
            , mPadding(padding)
            , mBorderType(borderType)
    {
    }

    void process(const cv::Mat& sourceImage, cv::Mat& resultImage)
    {
        assert(!resultImage.empty());
        assert(sourceImage.rows == resultImage.rows);
        assert(sourceImage.cols == resultImage.cols);

        int rows = (sourceImage.rows / mTileSize) + (sourceImage.rows % mTileSize ? 1 : 0);
        int cols = (sourceImage.cols / mTileSize) + (sourceImage.cols % mTileSize ? 1 : 0);

        Mat tileInput;
        Mat tileOutput;


        for (int rowTile = 0; rowTile < rows; rowTile++)
        {
            for (int colTile = 0; colTile < cols; colTile++)
            {
                cv::Rect srcTileRect(colTile * mTileSize - mPadding,
                                 rowTile * mTileSize - mPadding,
                                 mTileSize + 2 * mPadding,
                                 mTileSize + 2 * mPadding);

                cv::Rect dstTileRect(colTile * mTileSize,
                                 rowTile * mTileSize,
                                 mTileSize,
                                 mTileSize);

                copySourceTile(sourceImage, tileInput, srcTileRect);
                processTileImpl(tileInput, tileOutput);
                copyTileToResultImage(tileOutput, resultImage, dstTileRect);
                cout << rowTile << " " << colTile <<endl;
            }
        }
    }


protected:
    virtual void processTileImpl(const cv::Mat& srcTile, cv::Mat& dstTile)  = 0;


    void copyTileToResultImage(const cv::Mat& tileImage, cv::Mat& resultImage, cv::Rect resultRoi)
    {
            cv::Rect srcTile(mPadding, mPadding, mTileSize, mTileSize);

            auto br = resultRoi.br();

            if (br.x >= resultImage.cols)
            {
                resultRoi.width -= br.x - resultImage.cols;
                srcTile.width -= br.x - resultImage.cols;
            }

            if (br.y >= resultImage.rows)
            {
                resultRoi.height -= br.y - resultImage.rows;
                srcTile.height -= br.y - resultImage.rows;
            }

            cv::Mat tileView = tileImage;//(srcTile);
            //tileImage 8 * 8 srcTile (1,1,8,8)
            cv::Mat dstView = resultImage(resultRoi);

            assert(tileView.rows == dstView.rows);
            assert(tileView.cols == dstView.cols);

            tileView.copyTo(dstView);

    }
    void copySourceTile(const cv::Mat& src, cv::Mat& srcTile, cv::Rect &tile)
    {
        auto tl = tile.tl();
        auto br = tile.br();

        // tile with padding included
        cv::Point tloffset={0,0}, broffset={0,0};

        // Take care of border cases
        // x≤pt.x<x+width,y≤pt.y<y+height
        if (tile.x < 0)
        {
            tloffset.x = -tile.x;
            tl.x = 0;
        }

        if (tile.y < 0)
        {
            tloffset.y = -tile.y;
            tl.y = 0;
        }

        if (br.x >= src.cols)
        {
            broffset.x = br.x - src.cols + 1;
            br.x -= broffset.x;
        }

        if (br.y >= src.rows)
        {
            broffset.y = br.y - src.rows + 1;
            br.y -= broffset.y;
        }

        // If any of the tile sides exceed source image boundary we must use copyMakeBorder to make proper paddings for this side
        if (tloffset.x > 0 || tloffset.y > 0 || broffset.x > 0 || broffset.y > 0)
        {
            cv::Rect paddedTile(tl, br);
            assert(paddedTile.x >= 0);
            assert(paddedTile.y >= 0);
            assert(paddedTile.br().x < src.cols);
            assert(paddedTile.br().y < src.rows);
//            cout<<tile.tl() << tile.br()<<endl;
            cv::copyMakeBorder(src(paddedTile), srcTile, tloffset.y, broffset.y, tloffset.x, broffset.x, mBorderType);
        }
        else
        {
            // Entire tile (with paddings lies inside image and it's safe to just take a region:
            src(tile).copyTo(srcTile);
            // before copying the data, the method invoke srcTile.create ...
        }
    }

    int mPadding;
    int mTileSize;
};
#endif //DISPLAYIMAGE_TILEIMAGE_H
