
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  

#include <math.h>
#include <time.h>
//#include "SubImageMatch.h"

int *table;
int length;

#define IMG_SHOW
#define OK 1
#define FAIL 0
#define ITER 1

int ustc_Threshold(Mat grayImg, Mat& binaryImg, int Threshold) {
	if (NULL == grayImg.data || NULL == binaryImg.data)
	{
		cout << "image is NULL." << endl;
		return FAIL;
	}

	int width = binaryImg.cols;
	int height = binaryImg.rows;
	int gw = grayImg.cols;
	int gh = grayImg.rows;
	if (gw != width || gh != height)
	{
		cout << "image size is wrong." << endl;
		return FAIL;
	}
	uchar* grayPtr = grayImg.data;
	uchar* binaryPtr = binaryImg.data;
	int temp = 0;
	for (int row_i = 0; row_i < height; row_i++)
	{
		for (int col_j = 0; col_j < width; col_j++)
		{
			
			*binaryPtr = (Threshold - 1 - *grayPtr) >> 31;//0 or 255
			grayPtr++;
			binaryPtr++;
		}
	}
	return OK;

}

int Connect_Update(int a, int b) {
	int con_a = table[a];
	int con_b = table[b];
	if (con_a > con_b) {
		int temp = con_a;
		con_a = con_b;
		con_b = temp;
	}
	for (int i =con_b; i <= length; i++) 
	{
		if (table[i] == con_b) {
			table[i] = con_a;
		}
	}
	return min(a, b);
}

int First_Traverse(Mat BinaryImg, Mat &LabelImg) { 
	if (NULL == BinaryImg.data || NULL == LabelImg.data)
	{
		cout << "image read failed." << endl;
		return FAIL;
	}
	

	int width = BinaryImg.cols;
	int height = BinaryImg.rows;
	int label_width = LabelImg.cols;
	int label_height = LabelImg.rows;
	if (height != label_height || width != label_width)
	{
		cout << "image size wrong." << endl;
		return FAIL;
	}

	length = 0;
	memset(table, 0, width*height * sizeof(int));
	LabelImg.setTo(0);

	uchar* binptr = BinaryImg.data;
	float* labelptr = (float*)LabelImg.data;
	//length += (*binptr & 1);
	//table[(*binptr & 1)] = (*binptr & 1);
	if (*binptr) //first pixel
	{
		table[1] = 1;
		length = 1;
		*labelptr = length;
	}
	binptr++; labelptr++;
	for (int j = 1; j < width; j++) //first row without first pixel
	{
		switch ((*binptr & 2) + (*(binptr - 1) & 1))
		{
			case 0:	break;
			case 1:	break;
			case 2: 
			{
				length++;
				table[length] = length;
				*labelptr = length;
				break;
			}
			case 3: *labelptr = *(labelptr - 1);
		}
		binptr++; labelptr++;
	}
	
	for (int i = 1; i < height; i++)
	{
		switch ((*binptr & 4) + (*(binptr - width) & 2) + (*(binptr - width + 1) & 1)) 
			//	2	1
			//	4
		{
			case 0: break;
			case 1: break;
			case 2: break;
			case 3: break;
			case 4:				
			{
				length++;
				table[length] = length;
				*labelptr = length;
				break;
			}
			case 5:
			{
				*labelptr = *(labelptr - width + 1);
				break;
			}
			case 6:
			case 7:
			{
				*labelptr = *(labelptr - width);
				break;
			}

		}
		labelptr++; binptr++;
		for (int j = 1; j < width-1; j++)
		{
			switch ((*binptr & 16) + (*(binptr - 1) & 8) + (*(binptr - width - 1) & 4) + (*(binptr - width) & 2) + (*(binptr - width + 1) & 1))
				//	4	2	1	
				//	8	16
			{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15: break;
				case 16: 
				{
					length++;
					table[length] = length;
					*labelptr = length;
					break;
				}
				case 17: 
				{
					*labelptr = *(labelptr - width + 1);
					break;
				}
				case 18:
				case 19:
				{
					*labelptr = *(labelptr - width);
					break;
				}
				case 20:
				{
					*labelptr = *(labelptr - width - 1);
					break;
				}
				case 21:
				{
					*labelptr = Connect_Update(*(labelptr - width - 1), *(labelptr - width + 1));
					break;
				}
				case 22:
				case 23:
				{
					*labelptr = *(labelptr - width - 1);
					break;
				}
				case 24:
				{
					*labelptr = *(labelptr - 1);
					break;
				}
				case 25:
				{
					*labelptr = Connect_Update(*(labelptr - 1), *(labelptr - width + 1));
					break;
				}
				case 26:
				case 27:
				case 28:
				{
					*labelptr = *(labelptr - 1);
					break;
				}
				case 29:
				{
					*labelptr = *labelptr = Connect_Update(*(labelptr - 1), *(labelptr - width + 1));
					break;
				}
				case 30:
				case 31:
				{
					*labelptr = *(labelptr - 1);
					break;
				}
			}
			labelptr++; binptr++;
		}
		switch ((*binptr & 8) + (*(binptr - 1) & 4) + (*(binptr - width - 1) & 2) + (*(binptr - width) & 1))
			//	2	1
			//	4	8
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:break;
			case 8:
			{
				length++;
				table[length] = length;
				*labelptr = length;
				break;
			}
			case 9:
			{
				*labelptr = *(labelptr - width);
				break;
			}
			case 10:
			case 11:
			{
				*labelptr = *(labelptr - width - 1);
				break;
			}
			case 12:
			case 13:
			case 14:
			case 15:
			{
				*labelptr = *(labelptr - 1);
				break;
			}
		}
		labelptr++; binptr++;
	}
	

	return OK;
}

int Second_Traverse(Mat &LabelImg) {
	if ( NULL == LabelImg.data)
	{
		cout << "image read failed." << endl;
		return FAIL;
	}
	float* ptr = (float*)LabelImg.data;
	int width = LabelImg.cols;
	int height = LabelImg.rows;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++) {
			*ptr = table[(int)*ptr];
			ptr++;
		}
	}
	
	return OK;
}

int ustc_ConnectedComponentLabeling(
	Mat grayImg,
	Mat& labelImg,
	int bin_th);


int ustc_ConnectedComponentLabeling(
	Mat grayImg,
	Mat& labelImg,
	int bin_th) 
{
	if (NULL == grayImg.data|| NULL == labelImg.data)
	{
		cout << "image read failed." << endl;
		return FAIL;
	}

	int width = grayImg.cols;
	int height = grayImg.rows;
	int label_width = labelImg.cols;
	int label_height = labelImg.rows;
	if (height != label_height || width != label_width) 
	{
		cout << "image size wrong." << endl;
		return FAIL;
	}
	if (bin_th < 0 || bin_th>255) {
		cout << "threshold invalid." << endl;
		return FAIL;
	}
	Mat BinaryImg(height, width, CV_8UC1);
	
	table=(int*)malloc(width*height * sizeof(int));
	
	length=0;
	
	int flag = ustc_Threshold(grayImg, BinaryImg, bin_th);

	 flag = First_Traverse(BinaryImg, labelImg);

	 flag = Second_Traverse(labelImg);

	return OK;
}
