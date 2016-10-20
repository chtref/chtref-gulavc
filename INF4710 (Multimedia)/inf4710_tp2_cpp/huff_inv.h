
#pragma once

#include "huff_common.h"

template<size_t N=64, typename T=short>
inline std::vector<std::array<T,N>> huff_inv(const HuffOutput<T>& oInput) {
    CV_Assert(!oInput.map.empty() && !oInput.string.empty());
    std::vector<std::array<T,N>> vvOutput;

    // @@@@ TODO

	std::vector<bool> currentValue;

	for (size_t i = 0; i < oInput.string.size(); ++i) {

		currentValue.push_back(oInput.string.at(i));
		//if (/* on determine si la valeur du vecteur est dans la map */)
		//{
		//	vvOutput.push_back(/* la valeur associee dans la map, must be array<T,N> */);
		//	currentvalue.clear();
		//}

	}
   
    return vvOutput;
}
