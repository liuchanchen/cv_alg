#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

int32_t read_and_wirte()
{
    Mat img = imread("./test_pic.jpg");
    namedWindow("img_win");
    imshow("img_win", img);

    return 0;
}

int32_t main(int32_t argc, char **argv)
{
    read_and_wirte();

    return 0;
}