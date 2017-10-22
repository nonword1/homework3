#include "ConnectedComponentLabeling.h"

int ustc_ConnectedComponentLabeling(Mat grayImg, Mat& labelImg, int bin_th) {
	if (NULL == grayImg.data || NULL == labelImg.data)
	{
		cout << "image is NULL." << endl;
		return MY_FAIL;
	}
	if (grayImg.rows != labelImg.rows || grayImg.cols != labelImg.cols)
	{
		cout << "image is unequal." << endl;
		return MY_FAIL;
	}
	Mat binaryImg(grayImg.rows, grayImg.cols, CV_8UC1);
	threshold(grayImg, binaryImg, bin_th, 1, 0);
	uchar* binaryPtr = binaryImg.data;
	int* labelPtr = (int*)labelImg.data;
	int *labelC1, *labelC2, *labelC3, *labelC4;
	const int binImgCols = binaryImg.cols;
	const int binImgRows = binaryImg.rows;
	const int binImgSize = binImgCols*binImgRows;
	memset(labelImg.data, 0, binImgSize*4);
	int *connectedLabel = new int[binImgSize / 4 + 1]{ 0 };
	int *nextLabel = new int[binImgSize / 4 + 1]{ 0 };
	int *tailLabel = new int[binImgSize / 4 + 1]{ 0 };
	int labelingIndex = 1;
	int rowsIndex, colsIndex;
	int cmpFlag;
	int nextIndex;
	int get1Label, get2Label;
	int maxLabel, minLabel;

	labelC3 = labelPtr;
	labelC4 = labelPtr + 1;
	//Special judge on the first pixel
	labelingIndex += ((*binaryPtr) ? 0 : (*labelPtr = labelingIndex, connectedLabel[labelingIndex] = labelingIndex, tailLabel[labelingIndex] = labelingIndex, 1));
	binaryPtr++;
	labelPtr++;

	//Special judge on the first row
	for (colsIndex = binImgCols - 1; colsIndex; colsIndex--, binaryPtr++, labelPtr++) {
		if (*binaryPtr) continue;
		*labelPtr = ((*(labelPtr - 1)) ? (*(labelPtr - 1)) : (connectedLabel[labelingIndex] = labelingIndex, tailLabel[labelingIndex] = labelingIndex, labelingIndex++));
	}

	//Ordinary judge on other rows
	for (rowsIndex = binImgRows - 1; rowsIndex; rowsIndex--) {
		*labelPtr = ((*binaryPtr) ? 0 : ((*labelC3) ? (*labelC3) : ((*labelC4) ? (*labelC4) : (connectedLabel[labelingIndex] = labelingIndex, tailLabel[labelingIndex] = labelingIndex, labelingIndex++))));
		binaryPtr++;
		labelPtr++;
		labelC2 = labelC3;
		labelC3++;
		labelC4++;
		labelC1 = labelPtr - 1;

		//Ordinary judge on the other pixels of every row
		for (colsIndex = binImgCols - 2; colsIndex; colsIndex--, binaryPtr++, labelPtr++, labelC1++, labelC2++, labelC3++, labelC4++) {
			//labelC4++;
			if (*binaryPtr) continue;
			cmpFlag = ((*labelC4) ? ((*labelC2) ? ((connectedLabel[*labelC2] == connectedLabel[*labelC4]) ? (4) : (6)) : ((*labelC1) ? ((connectedLabel[*labelC4] == connectedLabel[*labelC1]) ? (4) : (5)) : (4))) : ((*labelC3) ? (3) : ((*labelC2) ? (2) : ((*labelC1) ? (1) : (0)))));
			*labelPtr = cmpFlag ? ((--cmpFlag) ? ((--cmpFlag) ? ((--cmpFlag) ? ((cmpFlag--, (*labelC4))) : (*labelC3)) : (*labelC2)) : (*labelC1)) : (connectedLabel[labelingIndex] = labelingIndex, tailLabel[labelingIndex] = labelingIndex, labelingIndex++);
			if (cmpFlag) {
				get1Label = connectedLabel[*labelC4];
				get2Label = (cmpFlag & 1)*(connectedLabel[*labelC1]) + ((cmpFlag & 2) >> 1)*(connectedLabel[*labelC2]);
				maxLabel = get1Label > get2Label ? (minLabel = get2Label, get1Label) : (minLabel = get1Label, get2Label);
				nextIndex = maxLabel;
				while(nextIndex) {
					//cout << nextIndex << ' ';
					connectedLabel[nextIndex] = minLabel;
					nextIndex = nextLabel[nextIndex];
				}
				nextLabel[tailLabel[minLabel]] = maxLabel;
				tailLabel[minLabel] = tailLabel[maxLabel];
			}
		}
		*labelPtr = (*binaryPtr) ? 0 : ((*labelC3) ? (*labelC3) : ((*labelC2) ? (*labelC2) : ((*labelC1) ? (*labelC1) : (connectedLabel[labelingIndex] = labelingIndex, tailLabel[labelingIndex] = labelingIndex, labelingIndex++))));
		labelC3++;
		labelC4++;
		binaryPtr++;
		labelPtr++;
	}
	labelPtr = (int*)labelImg.data;
	for (int i = binImgSize; i; i--) {
		*labelPtr = connectedLabel[*labelPtr];
		labelPtr++;
	}
	return MY_OK;
}
