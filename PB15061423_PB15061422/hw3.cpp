#include “ConnectedComponentLabeling.h”
#include <vector>
typedef struct MLABEL
{
	int labelnum;
	int result;
};
int ustc_ConnectedComponentLabeling(
	Mat grayImg,
	Mat& labelImg,
	int bin_th)
{
	if (grayImg.data == NULL)
	{
		cout << "no image!"<< endl;
		return MY_FAIL;
	}
	if (bin_th <= 0 || bin_th >= 255)
	{
		cout << "the threshhold is invalid!" << endl;
		return MY_FAIL;
	}
	Mat BinaryImg;
	threshold(grayImg, BinaryImg, bin_th, 255, CV_THRESH_BINARY);
	Mat borderImg;
	copyMakeBorder(BinaryImg, borderImg, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0));
	Mat Label=Label.zeros(borderImg.size(), CV_32SC1);
	int rows = BinaryImg.rows;
	int cols = BinaryImg.cols;
	int row_bor = borderImg.rows;
	int col_bor = borderImg.cols;
	int pix_row = 0, pix_col = 0;
	uchar *dataB = borderImg.data;
	int *dataL =(int*) Label.data;
	int label_num = 1;//label's number,
	vector<MLABEL>Label_array(1);
	vector<MLABEL>Extra_array;
	for (pix_row = 1; pix_row <=rows; pix_row++)
	{
		for (pix_col = 1; pix_col <= cols; pix_col++)
		{
			uchar a_Binary = dataB[(pix_row - 1)*col_bor + pix_col - 1], b_Binary = dataB[(pix_row - 1)*col_bor + pix_col], c_Binary = dataB[(pix_row - 1)*col_bor + pix_col + 1];
			uchar  d_Binary = dataB[pix_row*col_bor + pix_col - 1],	     e_Binary = dataB[pix_row*col_bor + pix_col];
			int a_Label = dataL[(pix_row - 1)*col_bor + pix_col - 1], b_Label = dataL[(pix_row - 1)*col_bor + pix_col], c_Label = dataL[(pix_row - 1)*col_bor + pix_col + 1];
			int d_Label = dataL[pix_row*col_bor + pix_col - 1],		 *e_Label = &(dataL[pix_row*col_bor + pix_col ]);
			int flag[4] = { a_Label, b_Label, c_Label,  d_Label };
			if (e_Binary == 255)
			{
				uchar zero_num = a_Binary + 1 + b_Binary + 1 + (c_Binary + 1)+ (d_Binary + 1);
				int max_btw12 = (((a_Label - b_Label) >> 31) & 1)*b_Label + (((b_Label - a_Label-1) >> 31) & 1)*a_Label;
				int min_btw12 = (((a_Label - b_Label) >> 31) & 1)*a_Label + (((b_Label - a_Label-1) >> 31) & 1)*b_Label;
				int max_btw34 = (((c_Label - d_Label) >> 31) & 1)*d_Label + (((d_Label - c_Label-1) >> 31) & 1)*c_Label;
				int min_btw34 = (((c_Label - d_Label) >> 31) & 1)*c_Label + (((d_Label - c_Label-1) >> 31) & 1)*d_Label;
				int max_btw1234 = (((max_btw12 - max_btw34) >> 31) & 1)*max_btw34 + (((max_btw34 - max_btw12-1) >> 31) & 1)*max_btw12;
				int min_btw1234 = (((min_btw12 - min_btw34) >> 31) & 1)*min_btw12 + (((min_btw34 - min_btw12-1) >> 31) & 1)*min_btw34;
				flag[0] = max_btw1234;
				int temp1 = max_btw12 + max_btw34 - max_btw1234;
				int temp2 = min_btw12 + min_btw34 - min_btw1234;
				flag[3] = min_btw1234;
				flag[1] = (((temp1 - temp2) >> 31) & 1)*temp2 + (((temp2 - temp1-1) >> 31) & 1)*temp1;
				flag[2] = (((temp1 - temp2) >> 31) & 1)*temp1 + (((temp2 - temp1-1) >> 31) & 1)*temp2;
				switch (zero_num)
				{
				case 0:
					*e_Label = flag[3];
					if (flag[0] != flag[3])
					{
						//flag[0]--flag[3]
						if (Label_array[flag[0]].result == flag[0])
						{
							Label_array[flag[0]].result = flag[3];
						}
						else
						{
							MLABEL newlabel;
							newlabel.labelnum = flag[0];
							newlabel.result = flag[3];
							Extra_array.push_back(newlabel);
						}
						//flag[1]--flag[3]
						if (Label_array[flag[1]].result == flag[1])
						{
							Label_array[flag[1]].result = flag[3];
						}
						else
						{
							MLABEL newlabel;
							newlabel.labelnum = flag[1];
							newlabel.result = flag[3];
							Extra_array.push_back(newlabel);
						}
						//flag[2]--flag[3]
						if (Label_array[flag[2]].result == flag[2])
						{
							Label_array[flag[2]].result = flag[3];
						}
						else
						{
							MLABEL newlabel;
							newlabel.labelnum = flag[2];
							newlabel.result = flag[3];
							Extra_array.push_back(newlabel);
						}
					}
					break;
				case 1:
					*e_Label = flag[2];
					if (flag[0] != flag[2])
					{
						//flag[0]--flag[2]
						if (Label_array[flag[0]].result == flag[0])
						{
							Label_array[flag[0]].result = flag[2];
						}
						else
						{
							MLABEL newlabel;
							newlabel.labelnum = flag[0];
							newlabel.result = flag[2];
							Extra_array.push_back(newlabel);
						}
						//flag[1]--flag[2]
						if (Label_array[flag[1]].result == flag[1])
						{
							Label_array[flag[1]].result = flag[2];
						}
						else
						{
							MLABEL newlabel;
							newlabel.labelnum = flag[1];
							newlabel.result = flag[2];
							Extra_array.push_back(newlabel);
						}
					}
					break;
				case 2:
					*e_Label = flag[1];
					if (flag[0] != flag[1])
					{
						//flag[0]--flag[1]
						if (Label_array[flag[0]].result == flag[0])
						{
							Label_array[flag[0]].result = flag[1];
						}
						else
						{
							MLABEL newlabel;
							newlabel.labelnum = flag[0];
							newlabel.result = flag[1];
							Extra_array.push_back(newlabel);
						}
					}
					break;
				case 3:
					*e_Label = flag[0];
					break;
				case 4:
					*e_Label = label_num;
					MLABEL newlabel;
					newlabel.labelnum = label_num;
					newlabel.result = label_num;
					Label_array.push_back(newlabel);
					label_num++;

					break;
				}
				
			}
		}
	}
	
	int array_index,temp_size=Label_array.size();
	for (array_index = 1; array_index < temp_size; array_index++)
	{
		int temp_result = Label_array[array_index].result;
		Label_array[array_index].result = Label_array[temp_result].result;
	}
	for (array_index = 0, temp_size = Extra_array.size(); array_index < temp_size; array_index++)
	{
		int connect1 = Extra_array[array_index].labelnum;
		int connect2 = Extra_array[array_index].result;
		
		int small_num = Label_array[connect1].result;
		int large_num = Label_array[connect2].result;
		if (Label_array[connect1].result != Label_array[connect2].result)
		{
			if (Label_array[connect1].result > Label_array[connect2].result)
			{
				small_num = Label_array[connect2].result;
				large_num = Label_array[connect1].result;
			}
			Label_array[large_num].result = small_num;
			int array_index_t, temp_size_t = Label_array.size();
			for (array_index_t = 1; array_index_t < temp_size_t; array_index_t++)
			{
				int temp_result = Label_array[array_index_t].result;
				Label_array[array_index_t].result = Label_array[temp_result].result;
			}
		}
	}
	int lab_vector2array[2000] = { 0 };
	temp_size = Label_array.size();
	for (array_index = 1; array_index < temp_size; array_index++)
	{
		lab_vector2array[array_index] = Label_array[array_index].result;
	}

	for (pix_row = 1; pix_row <= rows; pix_row++)
	{
		for (pix_col = 1; pix_col <= cols; pix_col++)
		{
			int *_label= &(dataL[pix_row*col_bor + pix_col]);
			int e_label = dataL[pix_row*col_bor + pix_col];
			
			*_label = lab_vector2array[e_label];
		}
	}
	labelImg = Label(Range(1, row_bor), Range(1, col_bor)).clone();
	return MY_OK;
}
