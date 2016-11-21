
// INF4710 A2016 TP3 v1.4

#include "utils.hpp"

float euclidianDistance(cv::Mat mat1, cv::Mat mat2) {
	CV_Assert(mat1.size() == mat2.size());

	float distance = 0;

	for (int i = 0; i < mat1.rows; ++i) {
		for (int j = 0; j < mat1.cols; ++j) {
			//std::cout << mat1.at<float>(i, j) << " " << mat2.at<float>(i, j) << std::endl;
			distance += sqrt(pow(mat1.at<float>(i, j) - mat2.at<float>(i, j), 2));
		}
	}

	return distance;
}

cv::Mat magnitudeGradient(cv::Mat grad_x, cv::Mat grad_y) {
	CV_Assert(grad_x.size() == grad_y.size());
	cv::Mat oResult(grad_x.size(), CV_32FC3);

	std::vector<cv::Mat> grad_channels_x(3), grad_channels_y(3), grad_channels_result(3);
	cv::split(grad_x, grad_channels_x);
	cv::split(grad_y, grad_channels_y);
	cv::split(oResult, grad_channels_result);

	for (int i = 0; i < grad_x.rows; ++i) {
		for (int j = 0; j < grad_x.cols; ++j) {
			for (int k = 0; k < grad_x.channels(); ++k) {
				float x = grad_channels_x[k].at<float>(i, j);
				float y = grad_channels_y[k].at<float>(i, j);
				float result = sqrt(pow(x, 2) + pow(y, 2));

				grad_channels_result[k].at<float>(i, j) = result;
			}
		}
	}

	cv::merge(grad_channels_result, oResult);
	return oResult;
}

cv::Mat edgeMap(cv::Mat gradmap, float th) {

	std::vector<cv::Mat> channels(3);
	cv::split(gradmap, channels);

	cv::Mat oResult(channels[0].size(), CV_32F);

	for (int i = 0; i < channels[0].rows; ++i) {
		for (int j = 0; j < channels[0].cols; ++j) {
			bool keep = false;
			for (int k = 0; k < gradmap.channels() && !keep; ++k) {

				if (channels[k].at<float>(i, j) >= th) {
					keep = true;
				}

			}
			float value = float(keep) * 255;
			oResult.at<float>(i, j) = value;
		}
	}

	return oResult;

}




//This is a hand

int main(int /*argc*/, char** /*argv*/) {
	try {
		cv::VideoCapture oCap("../data/TP3_video.avi");
		CV_Assert(oCap.isOpened());

		std::vector<cv::Mat> histogramme;

		cv::Mat_<float> sob_x(3, 3);
		cv::Mat_<float> sob_y(3, 3);

		sob_x = (cv::Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
		sob_y = (cv::Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

		for (int i = 0; i < sob_x.rows; ++i) {
			for (int j = 0; j < sob_x.cols; ++j) {
				std::cout << sob_y.at<float>(i, j) << " ";
			}
			std::cout << std::endl;
		}

		for (int i = 0; i<(int)oCap.get(cv::CAP_PROP_FRAME_COUNT); ++i) {
			cv::Mat oImg;
			oCap >> oImg;
			cv::imshow("oImg", oImg);
			// ... @@@@ TODO

			float frameDiff;

			histogramme.push_back(tp3::histo(oImg, 256));
			if (i > 0) {
				frameDiff = euclidianDistance(histogramme[i - 1], histogramme[i]);
				//std::cout << frameDiff << std::endl;
				if (frameDiff > 0.5) {
					system("pause");
				}
			}

			//for each frame, compare to the preceeding one'S histogram and conv.

			cv::Mat grad_x = tp3::convo(oImg, sob_x);
			cv::Mat grad_y = tp3::convo(oImg, sob_y);

			cv::Mat grad_mgt = magnitudeGradient(grad_x, grad_y);
			cv::Mat edge_Map = edgeMap(grad_mgt, 80);

			cv::imshow("edge_Map", edge_Map);

			cv::waitKey(1);

		}
	}
	catch (const cv::Exception& e) {
		std::cerr << "Caught cv::Exceptions: " << e.what() << std::endl;
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Caught std::runtime_error: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Caught unhandled exception." << std::endl;
	}
	return 0;
}