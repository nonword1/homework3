#include "ConnectedComponentLabeling.h"

////////连通域检测/////////
//功能说明：找到二值图像中所有的连通域，并且进行标记
//grayImg：用来进行连通域分析的目标图，8UC1
//labelImg：用来存储标记结果的图像，CV_32SC1，尺寸与grayImg一致
//bin_th：用来对灰度图进行二值化的阈值
//返回值：MY_OK或者MY_FAIL
int ustc_ConnectedComponentLabeling(
	Mat grayImg,
	Mat& labelImg,
	int bin_th)
{
	if (!grayImg.data || !labelImg.data)
	{
		cout << "image is NULL." << endl;
		return MY_FAIL;
	}
	if (grayImg.channels() != 1 || labelImg.channels() != 1)
	{
		cout << "channels wrong!" << endl;
		return MY_FAIL;
	}
	if (grayImg.depth() != CV_8U || labelImg.depth() != CV_32S)
	{
		cout << "depth wrong!" << endl;
		return MY_FAIL;
	}
	if (grayImg.rows != labelImg.rows || grayImg.cols != labelImg.cols)
	{
		cout << "size not match" << endl;
		return MY_FAIL;
	}

	int m = 1;   //labelImg的标签的值

	int width = grayImg.cols, height = grayImg.rows;
	int u, v, j;
	int T[10000] = { 0 };    //Table数组
	int Next[10000] = { 0 }, tail[10000] = { 0 };
	Mat binaryImg(height, width, CV_8UC1);               //定义二值图
	threshold(grayImg, binaryImg, bin_th, 1, THRESH_BINARY);        //灰度图二值化
	labelImg.setTo(0);
	//c4=b(x+1,y-1),c3=b(x,y-1),c2=b(x-1,y-1),c1=b(x-1,y)
	//****************
	//** c2  c3  c4 **
	//** c1   x     **
	//****************

	//First scan
	((int*)labelImg.data)[0] = binaryImg.data[0];
	for (int x = 1; x < width; x++)
	{
		if (binaryImg.data[x])
		{
			int mode = binaryImg.data[x - 1];
			switch (mode)
			{
			case 0:
				((int*)labelImg.data)[x] = m;
				T[m] = m;
				Next[m] = -1;
				tail[m] = m;
				m++;
				break;
			case 1:
				((int*)labelImg.data)[x] = ((int*)labelImg.data)[x - 1];
				break;
			}
		}
	}

	for (int y = 1; y < height; y++)
	{
		int temp = y*width;
		if (binaryImg.data[temp])
		{
			int mode0 = 2 * binaryImg.data[temp - width + 1] + binaryImg.data[temp - width];
			switch (mode0)
			{
			case 0:
				((int*)labelImg.data)[temp] = m;
				T[m] = m;
				Next[m] = -1;
				tail[m] = m;
				m++;
				break;
			case 1:
				((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width];
				break;
			case 2:
			case 3:
				((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width + 1];
				break;
			}
		}
		for (int x = 1; x < width - 1; x++)
		{
			if (binaryImg.data[temp + x])
			{
				//计算mode,mode=8*c4+4*c3+2*c2+c1
				int index = temp + x;
				int mode = 8 * binaryImg.data[index - width + 1]
					+ 4 * binaryImg.data[index - width]
					+ 2 * binaryImg.data[index - width - 1]
					+ binaryImg.data[index - 1];

				switch (mode)
				{
				case 0:
					((int*)labelImg.data)[index] = m;
					T[m] = m;
					Next[m] = -1;
					tail[m] = m;
					m++;
					break;
				case 8:
				case 12:
				case 13:
				case 14:
				case 15:
					//取c4
					((int*)labelImg.data)[index] = ((int*)labelImg.data)[index - width + 1];
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					//取c3
					((int*)labelImg.data)[index] = ((int*)labelImg.data)[index - width];
					break;
				case 2:
				case 3:
					//取c2
					((int*)labelImg.data)[index] = ((int*)labelImg.data)[index - width - 1];
					break;
				case 1:
					//取c1
					((int*)labelImg.data)[index] = ((int*)labelImg.data)[index - 1];
					break;

				case 9:
					//取c4
					((int*)labelImg.data)[index] = ((int*)labelImg.data)[index - width + 1];
					u = T[((int*)labelImg.data)[index - width + 1]], v = T[((int*)labelImg.data)[index - 1]];
					if (u < v)
					{
						j = v;
						while (j != -1)
						{
							T[j] = u;
							j = Next[j];
						}
						Next[tail[u]] = v;
						tail[u] = tail[v];
					}
					else if (u > v)
					{
						j = u;
						while (j != -1)
						{
							T[j] = v;
							j = Next[j];
						}
						Next[tail[v]] = u;
						tail[v] = tail[u];
					}
					break;
				case 10:
					//取c4
					((int*)labelImg.data)[index] = ((int*)labelImg.data)[index - width + 1];
					u = T[((int*)labelImg.data)[index - width + 1]], v = T[((int*)labelImg.data)[index - width - 1]];
					if (u < v)
					{
						j = v;
						while (j != -1)
						{
							T[j] = u;
							j = Next[j];
						}
						Next[tail[u]] = v;
						tail[u] = tail[v];
					}
					else if (u > v)
					{
						j = u;
						while (j != -1)
						{
							T[j] = v;
							j = Next[j];
						}
						Next[tail[v]] = u;
						tail[v] = tail[u];
					}
					break;
				case 11:
					//取c4
					((int*)labelImg.data)[index] = ((int*)labelImg.data)[index - width + 1];
					u = T[((int*)labelImg.data)[index - width + 1]], v = T[((int*)labelImg.data)[index - 1]];
					if (u < v)
					{
						j = v;
						while (j != -1)
						{
							T[j] = u;
							j = Next[j];
						}
						Next[tail[u]] = v;
						tail[u] = tail[v];
					}
					else if (u > v)
					{
						j = u;
						while (j != -1)
						{
							T[j] = v;
							j = Next[j];
						}
						Next[tail[v]] = u;
						tail[v] = tail[u];
					}
					break;

				}
			}
		}
		temp = (y + 1)*width - 1;
		if (binaryImg.data[temp])
		{
			int mode1 = 4 * binaryImg.data[temp - width] + 2 * binaryImg.data[temp - width - 1] + binaryImg.data[temp - 1];
			switch (mode1)
			{
			case 0:
				((int*)labelImg.data)[temp] = m;
				T[m] = m;
				Next[m] = -1;
				tail[m] = m;
				m++;
				break;
			case 1:
				((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - 1];
				break;
			case 2:
			case 3:
				((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width - 1];
				break;
			case 4:
			case 5:
			case 6:
			case 7:
				((int*)labelImg.data)[temp] = ((int*)labelImg.data)[temp - width];
				break;
			}
		}
	}

	//Second scan
	int xx;
	for (int y = 0; y < height; y++)
	{
		for (xx = 0; xx < width - 7; xx += 8)
		{
			int index = y*width + xx;
			((int*)labelImg.data)[index] = T[((int*)labelImg.data)[index]];
			((int*)labelImg.data)[index + 1] = T[((int*)labelImg.data)[index + 1]];
			((int*)labelImg.data)[index + 2] = T[((int*)labelImg.data)[index + 2]];
			((int*)labelImg.data)[index + 3] = T[((int*)labelImg.data)[index + 3]];
			((int*)labelImg.data)[index + 4] = T[((int*)labelImg.data)[index + 4]];
			((int*)labelImg.data)[index + 5] = T[((int*)labelImg.data)[index + 5]];
			((int*)labelImg.data)[index + 6] = T[((int*)labelImg.data)[index + 6]];
			((int*)labelImg.data)[index + 7] = T[((int*)labelImg.data)[index + 7]];
		}
		for (; xx< width; xx++)
		{
			((int*)labelImg.data)[y*width + xx] = T[((int*)labelImg.data)[y*width + xx]];
		}
	}
	return MY_OK;
}
