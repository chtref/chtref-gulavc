
#pragma once

#include "tp2.h"

// decoup: reformats a 2D image (i.e. always single channel) to a block array
template<size_t nBlockSize=8, typename T=uchar>
inline std::vector<cv::Mat_<T>> decoup(const cv::Mat_<T>& oImage) {
    CV_Assert(!oImage.empty() && (oImage.rows%nBlockSize)==0 && (oImage.cols%nBlockSize)==0 && oImage.isContinuous());
    std::vector<cv::Mat_<T>> vOutput;

    // @@@@ TODO
	for (int i = 0; i < oImage.rows; i+=8) {
		for (int j = 0; j < oImage.cols; j+=8) {
			cv::Mat_<T> mat8cols = oImage.colRange(cv::Range(j, j + 8));
			cv::Mat_<T> mat8by8 = mat8cols.rowRange(cv::Range(i, i + 8));
			
			//below is not clean programming. I suggest you avert your eyes
			cv::Mat_<T> output;
			output.create(8, 8);
			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 8; l++) {
					output.at<T>(k, l) = mat8by8.at<T>(k, l);
				}
			}

			vOutput.push_back(output);
		}

	}


    return vOutput;
}
