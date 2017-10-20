#include “ConnectedComponentLabeling.h”

int ustc_ConnectedComponentLabeling(
	Mat grayImg,
	Mat& labelImg,
	int bin_th) {
	if (NULL == grayImg.data)
	{
		cout << "image is NULL." << endl;
		return MY_FAIL;
	}
	int width = grayImg.cols;
	int height = grayImg.rows;
	int height_2 = height + 1;
	int width_2 = width + 2;
	Mat thresholded_2(height_2, width_2, CV_8UC1);
	for (int row_i = 0; row_i < width_2; row_i++) {
		thresholded_2.data[row_i] = 0;
	}
	for (int row_i = 1; row_i < height_2; row_i++) {
		int temp = row_i*width_2;
		thresholded_2.data[temp] = 0;
		thresholded_2.data[temp+width_2-1] = 0;
	}
	Mat thresholded = thresholded_2(Rect(1, 1, width, height));
	int* T = (int *)malloc((width*height/4) * sizeof(int));
	int* xy = (int *)malloc((width)*(height) * sizeof(int));
	int max_len = 0;
	int max_xy = height*width;
	for (int temp1 = 0; temp1 < max_xy; temp1++) {
		if (bin_th >= grayImg.data[temp1]) {
			thresholded.data[temp1] = 0;
			((int*)labelImg.data)[temp1] = 0;
		}
		else {
			thresholded.data[temp1] = 1;
			xy[max_len] = temp1;
			max_len++;
		}
	}
	//背景色是0
	int m = 1;
		for (int i= 0; i < max_len; i++) {
			int temp1 = xy[i];
				int a = (thresholded.data[temp1 - 1]);
				int b = (thresholded.data[temp1 - width - 1]);
				int c = (thresholded.data[temp1 - width]);
				int d = (thresholded.data[temp1 + 1 - width]);
				int binary_sum = 8 * d + 4 * c + 2 * b + a;
				int temp = temp1;
				switch (binary_sum)
				{
				case 0: {
					T[m] = m;
					((int*)labelImg.data)[temp] = m;
					m++;
				}
						break;
				case 1:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - 1]; break;
				case 2:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - 1 - width]; break;
				case 3:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - 1]; break;
				case 4:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width]; break;
				case 5:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width]; break;
				case 6:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width]; break;
				case 7:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width]; break;
				case 8:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width + 1]; break;
				case 12:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width + 1]; break;
				case 13:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width + 1]; break;
				case 14:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width + 1]; break;
				case 15:((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width + 1]; break;
				case 9: {int label1 = ((int*)labelImg.data)[temp - 1];
					int label2 = ((int*)labelImg.data)[temp + 1 - width];
					int maximum = 0;
					int minimum = 0;
					if (T[label1] > T[label2]) {
						maximum = T[label1];
						minimum = T[label2];
					}
					else {
						maximum = T[label2];
						minimum = T[label1];
					}
					for (int i = 1; i <= m; i++)
					{
						if (T[i] == maximum) T[i] = minimum;
					}
					((int*)labelImg.data)[temp] = minimum;
				}
						break;
				case 11: {int label1 = ((int*)labelImg.data)[temp - 1];
					int label2 = ((int*)labelImg.data)[temp + 1 - width];
					int maximum = 0;
					int minimum = 0;
					if (T[label1] > T[label2]) {
						maximum = T[label1];
						minimum = T[label2];
					}
					else {
						maximum = T[label2];
						minimum = T[label1];
					}
					for (int i = 1; i <= m; i++)
					{
						if (T[i] == maximum) T[i] = minimum;
					}
					((int*)labelImg.data)[temp] = minimum;
				}
						 break;
				case 10: {int label1 = ((int*)labelImg.data)[temp - 1 - width];
					int label2 = ((int*)labelImg.data)[temp + 1 - width];
					int maximum = 0;
					int minimum = 0;
					if (T[label1] > T[label2]) {
						maximum = T[label1];
						minimum = T[label2];
					}
					else {
						maximum = T[label2];
						minimum = T[label1];
					}
					for (int i = 1; i <= m; i++)
					{
						if (T[i] == maximum) T[i] = minimum;
					}
					((int*)labelImg.data)[temp] = minimum;
				}
						 break;
				}
		}
		for (int j = 0; j < max_len; j++)
		{
			int temp = xy[j];
			int label = ((int*)labelImg.data)[temp];
			((int*)labelImg.data)[temp] = T[label];
		}
		free(T);
		free(xy);
	return MY_OK;
}
