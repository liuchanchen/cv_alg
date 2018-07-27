#include "resize.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
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

static int32_t resize_nearest(img_info_t &new_img, img_info_t &old_img);

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
            printf("%d \r\n", new_img.img_addr[i + j * new_width]);
        }
    }

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
    old_img.img_addr = new uint8_t[new_img.width * new_img.height];
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
    Mat img, dst_img;
    const string file_name = "./lean.jpg";

    img = imread(file_name);
    cvtColor(img, dst_img, CV_BGR2GRAY);
    namedWindow("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", img);
    cvWaitKey(0);

    return 0;
}