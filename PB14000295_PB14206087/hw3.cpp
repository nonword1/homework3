#define MAX_LABEL 10000

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
						resolve_label(labelset, labelset[a] ,labelset[b], label);
					}
				}
				else if (c2)
				{
					labelImg.data[x0] = labelImg.data[x2];
					if (c4)
					{
						a = labelImg.data[x2];
						b = labelImg.data[x4];
						resolve_label(labelset, labelset[a], labelset[b], label);
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