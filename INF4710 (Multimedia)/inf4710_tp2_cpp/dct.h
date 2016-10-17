
#pragma once

#include "tp2.h"

// dct: computes the discrete cosinus tranform of a matrix
template<typename Tin=uchar,typename Tout=float>
inline cv::Mat_<Tout> dct(const cv::Mat_<Tin>& oBlock) {
    CV_Assert(!oBlock.empty() && oBlock.rows==oBlock.cols && (oBlock.rows%2)==0 && oBlock.isContinuous());
    cv::Mat_<Tout> oOutput(oBlock.size());

    // @@@@ TODO
	int n = oBlock.rows;

	for (int u = 0; u < oOutput.rows; u++) {
		for (int v = 0; v < oOutput.cols; v++) {

			float result = 0.0;
			for (int i = 1; i <= n; i++) {
				for (int j = 1; j <= n; j++) {
					
					// F_xy * cos#1 * cos#2
					result += oBlock.at<uchar>(i-1, j-1) *cos((CV_PI * (2.0 * (i - 1.0) + 1.0) * (u - 1.0) / (2.0 * n))) * cos((CV_PI * (2.0 * (j - 1.0) + 1.0) * (v - 1.0) / (2.0 * n)));

				}
			}
			float c_u, c_v;
			if (u == 1) {
				c_u = sqrt(1.0 / n);
			}
			else {
				c_u = sqrt(2.0 / n);
			}

			if (v == 1) {
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
