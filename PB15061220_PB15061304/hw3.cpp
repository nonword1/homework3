#include "stdafx.h"
#include"string.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#define VERYBIG 65535
int update_result(int*result, int oldone, int newone, int m) {
	for (int i = 0; i <= m; i++) {
		if (result[i] == oldone) {
			result[i] = newone;
		}
	}
	return MY_OK;
}
int pick_min2(int a, int b) {
	int min;
	if (a > b) {
		min = b;
	}
	else {
		min = a;
	}
	return min;
}
int pick_min3(int a, int b, int c) {
	int min1;
	int min;
	if (a > b) {
		min1 = b;
	}
	else {
		min1 = a;
	}
	if (min1>c) {
		min = c;
	}
	else {
		min = min1;
	}
	return min;
}
int pick_min4(int a, int b, int c, int d) {
	int min1;
	int min2;
	int min;
	if (a > b) {
		min1 = b;
	}
	else {
		min1 = a;
	}
	if (c > d) {
		min2 = d;
	}
	else {
		min2 = c;
	}
	if (min1 > min2) {
		min = min2;
	}
	else {
		min = min1;
	}
	return min;
}

int ustc_ConnectedComponentLabeling(Mat grayImg, Mat& labelImg,int bin_th) {
	if (NULL == grayImg.data || NULL == labelImg.data ||bin_th>255||bin_th<0) {
		cout << "wrong." << endl;
		return MY_FAIL;
	}
	int cols = grayImg.cols;
	int rows = grayImg.rows;
	int lenth = cols*rows / 4;
	int *label = new int[lenth];
	int *result = new int[lenth];
	memset(label, -1, lenth);
	memset(result, -1, lenth);
	
	int k = 0;
	int w = 0;
	
	
	Mat threImg(rows, cols, CV_8UC1);
	threshold(grayImg, threImg, bin_th, 255, THRESH_BINARY);
	
	int new_rows = rows + 2;
	int new_cols = cols + 2;
	Mat testImg(new_rows, new_cols, CV_32SC1);
	testImg.setTo(0);
	int rr = rows + 1;
	int cc = cols + 1;
	int*t_data =(int*)testImg.data;
	uchar*data = threImg.data;
	for (int i = 1; i < rr; i++) {
		int flag1 = i * new_cols;
		int flag1_s = (i - 1)*cols;
		for (int j = 1; j < cc; j++) {
			int flag2 = flag1 + j;
			int flag2_s = flag1_s + j - 1;
			t_data[flag2] = (int)data[flag2_s];
		}
	}
	

	
	int curr_max_label = 0;
	for (int i = 1; i < rr; i++) {
		int flag = (i - 1)*new_cols;
		for (int j = 1; j < cc; j++) {
			int loc_a = flag + j - 1;
			int loc_e = loc_a + new_cols+1;
			int a = t_data[loc_a];
			int b = t_data[loc_a + 1];
			int c = t_data[loc_a + 2];
			int d = t_data[loc_e-1];
			int e = t_data[loc_e];
			switch(e) {
			case 0:break;
			default:
				int bianma = (a &1) * 8 + (b &1) * 4 + (c&1) * 2 + (d &1);
				int flagg = 0;
				int mydefine = 0;
				switch (bianma) {
				case 0:curr_max_label++;
					label[w++] = curr_max_label;
					result[k++] = curr_max_label;
					t_data[loc_e] = curr_max_label;
					break;
				case 1:t_data[loc_e] = d;
					break;
				case 2:t_data[loc_e] = c;
					break;
				case 4:t_data[loc_e] = b;
					break;
				case 8:t_data[loc_e] = a;
					break;
				case 3:flagg = pick_min2(d, c);
					t_data[loc_e] = flagg;
					mydefine = ((d - flagg) || 0)*d + ((c - flagg) || 0)*c;
					update_result(result, mydefine, flagg, k);
					break;
				case 5: flagg = pick_min2(b, d);
					t_data[loc_e] = flagg;
					mydefine = ((b - flagg) || 0)*b + ((d - flagg) || 0)*d;
					update_result(result, mydefine, flagg, k);
					break;
				case 12: flagg = pick_min2(a, b);
					t_data[loc_e] = flagg;
					mydefine = ((a - flagg) || 0)*a + ((b - flagg) || 0)*b;
					update_result(result, mydefine, flagg, k);
					break;
				case 6:flagg = pick_min2(b, c);
					t_data[loc_e] = flagg;
					mydefine = ((b - flagg) || 0)*b + ((c - flagg) || 0)*c;
					update_result(result, mydefine, flagg, k);
					break;
				case 9:flagg = pick_min2(a, d);
					t_data[loc_e] = flagg;
					mydefine = ((a - flagg) || 0)*a + ((d - flagg) || 0)*d;
					update_result(result, mydefine, flagg, k);
					break;
				case 10: flagg = pick_min2(a, c);
					t_data[loc_e] = flagg;
					mydefine = ((a - flagg) || 0)*a + ((c - flagg) || 0)*c;
					update_result(result, mydefine, flagg, k);
					break;
				case 7: flagg = pick_min3(b, c, d);
					t_data[loc_e] = flagg;
				
					if (b == flagg) {
						update_result(result, c, flagg, k);
						update_result(result, d, flagg, k);
					}
					else if (c == flagg) {
						update_result(result, b, flagg, k);
						update_result(result, d, flagg, k);
					}
					else if (d == flagg) {
						update_result(result, b, flagg, k);
						update_result(result, c, flagg, k);
					}
					break;
				case 11: flagg = pick_min3(a, c, d);
					t_data[loc_e] = flagg;
				
					if (a == flagg) {
						update_result(result, c, flagg, k);
						update_result(result, d, flagg, k);
					}
					else if (c == flagg) {
						update_result(result, a, flagg, k);
						update_result(result, d, flagg, k);
					}
					else if (d == flagg) {
						update_result(result, a, flagg, k);
						update_result(result, c, flagg, k);
					}
					break;
				case 13:flagg = pick_min3(b, a, d);
					t_data[loc_e] = flagg;
				
					if (b == flagg) {
						update_result(result, a, flagg, k);
						update_result(result, d, flagg, k);
					}
					else if (a == flagg) {
						update_result(result, b, flagg, k);
						update_result(result, d, flagg, k);
					}
					else if (d == flagg) {
						update_result(result, b, flagg, k);
						update_result(result, a, flagg, k);
					}
					break;
				case 14:flagg = pick_min3(b, c, a);
					t_data[loc_e] = flagg;
				
					if (b == flagg) {
						update_result(result, c, flagg, k);
						update_result(result, a, flagg, k);
					}
					else if (c == flagg) {
						update_result(result, b, flagg, k);
						update_result(result, a, flagg, k);
					}
					else if (a == flagg) {
						update_result(result, b, flagg, k);
						update_result(result, c, flagg, k);
					}
					break;
				case 15: flagg = pick_min4(a, b, c, d);
					t_data[loc_e] = flagg;
				
					if (b == flagg) {
						update_result(result, c, flagg, k);
						update_result(result, d, flagg, k);
						update_result(result, a, flagg, k);
					}
					else if (c == flagg) {
						update_result(result, b, flagg, k);
						update_result(result, d, flagg, k);
						update_result(result, a, flagg, k);
					}
					else if (d == flagg) {
						update_result(result, b, flagg, k);
						update_result(result, c, flagg, k);
						update_result(result, a, flagg, k);
					}
					else if (a == flagg) {
						update_result(result, b, flagg, k);
						update_result(result, c, flagg, k);
						update_result(result, d, flagg, k);
					}
					break;
				}
			}
		}
	}
	for (int i = 0; i < k; i++) {
		int dd = result[i];
		int kk = result[dd - 1];
		if (dd != kk) {
			result[i] = kk;
		}
	}

	for (int i = 1; i < rr; i++) {
		int flag = i*new_cols;
		for (int j = 1; j < cc; j++) {
			int yy = flag + j;
			for (int f = 0; f < w; f++) {
				int dat = t_data[yy];
				int lab = label[f];
				int res = result[f];
				if (dat == lab) {
					t_data[yy] = res;
				}
			}
		}
	}
	int*label_data = (int*)labelImg.data;
	for (int i = 0; i < rows; i++) {
		int flag_a1 = i*cols;
		int flag_a2 = (i + 1)*new_cols;
		for (int j = 0; j < cols; j++) {
			int flag_b1 = flag_a1 + j;
			int flag_b2 = flag_a2 + j + 1;
			int dataa = t_data[flag_b2];
			label_data[flag_b1] = dataa;
		}
	}

	delete label;
	delete result;
	return MY_OK;
}
