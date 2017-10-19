#include “ConnectedComponentLabeling.h”
#define min_ab(x,y)	((x)<(y)?(x):(y))
#define max_ab(x,y)	((x)>(y)?(x):(y))
int ustc_ConnectedComponentLabeling(
	Mat grayImg,
	Mat& labelImg,
	int bin_th)
{
	if (NULL == grayImg.data)
	{
		cout << "image is NULL." << endl;
		return MY_FAIL;
	}
	if (grayImg.channels() != 1)
	{
		cout << "image is bgr not gray." << endl;
		return MY_FAIL;
	}
	if (NULL == labelImg.data)
	{
		cout << "image is NULL." << endl;
		return MY_FAIL;
	}
	if (labelImg.channels() != 1)
	{
		cout << "image is bgr not gray." << endl;
		return MY_FAIL;
	}
	if (grayImg.rows != labelImg.rows || grayImg.cols != labelImg.cols)
	{
		cout << "not match" << endl;
		return MY_FAIL;
	}
	int height = grayImg.rows;
	int width = grayImg.cols;
	int t[10000];
	int min_tbx;
	int min_bx;
	int m = 0;
	int bin[256];
	for (int i = 0; i <= 255; i++)
	{
		if (i < bin_th)
		{
			bin[i] = 0;
		}
		else
		{
			bin[i] = 255;
		}
	}
	int index1 = 0;
	index1 = width*height - 1;
	for (int index = 0; index <= index1; index++)
	{
		grayImg.data[index] = bin[grayImg.data[index]];
	}
	
	index1 = width - 1;
	for (int index = 0; index <= index1; index++)
	{
		labelImg.data[index] = 0;
	}
	index1 = grayImg.cols*grayImg.rows - 1;
	for (int index = index1-width+1; index <= index1; index++)
	{
		labelImg.data[index] = 0;
	}
	for (int index = 0; index <= height - 1; index++)
	{
		labelImg.data[index*width] = 0;
	}
	for (int index = 0; index <= height - 1; index++)
	{
		labelImg.data[index*width+width-1] = 0;
	}
	for (int row = 1; row < height - 1; row++)
	{
		for (int col = 1; col < width - 1; col++)
		{
			int rcw = row*width + col;
			int b1 = labelImg.data[rcw - width - 1];
			int b2 = labelImg.data[rcw - width];
			int b3 = labelImg.data[rcw - width + 1];
			int b4 = labelImg.data[rcw - 1];
			int b = grayImg.data[rcw];
			if (b == 0)
			{
				labelImg.data[rcw] = 0;
				continue;
			}
			int casen = (!(!b1)) * 8 + (!(!b2)) * 4 + (!(!b3)) * 2 + (!(!b4));
			switch (casen)
			{
			case 0:
				m++;
				labelImg.data[rcw] = m;
				t[m] = m;
				continue;
				break;
			case 1:
				labelImg.data[rcw] = b4;
				break;
			case 2:
				labelImg.data[rcw] = b3;
				break;
			case 4:
				labelImg.data[rcw] = b2;
				break;
			case 8:
				labelImg.data[rcw] = b1;
				break;
			case 3:
				labelImg.data[rcw] = min_ab(b4, b3);
				t[max_ab(b3, b4)] = t[min_ab(b3, b4)];
				break;
			case 5:
				labelImg.data[rcw] = min_ab(b4, b2);
				t[max_ab(b2, b4)] = t[min_ab(b2, b4)];
				break;
			case 6:
				labelImg.data[rcw] = min_ab(b2, b3);
				t[max_ab(b2, b3)] = t[min_ab(b3, b2)];
				break;
			case 9:
				labelImg.data[rcw] = min_ab(b1, b4);
				t[max_ab(b1, b4)] = t[min_ab(b1, b4)];
				break;
			case 10:
				labelImg.data[rcw] = min_ab(b1, b3);
				t[max_ab(b1, b3)] = t[min_ab(b1, b3)];
				break;
			case 12:
				labelImg.data[rcw] = min_ab(b2, b1);
				t[max_ab(b2, b1)] = t[min_ab(b1, b2)];
				break;
			case 7:
				min_bx = min_ab(b3, b4);
				min_bx = min_ab(b2, min_bx);
				t[b4] = t[min_bx];
				t[b3] = t[min_bx];
				t[b2] = t[min_bx];
				labelImg.data[rcw] = min_bx;
				break;
			case 11:
				min_bx = min_ab(b3, b4);
				min_bx = min_ab(b1, min_bx);
				t[b4] = t[min_bx];
				t[b3] = t[min_bx];
				t[b1] = t[min_bx];
				labelImg.data[rcw] = min_bx;
				break;
			case 13:
				min_bx = min_ab(b2, b4);
				min_bx = min_ab(b1, min_bx);
				t[b4] = t[min_bx];
				t[b1] = t[min_bx];
				t[b2] = t[min_bx];
				labelImg.data[rcw] = min_bx;
				break;
			case 14:
				min_bx = min_ab(b3, b2);
				min_bx = min_ab(b1, min_bx);
				t[b1] = t[min_bx];
				t[b3] = t[min_bx];
				t[b2] = t[min_bx];
				labelImg.data[rcw] = min_bx;
				break;
			case 15:
				min_bx = min_ab(b3, b4);
				min_bx = min_ab(b2, min_bx);
				min_bx = min_ab(b1, min_bx);
				t[b1] = t[min_bx];
				t[b3] = t[min_bx];
				t[b2] = t[min_bx];
				t[b4] = t[min_bx];
				labelImg.data[rcw] = min_bx;
				break;
			}


		}

	}
	int num = 0;
	int n[10000];
	for (int i = 1; i <= m; i++)
	{
		if (t[i] == i)
		{
			num++;
			n[i] = num;
		}
		else
		{
			t[i] = t[t[i]];
		}
	}
	for (int i = 1; i <= m; i++)
	{
		t[i] = n[t[i]];

	}
	n[0] = 0;
	t[0] = 0;
	index1 = labelImg.cols*labelImg.rows - 1;
	for (int index = 0; index <= index1; index++)
	{
		labelImg.data[index] = t[labelImg.data[index]];
	}

	/*for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			printf("%d ", labelImg.data[row*width + col]);
				//grayImg.data[row*width + col] = 255;

		}
		printf("\n");
	}
	getchar();*/
	/*for (int row = 1; row < height - 1; row++)
	{
		for (int col = 1; col < width - 1; col++)
		{
			if (labelImg.data[row*width + col])
			{
				labelImg.data[row*width + col] = labelImg.data[row*width + col] * 80;
			}
		}
	}
	imshow("resoult", labelImg);
	waitKey(0);*/
	return MY_OK;
}
