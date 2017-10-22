#include "ConnectedComponentLabeling.h"
//////全局变量////////
int T[100000] = { 0 };
Mat labelImgplus;
//////递归函数////////
void Insert(int big, int small)
{
	if (T[big] == small)return;
	while (T[big] > small) {
		if (T[big] == big) {
			T[big] = small;
			return;
		}
		big = T[big];
	}
	int next = T[big];
	T[big] = small;
	Insert(small, next);
}

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
	if (NULL == grayImg.data)
	{
		cout << "image is NULL." << endl;
		return MY_FAIL;
	}

	if (grayImg.channels() != 1)
	{
		cout << "channels not match" << endl;
		return MY_FAIL;
	}

	///////create labelImg & binaryImg/////////////
	int height = grayImg.rows;
	int width = grayImg.cols;
	labelImg.create(height, width, CV_32SC1);
	labelImgplus.create(height + 2, width + 2, CV_32SC1);


	//打一个二值化的表
	int* bi_array = new int[256];
	memset(bi_array, 0, sizeof(int) * 256);
	for (int ii = bin_th; ii < 256; ii++)
	{
		bi_array[ii] = 255;
	}

	//////////////值赋Imgplus//////////////
	int heightPlus = 1 + height + 1;
	int widthPlus = 1 + width + 1;
	int *ImgPlus = (int*)labelImgplus.data;
	int total = (heightPlus - 1)*widthPlus;


	//////////////最外围赋值零//////////////
	for (int i = 0; i != widthPlus; i++)
	{
		ImgPlus[i] = 0;
		ImgPlus[total + i] = 0;
	}
	int widPlusminus1 = widthPlus - 1;
	int heiPlusminus1 = heightPlus - 1;
	for (int row = 1; row != heightPlus; row++)
	{
		int temp1 = (row)*widthPlus;
		ImgPlus[temp1] = 0;
		ImgPlus[temp1 + widPlusminus1] = 0;
	}




	//////////////first scan//////////////
	int m = 1;
	uchar *graydata = grayImg.data;
	int p, q, maxTpq;
	uchar* pr = graydata;
	for (int row = 1; row != heiPlusminus1; row++)
	{
		//e的行指针
		int* data_e = (int*)labelImgplus.ptr(row);
		//a的行指针
		int* data_a = (int*)labelImgplus.ptr(row - 1);
		for (int col = 1; col != widPlusminus1; col++)
		{
			// a b c
			// d e 
			//data[e] = bi_array[*(pr)];
			if (data_e[col] =*(bi_array + *pr))
			{
				int d = data_e[col-1];
				int b = data_a[col];
				int a = data_a[col-1];
				int c = data_a[col+1];
				int option = 8 * !(!a) + 4 * !(!b) + 2 * !(!c) + !(!d);

				switch (option)
				{
				case 0:
					data_e[col] = m;
					T[m] = m;
					m++;
					break;
				case 1:
					data_e[col] = T[d];
					break;
				case 2:
					data_e[col] = T[c];
					break;
				case 3:
					p = c;
					q = d;
					data_e[col] = T[p] > T[q] ? T[q] : T[p];
					maxTpq = T[p] > T[q] ? p : q;
					Insert(maxTpq, data_e[col]);
					break;
				case 4:
					data_e[col] = T[b];
					break;
				case 5:
					data_e[col] = T[d];
					break;
				case 6:
					data_e[col] = T[c];
					break;
				case 7:
					p = c;
					q = d;
					data_e[col] = T[p] > T[q] ? T[q] : T[p];
					maxTpq = T[p] > T[q] ? p : q;
					Insert(maxTpq, data_e[col]);
					break;
				case 8:
					data_e[col] = T[a];
					break;
				case 9:
					data_e[col] = T[d];
					break;
				case 10:
					p = a;
					q = c;
					data_e[col] = T[p] > T[q] ? T[q] : T[p];
					maxTpq = T[p] > T[q] ? p : q;
					Insert(maxTpq, data_e[col]);
					break;
				case 11:
					p = c;
					q = d;
					data_e[col] = T[p] > T[q] ? T[q] : T[p];
					maxTpq = T[p] > T[q] ? p : q;
					Insert(maxTpq, data_e[col]);
					break;
				case 12:
					data_e[col] = T[b];
					break;
				case 13:
					data_e[col] = T[d];
					break;
				case 14:
					data_e[col] = T[c];
					break;
				case 15:
					p = c;
					q = d;
					data_e[col] = T[p] > T[q] ? T[q] : T[p];
					maxTpq = T[p] > T[q] ? p : q;
					Insert(maxTpq, data_e[col]);
				}
			}
			pr++;
		}
	}

	//递归更新T表
	for (int beg = 1; beg != m; beg++)
	{
		int temp = beg;
		while (T[temp] != temp)
		{
			temp = T[temp];
		}
		T[beg] = temp;
	}

	//////////////////second scan/////////////////// 
	for (int ii = 0; ii < height; ii++)
	{
		int* Mi = (int *)labelImgplus.ptr(ii + 1);
		int* Si = (int*)labelImg.ptr(ii);
		for (int jj = 0; jj < width; jj++)
		{
			Si[jj] = T[Mi[jj + 1]];
		}
	}
}


