#include “ConnectedComponentLabeling.h”
#define FOREGROUND_COLOR	1  //白色是255，设置成1是为了后面转换成01序列方便
#define BACKGROUND_COLOR	0

#define DEFINCE

//////////////////////////////////////////连通域检测////////////////////////////////////////////////


//二值化/在周围加上一层
int ustc_Threshold(Mat grayImg, Mat& binaryImg, int bin_th)
{
#ifdef DEFINCE
	if (NULL == grayImg.data || NULL == binaryImg.data)
	{
		cout << "grayImg or binaryImg is NULL." << endl;
		return MY_FAIL;
	}
	if (int(grayImg.channels()) != 1 || int(binaryImg.channels()) != 1)
	{
		cout << "grayImg or binaryImg is not gray" << endl;
		return MY_FAIL;
	}
	if (bin_th <= 0 || bin_th >= 255)
	{
		cout << "bin_th is out of range" << endl;
		return MY_FAIL;
	}
#endif // DEFINCE

	int height = grayImg.rows;
	int width = grayImg.cols;

	uchar* binaryImgData = binaryImg.data;

	//四周增加一层边框
	int location1 = (height + 1) * (width + 2);
	for (int col_j = 0; col_j < width + 2; col_j++)
	{
		binaryImgData[col_j] = BACKGROUND_COLOR;
		binaryImgData[location1 + col_j] = BACKGROUND_COLOR;
		
	}

	for (int row_i = 0; row_i < height + 2; row_i++)
	{
		binaryImgData[row_i * (width+2)] = BACKGROUND_COLOR;
		binaryImgData[row_i * (width+2) + width + 1] = BACKGROUND_COLOR;
	}

	//二值化
	uchar* grayImgData = grayImg.data;
	for (int row_i = 0; row_i < height; row_i++)
	{
		int loca = row_i * width;
		for (int col_j = 0; col_j < width; col_j++)
		{
			int value = grayImgData[loca + col_j] - bin_th >> 10;
			binaryImgData[loca + 2 * row_i + width + col_j + 3] = (value + 1);//*FOREGROUND_COLOR
		}
	}

#ifdef IMG_SHOW
	namedWindow("Threshood", 0);
	imshow("Threshood", binaryImg);
	waitKey(0);
#endif // IMG_SHOW
	return MY_OK;
}

//连续的图像进行拼接，每次遇到周围有两种不同的色块的时候都把所有大的label置为0
int ustc_ChangeLabel(int* labelImgDataInt, int connectSmall, int connectBig, int location, int connectBig_max)
{
	//connectBig_max 添加这个变量以大幅度减少运行时间 不用每次都遍历完全图 只要把所有等于connectBig位置都替换就可以
	//这个想法挺好的 程序的执行时间少了很多很多
	int connectBig_num = 0;
	for (int loca = location; (loca >= 0) && (connectBig_num < connectBig_max); loca--)
	{
		if (labelImgDataInt[loca] == connectBig)
		{
			labelImgDataInt[loca] = connectSmall;
			connectBig_num++;
		}
	}
	return MY_OK;
}

//选择相邻四个中,小的label进行赋值，并调用图像拼接
int ustc_ChooseSmallLable_ChangeLabel(int* labelImgDataInt, int label_1, int label_2, int location, int* label_count)
{
	if (label_1 == label_2)
	{
		labelImgDataInt[location] = label_1;
		label_count[label_1]++;
	}
	else if (label_1 < label_2)
	{
		labelImgDataInt[location] = label_1;
		ustc_ChangeLabel(labelImgDataInt, label_1, label_2, location, label_count[label_2]);
		label_count[label_1] += label_count[label_2] + 1;
	}
	else
	{
		labelImgDataInt[location] = label_2;
		ustc_ChangeLabel(labelImgDataInt, label_2, label_1, location, label_count[label_1]);
		label_count[label_2] += label_count[label_1] + 1;
	}
	return MY_OK;
}

