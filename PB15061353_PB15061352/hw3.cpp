#include <iostream>
#include <math.h>
#include "Find_Connected_Domain.h"

#define ARRAYSIZE 65536

int ustc_ConnectedComponentLabeling(Mat grayImg, Mat &labelImg,int bin_th) {
	
	if (NULL == grayImg.data && NULL == labelImg.data) {
		return MY_FAIL;
	}

	int NumArray[ARRAYSIZE] = { 0 };
	int length = 0;

	for (int i = 0; i < ARRAYSIZE; i++) {
		NumArray[i] = i;
	}

	int height = grayImg.rows;
	int width = grayImg.cols;
	Mat binaryImg(height, width, CV_8UC1);
	//对灰度图二值化
	threshold(grayImg, binaryImg, bin_th, 1, THRESH_BINARY);
	//对自身和相邻四格分别以1，2，4，8，16加权
	Mat testImg = binaryImg.clone();
	Mat subimg1 = testImg(Range(1, height), Range(1, width));
	subimg1 = subimg1 + binaryImg(Range(0, height - 1), Range(0, width - 1)) * 16;
	Mat subimg2 = testImg(Range(1, height), Range(0, width));
	subimg2 = subimg2 + binaryImg(Range(0, height - 1), Range(0, width)) * 8;
	Mat subimg3 = testImg(Range(1, height), Range(0, width - 1));
	subimg3 = subimg3 + binaryImg(Range(0, height - 1), Range(1, width)) * 4;
	Mat subimg4 = testImg(Range(0, height), Range(1, width));
	subimg4 = subimg4 + binaryImg(Range(0, height), Range(0, width - 1)) * 2;

	uchar* testImgptr = testImg.data;
	int* connectptr = (int *)labelImg.data;
	int limit = height*width;

	//对其第一次扫描
	for (int pos = 0; pos < limit; pos++) {
			int value = testImgptr[pos];
			switch (value) {
			case 1: connectptr[pos] = (++length); break;
			//a
			case 17:connectptr[pos] = connectptr[pos - width - 1]; break;
			//b
			case 9:
			case 25:connectptr[pos] = connectptr[pos - width]; break;
			//c
			case 5:
			case 13:
			case 29:connectptr[pos] = connectptr[pos - width + 1]; break;
			//d
			case 3:
			case 11:
			case 19:
			case 27:connectptr[pos] = connectptr[pos - 1]; break;
			//ac
			case 21:
			{
				int num1 = connectptr[pos - width - 1];
				int num2 = connectptr[pos - width + 1];

				if (num1 == num2) {
					connectptr[pos] = num1;
					break;
				}
				else if (num1 > num2) {
					connectptr[pos] = num2;
				}
				else{
					connectptr[pos] = num1;
				}
				int value1 = NumArray[num1];
				int value2 = NumArray[num2];
				if (value1 > value2) {
					int minvalue = value2;
					for (int i = 1; i <= length; i++) {
						if (NumArray[i] == value1) {
							NumArray[i] = minvalue;
						}
					}
				}
				else if (value1 < value2) {
					int minvalue = value1;
					for (int i = 1; i <= length; i++) {
						if (NumArray[i] == value2) {
							NumArray[i] = minvalue;
						}
					}
				}
				break;
			}
			//cd
			case 7:
			case 15:
			case 23:
			case 31:
			{
				int num1 =  connectptr[pos - 1];
				int num2 =  connectptr[pos -width + 1];

				if (num1 == num2) {
					connectptr[pos] = num1;
					break;
				}
				else if (num1 > num2) {
					connectptr[pos] = num2;
				}
				else {
					connectptr[pos] = num1;
				}
				int value1 = NumArray[num1];
				int value2 = NumArray[num2];
				if (value1 > value2) {
					int minvalue = value2;
					for (int i = 1; i <= length; i++) {
						if (NumArray[i] == value1) {
							NumArray[i] = minvalue;
						}
					}
				}
				else if (value1 < value2) {
					int minvalue = value1;
					for (int i = 1; i <= length; i++) {
						if (NumArray[i] == value2) {
							NumArray[i] = minvalue;
						}
					}
				}
				break;
			}
			//0
			default:connectptr[pos] = 0; break;
			}
	}

	//对其第二次扫描
	for (int row_col = 0; row_col < limit; row_col++) {
		int value = connectptr[row_col];
		connectptr[row_col] = NumArray[value];
	}

	return MY_OK;
}
