#include "ConnectedComponentLabeling.h"
#define MAX_VALUE 255
#define MAX_NUMBER 100000

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
	int width = grayImg.cols;
	int height = grayImg.rows;
	Mat binaryImg(height, width, CV_8UC1);
	threshold(grayImg, binaryImg, bin_th - 1, MAX_VALUE, CV_THRESH_BINARY);

	Mat testImg(height + 2, width + 2, CV_8UC1, Scalar(0));
	Mat imageROI = testImg(Rect(1, 1, width, height));
	binaryImg.copyTo(imageROI);


	Mat labelImg_copy(height + 2, width + 2, CV_32SC1, Scalar(0));
	int label[MAX_NUMBER] = { 0 };
	int label_count = 0;
	int result[MAX_NUMBER] = { 0 };
	for (int row_i = 1; row_i <= height; row_i++)
	{
		int temp0 = row_i*width;
		for (int col_j = 1; col_j <= width; col_j++)
		{
			int temp1 = temp0 + col_j;
			int light = testImg.data[temp1];
			if (0 == light)
			{
				continue;
			}
			int a = ((int *)labelImg_copy.data)[temp1 - width - 1];
			int b = ((int *)labelImg_copy.data)[temp1 - width];
			int c = ((int *)labelImg_copy.data)[temp1 - width + 1];
			int d = ((int *)labelImg_copy.data)[temp1 - 1];
			int e = ((int *)labelImg_copy.data)[temp1];
			int a_judge = (a > 0) ? 1 : 0;
			int b_judge = (b > 0) ? 1 : 0;
			int c_judge = (c > 0) ? 1 : 0;
			int d_judge = (d > 0) ? 1 : 0;
			int type = a_judge * 8 + b_judge * 4 + c_judge * 2 + d_judge;
			switch (type)
			{
			case 0:
				label_count++;
				label[label_count] = label_count;
				((int *)labelImg_copy.data)[temp1] = label_count;
				break;

			case 1:
				((int *)labelImg_copy.data)[temp1] = d;
				break;
			case 2:
				((int *)labelImg_copy.data)[temp1] = c;
				break;
			case 4:
				((int *)labelImg_copy.data)[temp1] = b;
				break;
			case 8:
				((int *)labelImg_copy.data)[temp1] = a;
				break;

			default:
				int maxer = a;
				int miner = a;
				if (a < b)
				{
					maxer = b;
				}
				if (maxer < c)
				{
					maxer = c;
				}
				if (maxer < d)
				{
					maxer = d;
				}

				if ((miner == 0 || a > b) && b != 0)
				{
					miner = b;
				}
				if ((miner == 0 || miner > c) && c != 0)
				{
					miner = c;
				}
				if ((miner == 0 || miner > d) && d != 0)
				{
					miner = d;
				}
				if (maxer == miner)
				{
					((int *)labelImg_copy.data)[temp1] = miner;
				}
				else if (miner != maxer)
				{
					((int *)labelImg_copy.data)[temp1] = miner;
					if (result[maxer] == 0)
					{
						result[maxer] = (result[miner] == 0) ? miner : result[miner];
						for (int temp_k = maxer + 1; temp_k <= label_count; temp_k++)
						{
							if (result[temp_k] == maxer)
							{
								result[temp_k] = result[maxer];
							}
						}

					}
					else if (result[maxer] != 0 && result[maxer] != miner)
					{
						int minest = result[maxer];
						if (miner < minest)
						{
							int temp = miner;
							miner = minest;
							minest = temp;
						}
						
						result[miner] = (result[minest] == 0) ? minest : result[minest];
						for (int temp_k = miner + 1; temp_k <= label_count; temp_k++)
						{
							if (result[temp_k] == miner)
							{
								result[temp_k] = result[miner];
							}

						}
					}
				}
				break;

			}
		}
	}

	for (int row_i = 1; row_i <= height; row_i++)
	{
		int temp0 = row_i*width;

		for (int col_j = 1; col_j <= width; col_j++)
		{
			int temp1 = temp0 + col_j;
			int currlabel = ((int *)labelImg_copy.data)[temp1];
			int newlabel = result[currlabel];
			((int *)labelImg_copy.data)[temp1] = newlabel ? newlabel : currlabel;
		}
	}

	Mat finalimageROI = labelImg_copy(Rect(1, 1, width, height));
	finalimageROI.copyTo(labelImg);

	return MY_OK;
}
