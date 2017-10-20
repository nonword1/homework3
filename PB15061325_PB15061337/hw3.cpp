#include “ConnectedComponentLabeling.h”

int ustc_ConnectedComponentLabeling(Mat grayImg, Mat& lablelImg, int bin_th)
{
	if (NULL == grayImg.data)
	{
		cout << "图片为空！" << endl;
		return MY_FAIL;
	}
	if (grayImg.type() != 0) {
		cout << "图片格式错误！" << endl;
		return MY_FAIL;
	}

	Mat binaryImg;
	threshold(grayImg, binaryImg, bin_th, 255, THRESH_BINARY);
	//namedWindow("binaryIMG", WINDOW_AUTOSIZE);
	//imshow("binaryIMG", binaryImg);

	int Rows = grayImg.rows;
	int Cols = grayImg.cols;
	int ExRows = Rows + 2;
	int ExCols = Cols + 2;
	Mat Expand_Img(ExRows, ExCols, CV_32SC1);
	Expand_Img.setTo(0);


	Mat tempImg = Expand_Img(Rect(1, 1, Cols, Rows));
	binaryImg.convertTo(tempImg, CV_32SC1);
	int *p = NULL;

	int max_label = 10000; //最大连通域数目
	//int max_label = Rows*Cols / 2; //最大连通域数目
	int *label_Table = new int[max_label]; //等价表格
	int *zuji1 = new int[max_label];
	int *zuji2 = new int[max_label];
	int *zuji3 = new int[max_label];
	int *zuji4 = new int[max_label];
	int curr_label;
	for (curr_label = 0; curr_label < max_label; curr_label++)
	{
		label_Table[curr_label] = curr_label;
	}
	curr_label = 0;

	int i, j;
	int i1, i2, i3, i4;
	int temp1_label, temp2_label, temp3_label, temp4_label;
	p = (int*)Expand_Img.data;
	int index_up;
	int temp_minlabel;
	int up, upleft, upright, left;
	int type;
	for (i = 1; i < ExRows - 1; i++)
	{
		p += ExCols;
		for (j = 1; j < ExCols - 1; j++)
		{
			index_up = j - ExCols;
			up = p[index_up];
			upleft = p[index_up - 1];
			upright = p[index_up + 1];
			left = p[j - 1];
			type = (!!upleft) * 8 + (!!up) * 4 + (!!upright) * 2 + !!left;
			if (p[j])
			{
				switch (type)
				{
				case 0://无标记
					curr_label++;
					p[j] = curr_label;
					break;
					//只有一个标记
				case 1://只有左有标记
					p[j] = left;
					break;
				case 2://只有右上有标记
					p[j] = upright;
					break;
				case 4://只有上有标记
					p[j] = up;
					break;
				case 8://只有左上有标记
					p[j] = upleft;
					break;
					//有两个标记
				case 3://左、右上有标记
					if (left == upright)
					{
						p[j] = left;
						break;
					}
					temp1_label = left;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = upright;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;

					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
				case 5://左、上有标记
					if (left == up)
					{
						p[j] = left;
						break;
					}
					temp1_label = left;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = up;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;
					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
				case 6://上、右上有标记
					if (up == upright)
					{
						p[j] = up;
						break;
					}
					temp1_label = up;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = upright;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;
					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
				case 9://左上、左有标记
					if (left == upleft)
					{
						p[j] = left;
						break;
					}
					temp1_label = upleft;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = left;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;
					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
				case 10://左上、右上有标记
					if (upleft == upright)
					{
						p[j] = upleft;
						break;
					}
					temp1_label = upleft;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = upright;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;
					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
				case 12://左上、上有标记
					if (upleft == up)
					{
						p[j] = up;
						break;
					}
					temp1_label = upleft;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = up;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;
					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
					//有三个标记
				case 7://上、右上、左有标记
					if ((up == upright) && (up == left))
					{
						p[j] = left;
						break;
					}
					temp1_label = up;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = upright;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp3_label = left;
					zuji3[0] = temp3_label;
					for (i3 = 1; label_Table[temp3_label] != temp3_label; i3++)
					{
						temp3_label = label_Table[temp3_label];
						zuji3[i3] = temp3_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;
					temp_minlabel = temp_minlabel < temp3_label ? temp_minlabel : temp3_label;
					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					for (i3--; i3 >= 0; i3--)
					{
						label_Table[zuji3[i3]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
				case 11://左上、右上、左有标记
					if ((upleft == upright) && (upleft == left))
					{
						p[j] = left;
						break;
					}
					temp1_label = upleft;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = upright;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp3_label = left;
					zuji3[0] = temp3_label;
					for (i3 = 1; label_Table[temp3_label] != temp3_label; i3++)
					{
						temp3_label = label_Table[temp3_label];
						zuji3[i3] = temp3_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;
					temp_minlabel = temp_minlabel < temp3_label ? temp_minlabel : temp3_label;
					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					for (i3--; i3 >= 0; i3--)
					{
						label_Table[zuji3[i3]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
				case 13://左上、上、左有标记
					if ((up == upleft) && (up == left))
					{
						p[j] = left;
						break;
					}
					temp1_label = upleft;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = up;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp3_label = left;
					zuji3[0] = temp3_label;
					for (i3 = 1; label_Table[temp3_label] != temp3_label; i3++)
					{
						temp3_label = label_Table[temp3_label];
						zuji3[i3] = temp3_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;
					temp_minlabel = temp_minlabel < temp3_label ? temp_minlabel : temp3_label;
					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					for (i3--; i3 >= 0; i3--)
					{
						label_Table[zuji3[i3]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
				case 14://左上、上、右上有标记
					if ((up == upright) && (up == upleft))
					{
						p[j] = upleft;
						break;
					}
					temp1_label = upleft;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = up;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp3_label = upright;
					zuji3[0] = temp3_label;
					for (i3 = 1; label_Table[temp3_label] != temp3_label; i3++)
					{
						temp3_label = label_Table[temp3_label];
						zuji3[i3] = temp3_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;
					temp_minlabel = temp_minlabel < temp3_label ? temp_minlabel : temp3_label;
					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					for (i3--; i3 >= 0; i3--)
					{
						label_Table[zuji3[i3]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
					//有四个标记
				case 15://四个均有标记
					if ((up == upright) && (up == left) && (up == upleft))
					{
						p[j] = left;
						break;
					}
					temp1_label = upleft;
					zuji1[0] = temp1_label;
					for (i1 = 1; label_Table[temp1_label] != temp1_label; i1++)
					{
						temp1_label = label_Table[temp1_label];
						zuji1[i1] = temp1_label;
					}
					temp2_label = up;
					zuji2[0] = temp2_label;
					for (i2 = 1; label_Table[temp2_label] != temp2_label; i2++)
					{
						temp2_label = label_Table[temp2_label];
						zuji2[i2] = temp2_label;
					}
					temp3_label = upright;
					zuji3[0] = temp3_label;
					for (i3 = 1; label_Table[temp3_label] != temp3_label; i3++)
					{
						temp3_label = label_Table[temp3_label];
						zuji3[i3] = temp3_label;
					}
					temp4_label = left;
					zuji4[0] = temp4_label;
					for (i4 = 1; label_Table[temp4_label] != temp4_label; i4++)
					{
						temp4_label = label_Table[temp4_label];
						zuji4[i4] = temp4_label;
					}
					temp_minlabel = temp1_label < temp2_label ? temp1_label : temp2_label;
					temp_minlabel = temp_minlabel < temp3_label ? temp_minlabel : temp3_label;
					temp_minlabel = temp_minlabel < temp4_label ? temp_minlabel : temp4_label;
					for (i1--; i1 >= 0; i1--)
					{
						label_Table[zuji1[i1]] = temp_minlabel;
					}
					for (i2--; i2 >= 0; i2--)
					{
						label_Table[zuji2[i2]] = temp_minlabel;
					}
					for (i3--; i3 >= 0; i3--)
					{
						label_Table[zuji3[i3]] = temp_minlabel;
					}
					for (i4--; i4 >= 0; i4--)
					{
						label_Table[zuji4[i4]] = temp_minlabel;
					}
					p[j] = temp_minlabel;
					break;
				default:cout << "未知错误！" << endl;
					return MY_FAIL;
				}
			}

		}
	}
	for (i = 2; i <= curr_label; i++)
	{
		label_Table[i] = label_Table[label_Table[i]];
	}

	p = (int*)Expand_Img.data;
	for (i = 1; i < ExRows - 1; i++)
	{
		p += ExCols;
		for (j = 1; j < ExCols - 1; j++)
		{
			p[j] = label_Table[p[j]];
		}
	}

	tempImg.copyTo(labelImg);

	delete[]label_Table;
	return MY_OK;
}
