
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void nms(
    const std::vector<cv::Rect> &srcRects,
    std::vector<cv::Rect> &resRects,
    float thresh)
{
    resRects.clear();

    const size_t size = srcRects.size();
    if (!size)
    {
        return;
    }

    // Sort the bounding boxes by the bottom - right y - coordinate of the bounding box
    std::multimap<int, size_t> idxs;
    for (size_t i = 0; i < size; ++i)
    {
        idxs.insert(std::pair<int, size_t>(srcRects[i].br().y, i));
    }

    // keep looping while some indexes still remain in the indexes list
    while (idxs.size() > 0)
    {
        // grab the last rectangle
        auto lastElem = --std::end(idxs);
        const cv::Rect &rect1 = srcRects[lastElem->second];

        resRects.push_back(rect1);

        idxs.erase(lastElem);

        for (auto pos = std::begin(idxs); pos != std::end(idxs);)
        {
            // grab the current rectangle
            const cv::Rect &rect2 = srcRects[pos->second];

            float intArea = (rect1 & rect2).area();
            float unionArea = rect1.area() + rect2.area() - intArea;
            float overlap = intArea / unionArea;

            // if there is sufficient overlap, suppress the current bounding box
            if (overlap > thresh)
            {
                pos = idxs.erase(pos);
            }
            else
            {
                ++pos;
            }
        }
    }
}

/**
 *******************************************************************************
 *
 *   main
 *
 *******************************************************************************
 */
int main(int argc, char *argv[])
{
    std::vector<cv::Rect> srcRects;

    /*
     // Test 1
     srcRects.push_back(cv::Rect(cv::Point(114, 60), cv::Point(178, 124)));
     srcRects.push_back(cv::Rect(cv::Point(120, 60), cv::Point(184, 124)));
     srcRects.push_back(cv::Rect(cv::Point(114, 66), cv::Point(178, 130)));*/

    /*
     // Test 2
     srcRects.push_back(cv::Rect(cv::Point(12, 84), cv::Point(140, 212)));
     srcRects.push_back(cv::Rect(cv::Point(24, 84), cv::Point(152, 212)));
     srcRects.push_back(cv::Rect(cv::Point(12, 96), cv::Point(140, 224)));
     srcRects.push_back(cv::Rect(cv::Point(36, 84), cv::Point(164, 212)));
     srcRects.push_back(cv::Rect(cv::Point(24, 96), cv::Point(152, 224)));
     srcRects.push_back(cv::Rect(cv::Point(24, 108), cv::Point(152, 236)));*/

    // Test 3
    srcRects.push_back(cv::Rect(cv::Point(12, 30), cv::Point(76, 94)));
    srcRects.push_back(cv::Rect(cv::Point(12, 36), cv::Point(76, 100)));
    srcRects.push_back(cv::Rect(cv::Point(72, 36), cv::Point(200, 164)));
    srcRects.push_back(cv::Rect(cv::Point(84, 48), cv::Point(212, 176)));

    cv::Size size(0, 0);
    for (const auto &r : srcRects)
    {
        size.width = std::max(size.width, r.x + r.width);
        size.height = std::max(size.height, r.y + r.height);
    }

    cv::Mat img = cv::Mat(2 * size.height, 2 * size.width, CV_8UC3, cv::Scalar(0, 0, 0));

    cv::Mat imgCopy = img.clone();

    for (auto r : srcRects)
    {
        cv::rectangle(img, r, cv::Scalar(0, 0, 255), 2);
    }

    cv::namedWindow("before", cv::WINDOW_NORMAL);
    cv::imshow("before", img);
    cv::waitKey(1);

    std::vector<cv::Rect> resRects;
    nms(srcRects, resRects, 0.3f);

    for (auto r : resRects)
    {
        cv::rectangle(imgCopy, r, cv::Scalar(0, 255, 0), 2);
    }

    cv::namedWindow("after", cv::WINDOW_NORMAL);
    cv::imshow("after", imgCopy);

    cv::waitKey(0);

    return 0;
}
