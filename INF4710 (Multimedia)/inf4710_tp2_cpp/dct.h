
#pragma once

#include "tp2.h"

// dct: computes the discrete cosinus tranform of a matrix
template<typename Tin=uchar,typename Tout=float>
inline cv::Mat_<Tout> dct(const cv::Mat_<Tin>& oBlock) {
    CV_Assert(!oBlock.empty() && oBlock.rows==oBlock.cols && (oBlock.rows%2)==0 && oBlock.isContinuous());
    cv::Mat_<Tout> oOutput(oBlock.size());

    // @@@@ TODO
	int n = oBlock.rows;

	for (int u = 0; u < oOutput.rows; ++u) {
		for (int v = 0; v < oOutput.cols; ++v) {

			float result = 0.0;

			for (int x = 0; x < (n - 1); ++x) {
				for (int y = 0; y < (n - 1); ++y) {
					
					// F_xy * cos#1 * cos#2
					result += float(oBlock.at<uchar>(x, y)) * cos(((CV_PI * (2.0*(x) + 1.0)*(u)) / (2.0*n))) * cos(((CV_PI * (2.0*(y) + 1.0)*(v)) / (2.0*n)));

				}
			}
			float c_u, c_v;
			if (u == 0) {
				c_u = sqrt(1.0 / n);
			}
			else {
				c_u = sqrt(2.0 / n);
			}

			if (v == 0) {
				c_v = sqrt(1.0 / n);
			}
			else {
				c_v = sqrt(2.0 / n);
			}

			oOutput.at<float>(u, v) = c_u * c_v * result;
		}
	}

    return oOutput;
}
