/*
 * graphics.h
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <opencv2/opencv.hpp>

namespace Plot {

typedef cv::Scalar Color;

#define COLOR cv::Scalar
#define GREEN COLOR(0,150,0)
#define RED COLOR(0,0,200)
#define BLUE COLOR(200,0,0)
#define CYAN COLOR(0,255,255)
#define DARKGRAY COLOR(250,250,250)
#define LIGHTGRAY COLOR(200,200,200)
#define BLACK COLOR(0,0,0)


typedef cv::Mat_<cv::Vec3b> Canvas;


bool isAColor(int code);
int color2Code(Color);
Color code2Color(int);

}

#endif /* GRAPHICS_H_ */
