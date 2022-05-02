#include "GaussianBlurGen.h"
#include "TileImage.h"
#include "Graph/Graph.h"
#include <random>
#include "Graph/TrueRandomizer.h"
#include "Graph/RandomizerSobol.h"
#define K_PREC 8
#define LOOP_NUM 1
#define REAL false
#define TRUERNS false
#define PRINT false
#define SOBOL true

#if TRUERNS
auto rand_gen_kernel = TrueRandomizer();
#elif SOBOL
auto rand_gen_kernel = Randomizer();//RandomizerSobol();
#else
auto rand_gen_kernel =
#endif

#if TRUERNS
auto rand_gen_tile = TrueRandomizer();
#elif SOBOL
auto rand_gen_tile = RandomizerSobol();
#else
auto rand_gen_tile = Randomizer();
#endif

#if TRUERNS
auto rand_gen_tile2 = TrueRandomizer();
#elif SOBOL
auto rand_gen_tile2 = RandomizerSobol();
#else
auto rand_gen_tile2 = Randomizer(9,2);
#endif

class GaussianBlurSelf : public TiledAlgorithm{

public:

    GaussianBlurSelf(int tileSize,int padding,int borderType): TiledAlgorithm(tileSize,padding,borderType),
                                                               convolutionUnits(9){

        kernel[0][0] = rand_gen_kernel.transform(DToBN(0.0625,K_PREC));

        kernel[1][0] = rand_gen_kernel.transform(DToBN(0.125,K_PREC));
        kernel[2][0] = rand_gen_kernel.transform(DToBN(0.0625,K_PREC));


        kernel[0][1] = rand_gen_kernel.transform(DToBN(0.125,K_PREC));
        kernel[1][1] = rand_gen_kernel.transform(DToBN(0.25,K_PREC));
        //rand.setTap(1);
        kernel[2][1] = rand_gen_kernel.transform(DToBN(0.125,K_PREC));

        kernel[0][2] = rand_gen_kernel.transform(DToBN(0.0625,K_PREC));
        kernel[1][2] = rand_gen_kernel.transform(DToBN(0.125,K_PREC));
        kernel[2][2] = rand_gen_kernel.transform(DToBN(0.0625,K_PREC));

        auto xxx = DeRandomizer();
        for (int i=0;i<3;i++){
            for (int j=0;j<3;j++){
                cout << kernel[i][j].getNum()<<endl;
                cout<<BtoD(xxx.transform(kernel[i][j]).getNum()) << " ";
            }
            cout << endl;
        }
        getchar();
        kernel_real = (Mat_<float>(3,3)<<0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625);//0.0751136,0.123841,0.0751136,0.123841,0.20418,0.123841,0.0751136,0.123841,0.0751136);
        // 0.0625, 0.125, 0.0625
        // 0.125, 0.25, 0.125;
        // 0.0625, 0.125, 0.0625
//        vector<Operator*> ops(17); // 9 Muler 36 Adder
//        int i=0;
//        for (;i<9;i++){
//            ops[i] = new Multiplier(i,SC);
//        }
//        ops[9] = new Adder(9,BC,{ops[0],ops[1]});
//        ops[10] = new Adder(10,BC,{ops[2],ops[3]});
//        ops[11] = new Adder(11,BC,{ops[4],ops[5]});
//        ops[12] = new Adder(12,BC,{ops[6],ops[7]});
//        ops[13] = new Adder(13,BC,{ops[9],ops[10]});
//        ops[14] = new Adder(14,BC,{ops[11],ops[12]});
//        ops[15] = new Adder(15,BC,{ops[13],ops[14]});
//        ops[16] = new Adder(16,BC,{ops[15],ops[8]});
        for (int i=0;i<9;i++){
            convolutionUnits[i] = new Multiplier(i,SC);
        }
    }

protected:

