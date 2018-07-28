#!/bin/bash
#g++ main.cpp resize.cpp -g -l opencv_core -l  opencv_highgui -l  opencv_imgproc -l opencv_imgcodecs
rm -rf a.out
g++  nms_sim.cpp -g -l opencv_core -l  opencv_highgui -l  opencv_imgproc -std=c++11
