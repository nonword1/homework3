#include "core.hpp"
#include "highgui.hpp"
#include "iostream"
#include "hw3.h"
#include "time.h"
using namespace cv;
using namespace std;

#define SHOW_IMG_IN_NUM

int main()
{
	namedWindow("result");
	Mat img = imread("pic5.jpg",0);//读取灰度图
	int row = img.rows;
	int col = img.cols;
	int row_num;
	Mat binaryImg(row, col, CV_8UC1);
	Mat binaryImg2(row, col, CV_8UC1);
	ustc_ConnectedComponentLabeling(img, binaryImg, 80);
	int max = -1;
	int index;
	for (int i = 0; i < row; i++)
	{
		row_num = i * col;
		for (int j = 0; j < col; j++)
		{
			index = row_num + j;
			if (img.data[index] >= 125)
			{
				binaryImg2.data[index] = 200;
			}
			else
			{
				binaryImg2.data[index] = 0;
			}
		}

	}
	for (int i = 0; i < row; i++)
	{
		row_num = i * col;
		for (int j = 1; j <= col; j++)
		{
			index = row_num + j;
			if (max<binaryImg.data[index])
			{
				max = binaryImg.data[index];
			}
		}

	}
	for (int i = 0; i < row; i++)
	{
		row_num = i * col;
		for (int j = 1; j <= col; j++)
		{
			index = row_num + j;
		
				binaryImg.data[index] = binaryImg.data[index] * 255 / max;

			
		}

	}
	imshow("result", binaryImg);
	waitKey();
}

//sub function
void resolve_label(int* labelset, int a, int b, int num)
{
	int min;
	int max;
	if (a > b)
	{
		max = a;
		min = b;
	}
	else
	{
		max = b;
		min = a;
	}
	for (int i = 0;i<=num;i++)
	{
		if (labelset[i] == max)
		{
			labelset[i] = min;
		}
	}

}

//任务函数
int ustc_ConnectedComponentLabeling(Mat grayImg, Mat& labelImg, int bin_th)
{
	if ((bin_th < 0) || (bin_th > 255))
	{
		printf("The threshold must between 0-255");
		return MY_FAIL;
	}
	if (grayImg.data == NULL)
	{
		printf("The gray image is null");
		return MY_FAIL;

	}
	if (labelImg.data == NULL)
	{
		printf("The label image is null");
		return MY_FAIL;

	}
	int i, j;
	int initial_row = grayImg.rows;
	int initial_col = grayImg.cols;
	// !!!Attention the fisrt label is 1 other than 0
	int labelset[MAX_LABEL] = { 0 };
	int label = 1;
	int a, b;
	int row_num;
	int row_num_last;
	int index;
	int c1, c2, c3, c4;
	int temp_label;
	int x, y;
	int x0, x1, x2, x3, x4;
	Mat binaryImg(initial_row, initial_col, CV_8UC1);
	for (i = 0; i < initial_row; i++)
	{
		row_num = i * initial_col;
		for (j = 0; j < initial_col; j++)
		{
			index = row_num + j;
			if (grayImg.data[index] >= bin_th)
			{
				binaryImg.data[index] = 200;
			}
			else
			{
				binaryImg.data[index] = 0;
			}
		}

	}
	Mat big_binaryImg(initial_row + 2, initial_col + 2, CV_8UC1);
	cv::copyMakeBorder(binaryImg, big_binaryImg, 1, 1, 1, 1, BORDER_CONSTANT,0);



	for (i = 1; i <= initial_row; i++)
	{
		row_num = i * (initial_col + 2);
		row_num_last = (i - 1) * (initial_col + 2);
		for (j = 1; j <= initial_col; j++)
		{
			index = row_num + j;			
			x = i - 1;
			y = j - 1;
			x0 = x*initial_col + y;
			if (big_binaryImg.data[index])
			{
				c1 = big_binaryImg.data[index - 1];
				c2 = big_binaryImg.data[row_num_last + j - 1];
				c3 = big_binaryImg.data[row_num_last + j];
				c4 = big_binaryImg.data[row_num_last + j + 1];
				x1 = x*initial_col + y - 1;
				x2 = (x - 1)*initial_col + y - 1;
				x3 = (x - 1)*initial_col + y;
				x4 = (x - 1)*initial_col + y + 1;
				if (c3)
				{
					labelImg.data[x0] = labelImg.data[x3];
				}
				else if (c1)
				{
					labelImg.data[x0] = labelImg.data[x1];
					if (c4)
					{
						a = labelImg.data[x1];
						b = labelImg.data[x4];
						resolve_label(labelset, a ,b, label);
					}
				}
				else if (c2)
				{
					labelImg.data[x0] = labelImg.data[x2];
					if (c4)
					{
						a = labelImg.data[x2];
						b = labelImg.data[x4];
						resolve_label(labelset, a, b, label);
					}
				}
				else if (c4)
				{
					labelImg.data[x0] = labelImg.data[x4];
				}
				else
				{
					labelImg.data[x0] = label;
					labelset[label] = label;
					label++;
				}
			}
			else
			{
				labelImg.data[x0] = 0;
			}
		}
	}
	for (i = 0; i < initial_row; i++)
	{
		row_num = i * initial_col;
		for (j = 0; j < initial_col; j++)
		{
			index = row_num + j;
			temp_label = labelImg.data[index];
			labelImg.data[index] = labelset[temp_label];
		}

	}
	return MY_OK;
}

