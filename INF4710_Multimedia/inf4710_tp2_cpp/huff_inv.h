
#pragma once

#include "huff_common.h"

template<size_t N=64, typename T=short>
inline std::vector<std::array<T,N>> huff_inv(const HuffOutput<T>& oInput) {
    CV_Assert(!oInput.map.empty() && !oInput.string.empty());
    std::vector<std::array<T,N>> vvOutput;

    // @@@@ TODO

	struct sort_pred {
		//To sort our vector by code length
		bool operator()(const std::pair<HuffCode, T> &left, const std::pair<HuffCode, T> &right) {
			return left.first.size() < right.first.size();
		}
	};

	//Put all HuffCodes & Values in a vector to be able to easily access codes
	std::vector<std::pair<HuffCode, T>> codeList;
	for (auto const &it : oInput.map) {
		codeList.push_back(std::pair<HuffCode,T>(it.second, it.first));
	}
	//Sort vector by code length
	std::sort(codeList.begin(), codeList.end(), sort_pred());

	HuffCode currentValue;
	std::array<T, N> tempOutput;
	int currentChar = 0;
	for (size_t i = 0; i < oInput.string.size(); ++i) {

		currentValue.push_back(bool(oInput.string.at(i)));
		for (auto &it = codeList.begin(); it < codeList.end() && it->first.size() <= currentValue.size(); ++it) {
			if (it->first == currentValue)
			{
				tempOutput[currentChar] = it->second;
				currentChar++;
				currentValue.clear();
				if (currentChar >= N) {
					vvOutput.push_back(tempOutput);
					currentChar = 0;
				}
			}
		}
		

	}
   
    return vvOutput;
}
