#include "resize.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <stdint.h>
#include <stdio.h>

using namespace std;
using namespace cv;

typedef uint8_t *addr_t;
typedef struct
{
    addr_t img_addr;
    uint32_t width;
    uint32_t height;
} img_info_t;
typedef int32_t (*resize_kernel_t)(img_info_t &, img_info_t &);

const int32_t channel_num = 3;

static int32_t resize_nearest(img_info_t &new_img, img_info_t &old_img);
static int32_t convert_multi_channels(img_info_t &new_img,
                                      img_info_t &old_img,
                                      Mat origin_mat);

static int32_t resize_nearest(img_info_t &new_img, img_info_t &old_img)
{
    int32_t scale_w = old_img.width / new_img.width;
    int32_t scale_h = old_img.height / new_img.height;

    int32_t new_width = new_img.width;
    int32_t old_width = old_img.width;
    for (int32_t j = 0; j < new_img.height; j++)
    {
        for (int32_t i = 0; i < new_img.width; i++)
        {
            new_img.img_addr[i + j * new_width] = *(old_img.img_addr +
                                                    scale_w * i +
                                                    (j * scale_h) * old_width);
            uint8_t char_idx = *(old_img.img_addr +
                                 scale_w * i +
                                 (j * scale_h) * old_width);
        }
    }

    return 0;
}

static int32_t convert_multi_channels(img_info_t &new_img,
                                      img_info_t &old_img,
                                      Mat origin_mat)
{

    memcpy(old_img.img_addr, origin_mat.data, origin_mat.cols * origin_mat.rows);

    return 0;
}

int32_t resize_test()
{
    resize_kernel_t resize_kernel = 0;

    resize_kernel = resize_nearest;
    img_info_t new_img, old_img;
    new_img.width = 2;
    new_img.height = 2;
    new_img.img_addr = new uint8_t[new_img.width * new_img.height];
    old_img.width = 12;
    old_img.height = 8;
    old_img.img_addr = new uint8_t[old_img.width * old_img.height];
    for (int32_t idx = 0; idx < old_img.width * old_img.height; idx++)
    {
        old_img.img_addr[idx] = idx;
        uint8_t char_old = old_img.img_addr[idx];
    }
    resize_kernel(new_img, old_img);
    delete[] new_img.img_addr;
    delete[] old_img.img_addr;

    return 0;
}

int32_t resize_test_opencv()
{
    const string file_name = "./lena.jpg";
    img_info_t new_img, old_img;
    Mat img, gray_img, dst_img;
    resize_kernel_t resize_kernel = 0;
    vector<cv::Mat> origin_mat, new_mat;

    resize_kernel = resize_nearest;
    img = imread(file_name);
    cvtColor(img, gray_img, CV_BGR2GRAY);
    //    split(img, g_img);
    //    split(img, r_img);
#if 0
    old_img.width = gray_img.cols;
    old_img.height = gray_img.rows;
    old_img.img_addr = new uint8_t[old_img.width * old_img.height];
#endif
    old_img.width = gray_img.cols;
    old_img.height = gray_img.rows;
    old_img.img_addr = new uint8_t[old_img.width * old_img.height];
    new_img.width = 200;
    new_img.height = 200;
    new_img.img_addr = new uint8_t[new_img.width * new_img.height];
    split(img, origin_mat);
    for (int32_t i = 0; i < channel_num; i++)
    {
        convert_multi_channels(new_img,
                               old_img,
                               origin_mat[i]);
        resize_kernel(new_img, old_img);

        new_mat.push_back(Mat(new_img.height, new_img.width, CV_8UC1, new_img.img_addr).clone());
        // new_mat[i] = Mat(new_img.height, new_img.width, CV_8UC1, new_img.img_addr).clone();
    }
    merge(new_mat, dst_img);
    namedWindow("Display window", WINDOW_AUTOSIZE);
    namedWindow("converted Display", WINDOW_AUTOSIZE);
    imshow("Display window", gray_img);
    imshow("converted Display", dst_img);

    cvWaitKey(0);

    return 0;
}
