
#pragma once

#include "tp2.h"

// conv_rgb2ycbcr: converts an 8-bit-depth RGB image to Y'CbCr format using optional 4:2:0 subsampling
inline void conv_rgb2ycbcr(const cv::Mat& RGB, bool bSubsample, cv::Mat_<uchar>& Y, cv::Mat_<uchar>& Cb, cv::Mat_<uchar>& Cr) {
    CV_Assert(!RGB.empty() && RGB.type()==CV_8UC3 && RGB.isContinuous());
    Y.create(RGB.rows,RGB.cols);

	if (bSubsample)	{
		// Si subsample, les canaux CbCr seront plus petits
		// Puisque le subsampling est 4:2:0, les valeurs de CbCr seront identiques pour des carrés de 2x2 pixels, donc leur matrice est 2 fois plus petite dans chaque dimension
		Cb.create(RGB.rows/2, RGB.cols/2);
		Cr.create(RGB.rows/2, RGB.cols/2);

		// Loop to calculate Y
		for (int i = 0; i < RGB.rows; i++) {
			for (int j = 0; j < RGB.cols; j++) {
				cv::Vec3b rgbvalues = RGB.at<cv::Vec3b>(i, j);
				//openCV rbg triplets are B,G,R

				uchar Y_prime = 0.299*rgbvalues[2] + 0.587*rgbvalues[1] + 0.114*rgbvalues[0];
				Y.at<uchar>(i, j) = Y_prime;

			}
		}

		// Loop to calculate Cb & Cr, taken from the value of the top left pixel from the group of 4
		for (int i = 0; i < RGB.rows; i+=2) {
			for (int j = 0; j < RGB.cols; j+=2) {
				cv::Vec3b rgbvalues = RGB.at<cv::Vec3b>(i, j);
				//openCV rbg triplets are B,G,R
				uchar Y_prime = 0.299*rgbvalues[2] + 0.587*rgbvalues[1] + 0.114*rgbvalues[0];
				Cb.at<uchar>(i/2, j/2) = 128 + 0.564*(rgbvalues[0] - Y_prime);
				Cr.at<uchar>(i/2, j/2) = 128 + 0.713*(rgbvalues[2] - Y_prime);

			}
		}
	}
	else {
		// Si pas de subsample, les canaux de donnees en format YCbCr seront de meme taille

		Cb.create(RGB.rows, RGB.cols);
		Cr.create(RGB.rows, RGB.cols);

		//coolr image
		for (int i = 0; i < RGB.rows; i++) {
			for (int j = 0; j < RGB.cols; j++) {
				cv::Vec3b rgbvalues = RGB.at<cv::Vec3b>(i, j);
				//openCV rbg triplets are B,G,R

				uchar Y_prime = 0.299*rgbvalues[2] + 0.587*rgbvalues[1] + 0.114*rgbvalues[0];
				Y.at<uchar>(i, j) = Y_prime;
				Cb.at<uchar>(i, j) = 128 + 0.564*(rgbvalues[0] - Y_prime);
				Cr.at<uchar>(i, j) = 128 + 0.713*(rgbvalues[2] - Y_prime);
			}
		}


	}

}
