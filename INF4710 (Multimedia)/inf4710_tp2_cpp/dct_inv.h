
#pragma once

#include "tp2.h"

// dct_inv: computes the inverse discrete cosinus tranform of a matrix
template<typename Tin=float,typename Tout=uchar>
inline cv::Mat_<Tout> dct_inv(const cv::Mat_<Tin>& oBlock) {
    CV_Assert(!oBlock.empty() && oBlock.rows==oBlock.cols && (oBlock.rows%2)==0 && oBlock.isContinuous());
    cv::Mat_<Tout> oOutput(oBlock.size());

	// @@@@ TODO
	int n = oBlock.rows;

	for (int x = 0; x < oOutput.rows; ++x) {
		for (int y = 0; y < oOutput.cols; ++y) {

			float F_xy = 0.0;

			for (int u = 0; u < (n - 1); ++u) {
				for (int v = 0; v < (n - 1); ++v) {

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

					// C_uv * cos#1 * cos#2
					F_xy += c_u * c_v * oBlock.at<float>(u, v) * cos(((CV_PI * (2.0*(x) + 1.0)*(u)) / (2.0*n))) * cos(((CV_PI * (2.0*(y) + 1.0)*(v)) / (2.0*n)));
				}
			}
			oOutput.at<uchar>(x, y) = cv::saturate_cast<uchar>(F_xy);
		}
	}

    return oOutput;
}
