
#include "utils.hpp"

cv::Mat tp3::convo(const cv::Mat& oImage, const cv::Mat_<float>& oKernel) {
    CV_Assert(!oImage.empty() && oImage.type()==CV_8UC3 && oImage.isContinuous());
    CV_Assert(!oKernel.empty() && oKernel.cols==oKernel.rows && oKernel.isContinuous());
    CV_Assert(oImage.cols>oKernel.cols && oImage.rows>oKernel.rows);
    cv::Mat oResult(oImage.size(),CV_32FC3);

    // @@@@ TODO

	for (int i = 0; i < oImage.rows; ++i) {
		for (int j = 0; j < oImage.cols; ++j) {
			for (int k = 0; k < oImage.channels(); ++k) {
				//Parcourir le kernel
				float result = 0.0f;
				for (int m = -(floor(oKernel.rows/2.0)); m <= floor(oKernel.rows/2.0); ++m) {
					for (int n = -(floor(oKernel.cols / 2.0)); n <= floor(oKernel.cols / 2.0); ++n) {
						if(!(i + m < 0 || i + m > oImage.rows || j + n < 0 || j + n > oImage.cols)){
							result += oImage.at<uchar>(i + m, j + n, k)*oKernel.at<float>(m + floor(oKernel.rows / 2.0), n + floor(oKernel.cols / 2.0));
						}					
					}
				}
				oResult.at<float>(i,j,k) = result;
			}
		}
	}

    return oResult;
}