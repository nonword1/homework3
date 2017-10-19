#include “ConnectedComponentLabeling.h”
int ustc_ConnectedComponentLabeling(Mat grayImg, Mat& lablelImg, int bin_th) {
	int row = grayImg.rows;
	int col = grayImg.cols;
	int m;

	if (NULL == grayImg.data)
	{
		printf("grayImg error\n");
		return MY_FAIL;
	}

	Mat grayImg_e;
	grayImg_e.create(row + 2, col + 2, CV_8UC1);
	grayImg_e.setTo(0);

	labelImg.create(row, col, CV_32SC1);
	labelImg.setTo(0);

	Mat imgROI = grayImg_e(Rect(1, 1, col, row));
	grayImg.convertTo(imgROI, CV_8UC1);

	Mat binaryImg;
	threshold(grayImg_e, binaryImg, bin_th, 1, THRESH_BINARY);

	Mat g;
	g.create(row + 2, col + 2, CV_32SC1);


	int tablesize = row*col / 4;
	int* rtable = (int*)malloc(tablesize);
	int* next = (int*)malloc(tablesize);
	int* tail = (int*)malloc(tablesize);
	g.setTo(0);
	memset(rtable, 0, tablesize);
	memset(next, 0, tablesize);
	memset(tail, 0, tablesize);
	m = 1;
	//first scan
	for (int i = 1; i <= row; i++)
	{
		uchar* row_down = binaryImg.ptr<uchar>(i);
		int* gData_up = g.ptr<int>(i - 1);
		int* gData_down = g.ptr<int>(i);
		for (int j = 1; j <= col; j++)
		{
			if (row_down[j] != 0)//x,y
			{
				int c3 = gData_up[j]; //x-1,y
				if (c3)
					gData_down[j] = c3;
				else
				{
					int c1 = gData_down[j - 1];
					if (c1)
					{
						gData_down[j] = c1;
						int c4 = gData_up[j + 1]; //x-1,y+1
						if (c4)
						{
							int u = rtable[c1]; int v = rtable[c4];
							if (u < v)
							{
								int k = v;
								while (k != -1)
								{
									rtable[k] = u;
									k = next[k];
								}
								next[tail[u]] = v;
								tail[u] = tail[v];
							}
							else if (u > v)
							{
								int k = u;
								while (k != -1)
								{
									rtable[k] = v;
									k = next[k];
								}
								next[tail[v]] = u;
								tail[v] = tail[u];
							}
						}
					}
					else
					{
						int c2 = gData_up[j - 1];  //x-1,y-1
						if (c2)
						{
							gData_down[j] = c2;
							int c4 = gData_up[j + 1]; //x-1,y+1
							if (c4)
							{
								int u = rtable[c2]; int v = rtable[c4];
								if (u < v)
								{
									int k = v;
									while (k != -1)
									{
										rtable[k] = u;
										k = next[k];
									}
									next[tail[u]] = v;
									tail[u] = tail[v];
								}
								else if (u > v)
								{
									int k = u;
									while (k != -1)
									{
										rtable[k] = v;
										k = next[k];
									}
									next[tail[v]] = u;
									tail[v] = tail[u];
								}
							}
						}
						else
						{
							int c4 = gData_up[j + 1]; //x-1,y+1
							if (c4)
								gData_down[j] = c4;
							else {
								gData_down[j] = m;
								rtable[m] = m;
								next[m] = -1;
								tail[m] = m;
								m++;
							}
						}
					}
				}
			}
		}
	}
	//second scan
	for (int i = 1; i <= row; i++)
	{
		int* gData = g.ptr<int>(i);
		int* dstData = labelImg.ptr<int>(i - 1);
		int j;
		int jend = col - 8;
		for (j = 1; j <= jend; j += 8)
		{
			dstData[j - 1] = rtable[gData[j]];
			dstData[j] = rtable[gData[j + 1]];
			dstData[j + 1] = rtable[gData[j + 2]];
			dstData[j + 2] = rtable[gData[j + 3]];
			dstData[j + 3] = rtable[gData[j + 4]];
			dstData[j + 4] = rtable[gData[j + 5]];
			dstData[j + 5] = rtable[gData[j + 6]];
			dstData[j + 6] = rtable[gData[j + 7]];
		}
		j -= 8;
		for (; j <= col; j++)
		{
			dstData[j - 1] = rtable[gData[j]];
		}
	}
	free(rtable);
	free(next);
	free(tail);

	return MY_OK;

}
