
#pragma once

#include "tp2.h"

// conv_ycbcr2rgb: converts an 8-bit-depth YCbCr image to RGB format considering optional 4:2:0 subsampling
inline void conv_ycbcr2rgb(const cv::Mat_<uchar>& Y, const cv::Mat_<uchar>& Cb, const cv::Mat_<uchar>& Cr, bool bSubsample, cv::Mat& RGB) {
    CV_Assert(!Y.empty() && Y.isContinuous() && !Cb.empty() && Cb.isContinuous() && !Cr.empty() && Cr.isContinuous());
    CV_Assert(!bSubsample || (Y.rows/2==Cb.rows && Y.rows/2==Cr.rows && Y.cols/2==Cb.cols && Y.cols/2==Cr.cols));
    CV_Assert(bSubsample || (Y.rows==Cb.rows && Y.rows==Cr.rows && Y.cols==Cb.cols && Y.cols==Cr.cols));
    RGB.create(Y.size(),CV_8UC3);

	if (bSubsample) {
		// Si subsample, les canaux CbCr seront plus petits
		// Puisque le subsampling est 4:2:0, les valeurs de CbCr seront identiques pour des carrés de 2x2 pixels, donc leur matrice est 2 fois plus petite dans chaque dimension

		// Loop on Y to restore RGB
		for (int i = 0; i < Y.rows; i++) {
			for (int j = 0; j < Y.cols; j++) {
				float Cb_prime = Cb.at<uchar>(i / 2, j / 2); //works because division entiere hue
				float Cr_prime = Cr.at<uchar>(i / 2, j / 2);
				float Y_Prime = Y.at<uchar>(i, j);

				uchar R = cv::saturate_cast<uchar>(Y_Prime + 1.403*(Cr_prime - 128));
				uchar G = cv::saturate_cast<uchar>(Y_Prime - 0.714*(Cr_prime - 128) - 0.344*(Cb_prime - 128));
				uchar B = cv::saturate_cast<uchar>(Y_Prime + 1.773*(Cb_prime - 128));

				//openCV rbg triplets are B,G,R -- NEVR 5GET
				RGB.at<cv::Vec3b>(i, j) = cv::Vec3b(B,G,R);
			}
		}
	}
	else {
		// Si pas de subsample, les canaux de donnees en format YCbCr seront de meme taille

		//coolr image
		for (int i = 0; i < Y.rows; i++) {
			for (int j = 0; j < Y.cols; j++) {
				float Cb_prime = Cb.at<uchar>(i, j);
				float Cr_prime = Cr.at<uchar>(i, j);
				float Y_Prime = Y.at<uchar>(i, j);

				uchar R = cv::saturate_cast<uchar>(Y_Prime + 1.403*(Cr_prime - 128));
				uchar G = cv::saturate_cast<uchar>(Y_Prime - 0.714*(Cr_prime - 128) - 0.344*(Cb_prime - 128));
				uchar B = cv::saturate_cast<uchar>(Y_Prime + 1.773*(Cb_prime - 128));

				//openCV rbg triplets are B,G,R -- NEVR 5GET
				RGB.at<cv::Vec3b>(i, j) = cv::Vec3b(B, G, R);
			}
		}

	}

}
