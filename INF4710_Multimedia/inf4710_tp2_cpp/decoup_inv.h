
#pragma once

#include "tp2.h"

// decoup_inv: reconstructs a 2D image (i.e. always single channel) from a block array
template<size_t nBlockSize=8, typename T=uchar>
inline cv::Mat_<T> decoup_inv(const std::vector<cv::Mat_<T>>& vBlocks, const cv::Size& oImageSize) {
    CV_Assert(!vBlocks.empty() && !vBlocks[0].empty() && vBlocks[0].rows==nBlockSize && vBlocks[0].cols==nBlockSize && vBlocks[0].isContinuous());
    CV_Assert(oImageSize.area()>0 && (oImageSize.height%nBlockSize)==0 && (oImageSize.width%nBlockSize)==0);
    cv::Mat_<T> oOutput(oImageSize);

	int numRows = oImageSize.height / 8;
	int numCols = oImageSize.width / 8;

	for (int i = 0; i < vBlocks.size(); ++i) {
		for (int k = 0; k < vBlocks[i].cols; ++k) {
			for (int j = 0; j < vBlocks[i].rows; ++j) {
				oOutput.at<uchar>((i / numRows) * 8 + k, (i%numRows) * 8 + j) = vBlocks[i].at<uchar>(j, k);
			}
		}
	}

    return oOutput;
}