//连通域
int ustc_ConnectedComponentLabeling(
	Mat grayImg,
	Mat& labelImg,
	int bin_th)
{
#ifdef DEFINCE
	if (NULL == grayImg.data || NULL == labelImg.data)
	{
		cout << "grayImg or labelImg is NULL." << endl;
		return MY_FAIL;
	}
	if (int(grayImg.channels()) != 1 || int(labelImg.channels()) != 1)
	{
		cout << "grayImg or labelImg is not channel 1" << endl;
		return MY_FAIL;
	}
	if (bin_th <= 0 || bin_th >= 255)
	{
		cout << "bin_th is out of range" << endl;
		return MY_FAIL;
	}
#endif // DEFINCE
	int width_gray = grayImg.cols;
	int height_gray = grayImg.rows;

	int width = width_gray + 2;
	int height = height_gray + 2;
	
	Mat newlabelImg(height, width, CV_32SC1);
	newlabelImg.setTo(0);

	Mat binaryImg(height, width, CV_8UC1);
	//二值化的同时，增加一层边框
	ustc_Threshold(grayImg, binaryImg, bin_th);

	int lable_num = 1;

	int* labelImgDataInt = ((int*)(newlabelImg.data));
	uchar* binaryImgData = binaryImg.data;

	int label_count[30000] = { 0 };


	int test_temp = 0;
	for (int row_i = 1; row_i < height; row_i++)
	{
		for (int col_j = 1; col_j < width; col_j++)
		{
			int location = row_i * width + col_j;
			if (FOREGROUND_COLOR == binaryImgData[location])
			{

				//四位01序列：左上.上.右上.左
				int situation =
					binaryImgData[location - 1]
					+ 10 * binaryImgData[location - width + 1]
					+ 100 * binaryImgData[location - width]
					+ 1000 * binaryImgData[location - width - 1];
				switch (situation)
				{
					//周围没被标记
				case 0:
					labelImgDataInt[location] = lable_num;
					label_count[lable_num]++;
					lable_num++;							
					break;
					//周围有一个被标记
				case 1:
					labelImgDataInt[location] = labelImgDataInt[location - 1];
					label_count[labelImgDataInt[location - 1]]++;
					break;
				case 10:
					labelImgDataInt[location] = labelImgDataInt[location - width + 1];
					label_count[labelImgDataInt[location - width + 1]]++;
					break;
				case 100:
					labelImgDataInt[location] = labelImgDataInt[location - width];
					label_count[labelImgDataInt[location - width]]++;
					break;
				case 1000:
					labelImgDataInt[location] = labelImgDataInt[location - width - 1];
					label_count[labelImgDataInt[location - width - 1]]++;
					break;
					//周围有两个被标记
				case 11:
					ustc_ChooseSmallLable_ChangeLabel(labelImgDataInt, labelImgDataInt[location - width + 1], labelImgDataInt[location - 1], location, label_count);
					break;
				case 101:
					labelImgDataInt[location] = labelImgDataInt[location - 1];
					label_count[labelImgDataInt[location - 1]]++;
					break;
				case 110:
					labelImgDataInt[location] = labelImgDataInt[location - width];
					label_count[labelImgDataInt[location - width]]++;				
					break;
				case 1001:
					labelImgDataInt[location] = labelImgDataInt[location - 1];
					label_count[labelImgDataInt[location - 1]]++;
					break;
				case 1010:
					ustc_ChooseSmallLable_ChangeLabel(labelImgDataInt, labelImgDataInt[location - width - 1], labelImgDataInt[location - width + 1], location, label_count);
					break;
				case 1100:
					labelImgDataInt[location] = labelImgDataInt[location - width];
					label_count[labelImgDataInt[location - width]]++;
					break;
					//周围有三个被标记
				case 111:
					labelImgDataInt[location] = labelImgDataInt[location - 1];
					label_count[labelImgDataInt[location - 1]]++;
					break;
				case 1011:
					ustc_ChooseSmallLable_ChangeLabel(labelImgDataInt, labelImgDataInt[location - 1], labelImgDataInt[location - width + 1], location, label_count);
					break;
				case 1101:
					labelImgDataInt[location] = labelImgDataInt[location - 1];
					label_count[labelImgDataInt[location - 1]]++;
					break;
				case 1110:
					labelImgDataInt[location] = labelImgDataInt[location - width];
					label_count[labelImgDataInt[location - width]]++;
					break;
					//周围四个都被标记
				case 1111:		
					labelImgDataInt[location] = labelImgDataInt[location - 1];
					label_count[labelImgDataInt[location - 1]]++;
					break;
				default:
					cout << "situation is wrong!" << endl;
					cout << "situation is " << situation << endl;
					cout << "row_i " << row_i << "\tcol_j " << col_j << endl;
					getchar();
					break;
				}
			}
		}
	}

	//把内部定义的newlabelImg赋值给外部的labelImg
	for (int row_i = 0; row_i < height_gray; row_i++)
	{
		for (int col_j = 0; col_j < width_gray; col_j++)
		{
			int location_label = row_i * width_gray + col_j;
			int location_label_new = (row_i + 1) * (width_gray + 2) + col_j + 1;
			((int*)(labelImg.data))[location_label] = labelImgDataInt[location_label_new];
		}
	}
	return MY_OK;
}