//测试函数
void test_ConnectedComponentLabeling(Mat &grayImg)
{
	Mat labelImg(grayImg.rows, grayImg.cols, CV_32SC1);
	int bin_th = 100;
	int i = 0, j = 0;

	time_t start = clock();
	for (i = 100; i > 0; i--)
	{
		ustc_ConnectedComponentLabeling(grayImg, labelImg, bin_th);
	}
	time_t end = clock();
	cout << (end - start) / 100.0f << "ms used per time\n";

#ifdef SHOW_IMG_IN_NUM
	cout << "\n\n";
	cout << "---------------------------------------------------------------------------------------------------------------\n";
	for (i = 0; i < labelImg.rows; i++)
	{
		for (j = 0; j < labelImg.cols; j++)
		{
			cout.width(2);
			cout << ((int*)labelImg.data)[i * labelImg.cols + j];
		}
		cout << endl;
	}
#endif // SHOW_IMG_IN_NUM

	cin >> i;
}

//灰度图二值化，二值图行列均多2
void gray_expand2_binary(const Mat &grayImg, Mat &binaryImg, const int bin_th)
{
	int i = 0, j = 0;
	int i_gray = 0, j_gray = 0;
	int i_binary = 0, j_binary = 0;
	int gray_row = grayImg.rows;
	int gray_col = grayImg.cols;
	int tmp_crt_row_gray = 0;
	int tmp_crt_row_bi = 0;
	int threshold[256] = { 0 };
	//等于阈值为前景
	for (i = 255; i >= bin_th; i--)
	{
		threshold[i] = 1;
	}

	int j_quotient = 0;
	int j_remainder = 0;
	for (i_gray = gray_row - 1, i_binary = gray_row; i_gray >= 0; i_gray--, i_binary--)
	{
		tmp_crt_row_bi = i_binary * binaryImg.cols;
		tmp_crt_row_gray = i_gray * gray_col;
		j_quotient = grayImg.cols >> 2;
		j_remainder = grayImg.cols - (j_quotient << 2);
		for (j_gray = gray_col - 1, j_binary = gray_col; j_quotient >= 0; j_quotient--)
		{
			binaryImg.data[tmp_crt_row_bi + j_binary] = threshold[grayImg.data[tmp_crt_row_gray + j_gray]];  j_gray--; j_binary--;
			binaryImg.data[tmp_crt_row_bi + j_binary] = threshold[grayImg.data[tmp_crt_row_gray + j_gray]];  j_gray--; j_binary--;
			binaryImg.data[tmp_crt_row_bi + j_binary] = threshold[grayImg.data[tmp_crt_row_gray + j_gray]];  j_gray--; j_binary--;
			binaryImg.data[tmp_crt_row_bi + j_binary] = threshold[grayImg.data[tmp_crt_row_gray + j_gray]];  j_gray--; j_binary--;
		}
		for (; j_remainder >= 0; j_remainder--)
		{
			binaryImg.data[tmp_crt_row_bi + j_binary] = threshold[grayImg.data[tmp_crt_row_gray + j_gray]];  j_gray--; j_binary--;
		}
	}

	//灰度图头尾两行置零
	i = (binaryImg.rows - 1) * binaryImg.cols;
	for (j = binaryImg.cols - 1; j >= 0; j--)
	{
		binaryImg.data[j] = 0;
		binaryImg.data[i + j] = 0;
	}

	//灰度图头尾两列置零
	j = binaryImg.cols - 1;
	for (i = binaryImg.rows - 1; i >= 0; i--)
	{
		tmp_crt_row_bi = i * binaryImg.cols;
		binaryImg.data[tmp_crt_row_bi] = 0;
		binaryImg.data[tmp_crt_row_bi + j] = 0;
	}
}

//
void label_tmp_shrink2_label(const Mat &labelImg_tmp, Mat &labelImg)
{
	int i = 0, j = 0;
	int i_tmpIm = 0, j_tmpIm = 0;
	int row = labelImg.rows;
	int col = labelImg.cols;
	int row_tmpIm = labelImg_tmp.rows;
	int col_tmpIm = labelImg_tmp.cols;
	int mark = 0;
	int crt_rowhead = 0;
	int crt_rowhead_tmpIm = 0;

	for (i = row - 1, i_tmpIm = row; i >= 0; i--, i_tmpIm--)
	{
		crt_rowhead = i * col;
		crt_rowhead_tmpIm = (i + 1) * col_tmpIm;
		for (j = col - 1; j >= 0; j--)
		{
			mark = ((int*)labelImg_tmp.data)[crt_rowhead_tmpIm + j + 1];
			((int*)labelImg.data)[crt_rowhead + j] = mark;
		}
	}
}