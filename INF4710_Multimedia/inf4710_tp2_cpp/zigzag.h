
#pragma once

#include "tp2.h"

// zigzag: returns a (NxN)-element 1D array created by zig-zagging through a NxN block
template<int nBlockSize=8, typename T=short>
inline std::array<T,nBlockSize*nBlockSize> zigzag(const cv::Mat_<T>& oBlock) {
	CV_Assert(!oBlock.empty() && oBlock.rows==oBlock.cols && oBlock.rows==nBlockSize);
    std::array<T,nBlockSize*nBlockSize> aZigzag;

    // @@@@ TODO

	enum Etat{
		DROIT,
		BAS,
		DIAGGAUCHE,
		DIAGDROIT		
	};

	int i = 0;
	int j = 0;
	int n = 0; // counter
	Etat state = DROIT;

	while (n < aZigzag.size()) {
		aZigzag.at(n) = oBlock.at<T>(i, j);
		++n;

		switch (state) {

		case DROIT:
			i++;
			if (j == 0) {
				state = DIAGGAUCHE;
			}
			else {
				state = DIAGDROIT;
			}
			break;


		case BAS:
			j++;
			if (i == 0) {
				state = DIAGDROIT;
			}
			else {
				state = DIAGGAUCHE;
			}
			break;


		case DIAGDROIT:
			i++;
			j--;
			if (i == oBlock.cols - 1) {
				state = BAS;
			}
			else if (j == 0) {
				state = DROIT;
			}
			break;


		case DIAGGAUCHE:
			i--;
			j++;
			if (j == oBlock.rows - 1) {
				state = DROIT;
			}
			else if (i == 0) {
				state = BAS;
			}
			break;


		default:
			//panic
			std::cout << "HOW DID I GET HERE?" << std::endl;
			break;
		}
	}
	
	return aZigzag;
}
