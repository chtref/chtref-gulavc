
#include "utils.hpp"

cv::Mat tp3::convo(const cv::Mat& oImage, const cv::Mat_<float>& oKernel) {
	CV_Assert(!oImage.empty() && oImage.type() == CV_8UC3 && oImage.isContinuous());
	CV_Assert(!oKernel.empty() && oKernel.cols == oKernel.rows && oKernel.isContinuous());
	CV_Assert(oImage.cols>oKernel.cols && oImage.rows>oKernel.rows);
	cv::Mat oResult(oImage.size(), CV_32FC3);

	// @@@@ TODO

	std::vector<cv::Mat> resultChannels(3);
	cv::split(oResult, resultChannels);

	std::vector<cv::Mat> imageChannels(3);
	cv::split(oImage, imageChannels);

	for (int i = 0; i < oImage.rows; ++i) {
		for (int j = 0; j < oImage.cols; ++j) {
			for (int k = 0; k < oImage.channels(); ++k) {


				//Parcourir le kernel
				float result = 0.0f;

				for (int m = -(floor(oKernel.rows / 2.0)); m <= floor(oKernel.rows / 2.0); ++m) {


					for (int n = -(floor(oKernel.cols / 2.0)); n <= floor(oKernel.cols / 2.0); ++n) {


						if (!(i + m < 0 || i + m >= oImage.rows || j + n < 0 || j + n >= oImage.cols)) {


							//cv::Vec3f channels = oImage.at<uchar>(i + m, j + n);

							float temp1 = imageChannels[k].at<uchar>(i + m, j + n);
							float temp2 = m + floor(oKernel.rows / 2.0);
							float temp3 = n + floor(oKernel.cols / 2.0);
							float temp4 = oKernel.at<float>(temp2, temp3);

							float temp5 = temp1 * temp4;
							result += temp5;
							//result += channels[k]*oKernel.at<float>(m + floor(oKernel.rows / 2.0), n + floor(oKernel.cols / 2.0));
						}
					}
				}
				resultChannels[k].at<float>(i, j) = result;
				

			}
		}
	}

	cv::merge(resultChannels, oResult);
	return oResult;
}