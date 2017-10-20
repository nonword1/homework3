#include “ConnectedComponentLabeling.h”

#define MY_BACKGROUND 0
#define MY_SCAN_LEN 4

//功能说明：更新标签的连接关系
//label：“标签连接关系”的数组
//first：被连接的第一个标签
//second：被连接的第二个标签
void my_connect(int *label, int first, int second);

int ustc_ConnectedComponentLabeling(Mat grayImg, Mat & labelImg, int bin_th)
{
	uchar *data_img = (uchar *)grayImg.data;
	if (NULL == data_img)
	{
		cout << "failed to read img!" << endl;
		return MY_FAIL;
	}

	int *data_label = (int *)labelImg.data;
	if (NULL == data_label)
	{
		cout << "labelImg is NULL!" << endl;
		return MY_FAIL;
	}

	int mRows = grayImg.rows;  //灰度图行数
	int mCols = grayImg.cols;  //灰度图列数
	int len = mRows*mCols / 4;  //“标签连接关系”数组的长度
	int *label = new int[len]();  //“标签连接关系”数组
	int now_label = 1;  //当前标签
	int scan[MY_SCAN_LEN];  //扫描器大小
	int num = 0;  //当前扫描器内标签个数

	//初始化“标签连接关系”数组，对应位置设成对应值
	for (int i = 0; i < len; i++)
	{
		label[i] = i;
	}

	//将标签图扩展一圈，扩充的像素值置0，并填入原图像素，这样做是为了便于处理边界点
	Mat ex_labelImg(mRows + 2, mCols + 2, CV_32FC1);
	ex_labelImg.setTo(MY_BACKGROUND);
	int *data_ex = (int *)ex_labelImg.data;
	int mRows_ex = ex_labelImg.rows;  //扩展标签图行数
	int mCols_ex = ex_labelImg.cols;  //扩展标签图列数
	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mCols; j++)
		{
			data_ex[(i + 1)*mCols_ex + (j + 1)] = data_img[i*mCols + j];
		}
	}

	//扫描扩展后的标签图
	for (int i = 1; i < mRows + 1; i++)
	{
		for (int j = 1; j < mCols + 1; j++)
		{
			int index = i*mCols_ex + j;
			int pix = data_ex[index];

			//如果是背景，不作处理，标签记为0
			if (pix < bin_th)
			{
				data_ex[index] = MY_BACKGROUND;
			}
			//如果是前景，检查其邻域（扫描器）
			else
			{			
				int index_left_top = (i - 1)*mCols_ex + (j - 1);
				int label_left_top = data_ex[index_left_top];
				int label_top = data_ex[index_left_top + 1];
				int label_right_top = data_ex[index_left_top + 2];
				int label_left = data_ex[index_left_top + mCols_ex];

				//记录下扫描器内的标签值
				if (label_left_top != MY_BACKGROUND)
				{
					scan[num] = label_left_top;
					num++;
				}
				if (label_top != MY_BACKGROUND)
				{
					scan[num] = label_top;
					num++;
				}
				if (label_right_top != MY_BACKGROUND)
				{
					scan[num] = label_right_top;
					num++;
				}
				if (label_left != MY_BACKGROUND)
				{
					scan[num] = label_left;
					num++;
				}

				//扫描器内都是背景
				if (num == 0)
				{
					data_ex[index] = now_label;
					now_label++;
				}
				//扫描器内有标签
				else
				{
					//给当前位置填入扫描器内的最小标签
					int min_label = scan[0];
					for (int k = 1; k < num; k++)
					{
						if (min_label > scan[k])
						{
							min_label = scan[k];
						}
					}
					data_ex[index] = min_label;

					//更新标签之间的联通关系
					for (int k = 1; k < num; k++)
					{
						my_connect(label, scan[k-1], scan[k]);
					}
				}
				num = 0;  //扫描器内标签数清零，为下一次扫描做准备
				
			}
		}
	}

	//第一次扫描完标签图后，第二次更新“标签连接关系”数组
	for (int i = 1; i < now_label; i++)
	{
		int tag = label[i];
		//找到该标签连通的最小标签值
		while (label[tag] != tag)
		{
			tag = label[tag];
		}
		label[i] = tag;
	}

	//第二次扫描标签图，完成连通域标记
	for (int i = 1; i < mRows + 1; i++)
	{
		for (int j = 1; j < mCols + 1; j++)
		{
			int index_ex = i*mCols_ex + j;
			int index = (i - 1)*mCols + (j - 1);
			int tag = data_ex[index_ex];
			data_label[index] = label[tag];
		}
	}
	delete []label;
	return MY_OK;
}

void my_connect(int * label, int first, int second)
{
	if (first == second)
	{
		return;
	}

  //一个记录大标签，一个记录小标签
	int label_b = first;
	int label_s = second;
	int cash;  //用于交换数据
	if (first < second)
	{
		label_b = second;
		label_s = first;
	}

	//大标签已经记录了连接关系，则转化为记录的标签与另一标签的连接关系
	while(label[label_b] != label_b)
	{
		label_b = label[label_b];
		//记录的标签与另一标签相同，说明该连接关系已被记录，直接返回
		if (label_b == label_s)
		{
			return;
		}

		//记录的标签与另一标签不同，则重复以上步骤（要确保是在大标签下记录小标签）
		if (label_b < label_s)
		{
			cash = label_b;
			label_b = label_s;
			label_s = cash;
		}
	}

	//记录标签连接关系
	label[label_b] = label_s;
	return;
}
