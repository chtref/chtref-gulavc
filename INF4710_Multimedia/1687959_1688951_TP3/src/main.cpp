
// INF4710 A2016 TP3 v1.4

#include "utils.hpp"

#define EDGE_TH 80
#define HISTOGRAM_DISTANCE 0.5
#define EDGE_TRANSITION 0.5
#define FADING_FRAMES_LIMIT 50

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
			float value = float(keep);
			oResult.at<float>(i, j) = value;
		}
	}

	return oResult;

}


float calculate_p(cv::Mat dilated, cv::Mat edge) {

	float numerator = 0.0;
	float decimator = 0.0;

	for (int i = 0; i < dilated.rows; ++i){
		for (int j = 0; j < dilated.rows; ++j) {
			numerator += dilated.at<float>(i, j) * edge.at<float>(i, j);
			decimator += edge.at<float>(i, j);
		}
	}

	return 1 - (numerator / decimator);
}





//This is a hand

int main(int /*argc*/, char** /*argv*/) {
	try {
		cv::VideoCapture oCap("../data/TP3_video.avi");
		CV_Assert(oCap.isOpened());

		int recent_transition = 0;

		std::vector<cv::Mat> histogramme;
		std::vector<cv::Mat> edgemaps;
		std::vector<cv::Mat> edgemaps_dilated;


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

			if (recent_transition > 0)
				recent_transition--;

			float frameDiff;

			histogramme.push_back(tp3::histo(oImg, 256));
			
			cv::Mat grad_x = tp3::convo(oImg, sob_x);
			cv::Mat grad_y = tp3::convo(oImg, sob_y);

			cv::Mat grad_mgt = magnitudeGradient(grad_x, grad_y);
			cv::Mat edge_Map = edgeMap(grad_mgt, EDGE_TH);
			
			edgemaps.push_back(edge_Map);

			cv::Mat dil_edges;
			cv::dilate(edge_Map, dil_edges, cv::Mat());
			edgemaps_dilated.push_back(dil_edges);

			std::vector<float> p_vect;

			if (i > 0) {
				//calculs
				frameDiff = euclidianDistance(histogramme[i - 1], histogramme[i]);

				float p_in = calculate_p(edgemaps_dilated[i - 1], edgemaps[i]);
				float p_out = calculate_p(edgemaps_dilated[i], edgemaps[i - 1]);
				float p = std::max(p_in, p_out);
				p_vect.push_back(p);

				

				//detection transitions
				if (i >= FADING_FRAMES_LIMIT) {
					float p_avg = 0.0;
					float p_avg_total = 0.0;
					for (int f = 0; f < p_vect.size(); ++f){										
						p_avg_total += p_vect[f];
						if (f < FADING_FRAMES_LIMIT) {
							p_avg += p_vect[i - f];
						}
					}
					p_avg_total /= p_vect.size();
					p_avg /= FADING_FRAMES_LIMIT;


					if (p_avg > p_avg_total && recent_transition == 0) {
						if (euclidianDistance(histogramme[i - FADING_FRAMES_LIMIT], histogramme[i]) > HISTOGRAM_DISTANCE) {
							recent_transition = FADING_FRAMES_LIMIT * 2;
							std::cout << "Transition detectee." << std::endl;
							system("pause");
						}
					}
				}
			}
			//for each frame, compare to the preceeding one'S histogram and conv.
			

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