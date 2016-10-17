
#pragma once

#include "tp2.h"

// dct_inv: computes the inverse discrete cosinus tranform of a matrix
template<typename Tin=float,typename Tout=uchar>
inline cv::Mat_<Tout> dct_inv(const cv::Mat_<Tin>& oBlock) {
    CV_Assert(!oBlock.empty() && oBlock.rows==oBlock.cols && (oBlock.rows%2)==0 && oBlock.isContinuous());
    cv::Mat_<Tout> oOutput(oBlock.size());

	// @@@@ TODO
	int n = oBlock.rows;

	for (int i = 0; i < oOutput.rows; i++) {
		for (int j = 0; j < oOutput.cols; j++) {

			float F_xy = 0.0;
			for (int u = 1; u <= n; u++) {
				for (int v = 1; v <= n; v++) {

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

					// C_uv * cos#1 * cos#2
					F_xy += c_u * c_v * oBlock.at<float>(u-1, v-1) *cos((CV_PI * (2.0 * (i - 1.0) + 1.0) * (u - 1.0) / (2.0 * n))) * cos((CV_PI * (2.0 * (j - 1.0) + 1.0) * (v - 1.0) / (2.0 * n)));

				}
			}
			

			oOutput.at<uchar>(i, j) = cv::saturate_cast<uchar>(F_xy);
		}
	}

    return oOutput;
}
