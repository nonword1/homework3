#include “ConnectedComponentLabeling.h”
int ustc_ConnectedComponentLabeling(Mat grayImg, Mat& lablelImg, int bin_th){
  	if (0 > bin_th || 255 < bin_th) 
	{
		cout << "bin_th too small";
		return MY_FAIL;
	}

	if (NULL == grayImg.data || NULL == labelImg.data)
	{
		cout << "input erro";
		return MY_FAIL;
	}

	if (grayImg.channels() != 1 || labelImg.channels() != 1) 
	{
		cout << "channel erro" << endl;
		return MY_FAIL;
	}
	int width = grayImg.cols;
	int height = grayImg.rows;

	if (labelImg.cols != width || labelImg.rows != height) 
	{
		cout << "input erro" << endl;
		return MY_FAIL;
	}
	//动态分配标签数组
	int* T = NULL;
	int size_T = ((width*height) >>2) + 1;
	T = new int[size_T];

	if (NULL == T) 
	{
		cout << "no enough tip";
		return MY_FAIL;
	}

	memset(T, 0, size_T * sizeof(int));

	//定义加边图片
	int width_ex = width + 2;
	int height_ex = height + 1;

	
	//二值化并存储
	Mat binaryImg(height, width, CV_8UC1);
	threshold(grayImg, binaryImg, bin_th, 1, THRESH_BINARY);
	Mat ex_binaryImg(height_ex, width_ex, CV_8UC1,Scalar(0));
	Mat ex_labelImg(height_ex, width_ex, CV_32SC1, Scalar(0));


	//将binaryImg中从（1，1）开始截图
	Mat imageROI = ex_binaryImg(Rect(1, 1, width, height));
	binaryImg.copyTo(imageROI);

	/*
	namedWindow("ex_binary", 1);
	namedWindow("binary", 1);
	imshow("ex_binary", ex_binaryImg);
	imshow("binary",binaryImg);
	waitKey(0);*/

	uchar *ptr_ex_bin = ex_binaryImg.data;
  	int *ptr_ex_lab = (int*)ex_labelImg.data;

	int maxlabel = 1;//标签序号
	int height_of_initial = height + 1;
	int width_of_initial = width + 1;

	for (int i = 1; i < height_of_initial; i++)
	{
		//本行和上一行坐标乘宽
		int temp_x2 = i * width_ex;
		int temp_x1 = temp_x2 - width_ex;
		for (int j = 1; j < width_of_initial; j++)
		{
			int loc = temp_x2 + j;
			//若该点为背景
			if (!ptr_ex_bin[loc])
			{
				continue;
			}

			//前后一列的列坐标
			int loc_b = loc - width_ex;
			int loc_a = loc_b - 1;
			int loc_c = loc_b + 1;
			int loc_d = loc - 1;
			//取二值
			int a = ptr_ex_bin[loc_a];
			int b = ptr_ex_bin[loc_b];
			int c = ptr_ex_bin[loc_c];
			int d = ptr_ex_bin[loc_d];
			int label_a = ptr_ex_lab[loc_a];
			int label_b = ptr_ex_lab[loc_b];
			int label_c = ptr_ex_lab[loc_c];
			int label_d = ptr_ex_lab[loc_d];

			int bi_label = (a<<3) + (b <<2) + (c <<1) + d;
			int min;
			int t_label_a;
			int t_label_b;
			int t_label_c;
			int t_label_d;
			switch (bi_label)
			{
			case 0://0000
				ptr_ex_lab[loc] = maxlabel;
  				T[maxlabel] = maxlabel;
				maxlabel++;
				break;
			case 1://0001
				ptr_ex_lab[loc] = T[label_d];
				break;
			case 2://0010
				ptr_ex_lab[loc] = T[label_c];
				break;
			case 3://0011
				t_label_c = T[label_c];
				t_label_d = T[label_d];
				if (t_label_c <= t_label_d)
				{
					ptr_ex_lab[loc] = t_label_c;
					T[label_d] = t_label_c;
				}
				else
				{
					ptr_ex_lab[loc] = t_label_d;
					T[label_c] = t_label_d;
				}
				break;
			case 4://0100
				ptr_ex_lab[loc] = T[label_b];
				break;
			case 5://0101
				t_label_b = T[label_b];
				t_label_d = T[label_d];
				if (t_label_b <= t_label_d)
				{
					ptr_ex_lab[loc] = t_label_b;
					T[label_d] = t_label_b;
				}
				else
				{
					ptr_ex_lab[loc] = t_label_d;
					T[label_b] = t_label_d;
				}
				break;
			case 6://0110
				t_label_b = T[label_b];
				t_label_c = T[label_c];
				if (t_label_b <= t_label_c)
				{
					ptr_ex_lab[loc] = t_label_b;
					T[label_c] = t_label_b;
				}
				else
				{
					ptr_ex_lab[loc] = t_label_c;
					T[label_b] = t_label_c;
				}
				break;
			case 7://0111
				min = T[label_b];
				if (min > T[label_c])
					min = T[label_c];
				if (min > T[label_d])
					min = T[label_d];
				ptr_ex_lab[loc] = min;
				T[label_b] = min;
				T[label_c] = min;
				T[label_d] = min;
				break;
			case 8://1000
				ptr_ex_lab[loc] = T[label_a];
				break;
			case 9://1001
				t_label_a = T[label_a];
				t_label_d = T[label_d];
				if (t_label_a <= t_label_d)
				{
					ptr_ex_lab[loc] = t_label_a;
					T[label_d] = t_label_a;
				}
				else
				{
					ptr_ex_lab[loc] = t_label_d;
					T[label_a] = t_label_d;
				}
				break;
			case 10://1010
				t_label_a = T[label_a];
				t_label_c = T[label_c];
				if (t_label_a <= t_label_c)
				{
					ptr_ex_lab[loc] = t_label_a;
					T[label_c] = t_label_a;
				}
				else
				{
					ptr_ex_lab[loc] = t_label_c;
					T[label_a] = t_label_c;
				}
				break;
			case 11://1011
				min = T[label_a];
				if (min > T[label_c])
					min = T[label_c];
				if (min > T[label_d])
					min = T[label_d];
				ptr_ex_lab[loc] = min;
				T[label_a] = min;
				T[label_c] = min;
				T[label_d] = min;
				break;
			case 12://1100
				t_label_a = T[label_a];
				t_label_b = T[label_b];
				if (t_label_a <= t_label_b)
				{
					ptr_ex_lab[loc] = t_label_a;
					T[label_b] = t_label_a;
				}
				else
				{
					ptr_ex_lab[loc] = t_label_b;
					T[label_a] = t_label_b;
				}
				break;
			case 13://1101
				min = T[label_a];
				if (min > T[label_b])
					min = T[label_b];
				if (min > T[label_d])
					min = T[label_d];
				ptr_ex_lab[loc] = min;
				T[label_a] = min;
				T[label_b] = min;
				T[label_d] = min;
				break;
			case 14://1110
				min = T[label_a];
				if (min > T[label_b])
					min = T[label_b];
				if (min > T[label_c])
					min = T[label_c];
				ptr_ex_lab[loc] = min;
				T[label_a] = min;
				T[label_b] = min;
				T[label_c] = min;
				break;
			case 15://1111
				min = T[label_a];
				if (min > T[label_b])
					min = T[label_b];
				if (min > T[label_c])
					min = T[label_c];
				if (min > T[label_d])
					min = T[label_d];
				ptr_ex_lab[loc] = min;
				T[label_a] = min;
				T[label_b] = min;
				T[label_c] = min;
				T[label_d] = min;
				break;


			}
		}
	}

	for (int i = 1; i < height_of_initial; i++)
	{
		int temp_loc2 = i*width_ex;
		for (int j = 1; j < width_of_initial; j++)
		{
			int loc2 = temp_loc2 + j;
            ptr_ex_lab[loc2] = T[ptr_ex_lab[loc2]];
		}
	}
	Mat part_of_labelImg = ex_labelImg(Rect(1, 1, width, height));
	part_of_labelImg.copyTo(labelImg);
        delete[] T;

	return MY_OK;
}