    void processTileImpl(const cv::Mat& srcTile, cv::Mat& dstTile) {

        dstTile.create(8,8,CV_32FC1);
        float list[8][8] = {0};
        Mat tmp;
#if PRINT
        GaussianBlur(srcTile(Rect(1,1,8,8)),tmp,{3,3},0,0,BORDER_REPLICATE);
        cout << "Correct Mat:" <<endl;
        cout << tmp <<endl;
        cout << "Original Mat:" <<endl;
        cout << srcTile <<endl;
#endif
        GaussianBlur(srcTile(Rect(1,1,8,8)),tmp,{3,3},0,0,BORDER_REPLICATE);
        for (int i=0;i<LOOP_NUM;i++) {
            if (!REAL) {
                Number srcTileNumber[10][10];

                for (int y = 0; y < 10; y++) {
                    for (int x = 0; x < 10; x++) {
                        //if (x+10*y > 50)
                        srcTileNumber[x][y] = rand_gen_kernel.transform(DToBN(srcTile.at<float>(y, x) / 255, K_PREC));
                        //else
                        //    srcTileNumber[x][y] = rand_gen_tile2.transform(DToBN(srcTile.at<float>(y, x) / 255, K_PREC));

//                        auto xxx = DeRandomizer();
//                        cout << srcTile.at<float>(y, x) <<"@"<<endl;
//                        cout<<BtoD(xxx.transform(srcTileNumber[x][y]).getNum())*255 << " ";
//                        cout << endl;
//                        if (abs(BtoD(xxx.transform(srcTileNumber[x][y]).getNum())*255 - srcTile.at<float>(y, x) ) > 3)
//                            getchar();
#if !TRUERNS
                        //rand_gen_tile.setTap((int)srcTile.at<float>(y, x)%5);
#endif
                    }
                }
                for (int y = 1; y < srcTile.rows - 1; y++) {
                    for (int x = 1; x < srcTile.cols - 1; x++) {
                        float res = ConvolutionSC(srcTileNumber, x - 1, y - 1);
                        list[x-1][y-1] += res;
                        //dstTile.at<float>(y - 1, x - 1) = res;
//                        if (abs(tmp.at<float>(y - 1, x - 1) - res) > 25){
//                            string tmpErrLog;
//                            //tmpErrLog = (x -1 +'0') +','+( y - 1 +'0');
//                            tmpErrLog  = tmpErrLog + "correct value: " + to_string(tmp.at<float>(y - 1, x - 1)) + "\n"
//                                    + " my value: " + to_string(res) + "\n";
//
//                            tmpErrLog.append("my pixel matrix\n");
//                            cout << tmpErrLog;
//                            getchar();
//                        }
                    }
                }

            } else {
#if PRINT
            cout << "Original Mat:" <<endl;
            cout << srcTile <<endl;
            Mat tmp;
            GaussianBlur(srcTile(Rect(1,1,8,8)),tmp,{3,3},0,0,BORDER_REPLICATE);
            cout << "Correct Mat:" <<endl;
            cout << tmp <<endl;
#endif
                for (int y = 1; y < srcTile.rows - 1; y++) {
                    for (int x = 1; x < srcTile.cols - 1; x++) {
                        Mat subMat = srcTile(Rect(x - 1, y - 1, 3, 3));
                        Mat C = subMat.mul(kernel_real);
                        cout<<"Sub"<<endl;
                        cout << subMat<<endl;
                        cout<<"C"<<endl;
                        cout << C<<endl;
                        getchar();
                        auto res = sum(C)[0]; // convolution
                        //float tmp = ConvolutionSC(srcTileNumber,x-1,y-1);
                        //cout<<"Pixel Result: "<<tmp<<endl;
                        dstTile.at<float>(y - 1, x - 1) = res;
                    }
                }
#if PRINT
            cout << "My Mat:" <<endl;
            cout << dstTile <<endl;
            getchar();
#endif
            }
        }
        for (int y=0;y<8;y++){
            for (int x=0;x<8;x++){
                dstTile.at<float>(y,x) = list[x][y] / LOOP_NUM;
            }
        }
#if PRINT
        cout << "My Mat:" <<endl;
        cout << dstTile <<endl;
        cout << "Difference:" <<endl;
        Mat temp;
        absdiff(dstTile,tmp,temp);
        temp.convertTo(temp,CV_8UC1);
        cout << temp << endl;
        getchar();

#endif
    }

private:
    Number kernel[3][3];
    Mat kernel_real;
    vector<Operator *> convolutionUnits;
    float ConvolutionSC(Number inputMat[][10],int x,int y) {
        for (auto &k:convolutionUnits){
            k->clearInput();
        }
        vector<float> res(9);
        for (int i=0;i<9;i++){
            auto pixel = inputMat[x+i%3][y+i/3];
            auto weight = kernel[i%3][i/3];
            convolutionUnits[i]->addInputs(weight,pixel);
            res[i] = NumToD(convolutionUnits[i]->operate());
        }
        float sum = 0;
        for (auto k:res){
            sum+=k;
        }
        return sum*255;
    }
};