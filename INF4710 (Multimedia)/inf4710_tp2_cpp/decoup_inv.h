
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

	/*cv::Mat_<T> test_output;
	for (int i = 0; i < numRows; i++) {
		cv::Mat_<T> row;
		for (int j = 0; j < numCols; j++) {
			std::cout << "charles: " << std::endl;
			cv::Mat_<T> test = vBlocks[8 * i + j];
			cv::Mat_<T> test_row(cv::Size(test.rows + 8*j,8));
			cv::hconcat(test, row, test_row);
			row = test_row;
		}
		test_output.release();
		test_output(cv::Size(row.cols, oOutput.rows));
		cv::vconcat(row, oOutput, test_output);
		test_output = oOutput;
	}*/

	for (int i = 0; i < vBlocks.size(); ++i) {
		for (int k = 0; k < vBlocks[i].cols; ++k) {
			for (int j = 0; j < vBlocks[i].rows; ++j) {
				oOutput.at<uchar>((i / numRows) * 8 + k, (i%numRows) * 8 + j) = vBlocks[i].at<uchar>(k, j);
			}
		}
	}

    return oOutput;
}
