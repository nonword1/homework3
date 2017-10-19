#include "ConnectedComponentLabeling.h"

int ustc_ConnectedComponentLabeling(Mat grayImg, Mat& labelImg, int bin_th) {
	if (NULL == grayImg.data || CV_8UC1 != grayImg.type()) {
		cout << "Input image is invalid." << endl;
		return MY_FAIL;
	}
	if (bin_th < 0 || bin_th>255) {
		cout << "Input parameter is invalid." << endl;
		return MY_FAIL;
	}

	int height = grayImg.rows, width = grayImg.cols;
	Mat binaryImg(height + 2, width + 2, CV_8UC1, Scalar(0));
	for (int i = 0; i < height; i++) {
		uint8_t *grayImg_line = grayImg.ptr<uint8_t>(i);
		uint8_t *binaryImg_line = binaryImg.ptr<uint8_t>(i + 1);
		for (int j = 0; j < width; j++) {
			binaryImg_line[j + 1] = ((bin_th - grayImg_line[j]) >> 31) & 1;
		}
	}

	Mat ex_labelImg(height + 2, width + 2, CV_32SC1, Scalar(0));
	vector<int> result;
	result.push_back(0);
	for (int i = 1; i <= height; i++) {
		uint8_t *binImg_preline = binaryImg.ptr<uint8_t>(i - 1);
		uint8_t *binImg_line = binaryImg.ptr<uint8_t>(i);
		int32_t *labImg_preline = ex_labelImg.ptr<int32_t>(i - 1);
		int32_t *labImg_line = ex_labelImg.ptr<int32_t>(i);
		for (int j = 1; j <= width; j++) {
			if (binImg_line[j]) {
				int num = 0;
				int nlabels[4];
				nlabels[0] = labImg_preline[j - 1];
				num = binImg_preline[j - 1];
				nlabels[num] = labImg_preline[j];
				num += binImg_preline[j];
				nlabels[num] = labImg_preline[j + 1];
				num += binImg_preline[j + 1];
				nlabels[num] = labImg_line[j - 1];
				num += binImg_line[j - 1];
				if (0 == num) {
					labImg_line[j] = (int)result.size();
					result.push_back(0);
				}
				else {
					int min = nlabels[0];
					for (int k = 1; k < num; k++) {
						if (nlabels[k] < min) {
							min = nlabels[k];
						}
					}
					labImg_line[j] = min;
					for (int k = 0; k < num; k++) {
						if (nlabels[k] != min) {
							int big = nlabels[k];
							int small = min;
							while (small != result.at(big)) {
								if (0 == result.at(big)) {
									result.at(big) = small;
									break;
								}
								else {
									big = result.at(big);
									if (big < small) {
										int temp = big;
										big = small;
										small = temp;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	size_t length = result.size();
	for (int i = 1; i < length; i++) {
		if (0 == result.at(i)) {
			result.at(i) = i;
		}
		else {
			result.at(i) = result.at(result.at(i));
		}
	}

	labelImg.create(height, width, CV_32SC1);
	for (int i = 0; i < height; i++) {
		int32_t *labImg_line = labelImg.ptr<int32_t>(i);
		int32_t *exlabImg_line = ex_labelImg.ptr<int32_t>(i + 1);
		for (int j = 0; j < width; j++) {
			labImg_line[j] = result.at(exlabImg_line[j + 1]);
		}
	}
	return MY_OK;
}
