
// INF4710 A2016 TP3 v1.4

#include "utils.hpp"

int euclidianDistance(cv::Mat mat1, cv::Mat mat2) {
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



int main(int /*argc*/, char** /*argv*/) {
	try {
		cv::VideoCapture oCap("../data/TP3_video.avi");
		CV_Assert(oCap.isOpened());

		std::vector<cv::Mat> histogramme;

		for (int i = 0; i<(int)oCap.get(cv::CAP_PROP_FRAME_COUNT); ++i) {
			cv::Mat oImg;
			oCap >> oImg;
			cv::imshow("oImg", oImg);
			cv::waitKey(1);
			// ... @@@@ TODO

			histogramme.push_back(tp3::histo(oImg, 256));
			if (i > 0) {
				std::cout << euclidianDistance(histogramme[i - 1], histogramme[i]) << std::endl;
			}

			//for each frame, compare to the preceeding one'S histogram and conv.

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