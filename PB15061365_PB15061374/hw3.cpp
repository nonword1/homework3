# include"ConnectedComponentLabeling.h"
//#define IMG_SHOW
int ustc_ConnectedComponentLabeling(Mat grayImg, Mat& labelingImg, int bin_th)
{
	int Rows = grayImg.rows;
	int Cols = grayImg.cols;
	Mat binaryImg(Rows, Cols, CV_8UC1);
	Mat binaryImg_show(Rows, Cols, CV_8UC1);
	//the create a threhold img
	cv::threshold(grayImg, binaryImg, bin_th, 1, cv::THRESH_BINARY);
	
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			labelingImg.data[i*Cols + j] = 0;
		}
	}
	if (binaryImg.data == NULL || labelingImg.data == NULL)
	{
		cout << "pic does not exist" << endl;
		return MY_FAIL;
	}
	if (Rows != labelingImg.rows || Cols != labelingImg.cols)
	{
		cout << "size error" << endl;
		return MY_FAIL;
	}
	Mat big_binaryImg(Rows + 1, Cols + 1, CV_8UC1);
	copyMakeBorder(binaryImg, big_binaryImg, 1, 1, 1, 1, 0);

	
	int scan_1dim[10000] = { 0 };
	int label = 1;
	////////////////////////////////////////////first scan
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			int x = j + 1;
			int y = i + 1;
			int col_big = Cols + 2;
			if (big_binaryImg.data[y*col_big + x])
			{
				int n;
				int max = 0;
				int min = 0;
				int a, b;

				n = (big_binaryImg.data[y*col_big + x - 1] <<3)
					+ (big_binaryImg.data[(y - 1)*col_big + x - 1]  <<2)
					+ (big_binaryImg.data[(y - 1)*col_big + x]  <<1)
					+ big_binaryImg.data[(y - 1)*col_big + x + 1];
			
				switch (n)
				{
				case 0:
					((int*)labelingImg.data)[i*Cols + j] = label;
					scan_1dim[label]= label;
					label++;
					break;
				case 1:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[(i - 1)*Cols + j + 1];
					break;
				case 2:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[(i - 1)*Cols + j];
					break;
				case 3:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[(i - 1)*Cols + j];
					break;
				case 4:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[(i - 1)*Cols + j - 1];
					break;
				case 5:
					a = scan_1dim[((int*)labelingImg.data)[(i - 1)*Cols + j - 1]];
					b = scan_1dim[((int*)labelingImg.data)[(i - 1)*Cols + j + 1]];
					if (a <= b)
					{
						max = b;
						min = a;
					}
					else
					{
						min = b;
						max =a;
					}
					for (int m = 1; m < label; m++)
					{
						if (scan_1dim[m] == max)
							scan_1dim[m] = min;
					}
					((int*)labelingImg.data)[i*Cols + j] = min;
					break;
				case 6:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[(i - 1)*Cols + j - 1];
					break;
				case 7:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[(i - 1)*Cols + j - 1];
					break;
				case 8:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[i*Cols + j - 1];
					break;
				case 9:
					a = scan_1dim[((int*)labelingImg.data)[i*Cols + j - 1]];
					b = scan_1dim[((int*)labelingImg.data)[(i - 1)*Cols + j + 1]];
					if (a<= b)
					{
						max = b;
						min = a;
					}
					else
					{
						min = b;
						max = a;
					}
					for (int m = 1; m < label; m++)
					{
						if (scan_1dim[m] == max)
							scan_1dim[m] = min;
					}
					((int*)labelingImg.data)[i*Cols + j] = min;
					break;
				case 10:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[i*Cols + j - 1];
					break;
				case 11:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[i*Cols + j - 1];
					break;
				case 12:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[i*Cols + j - 1];
					break;
				case 13:
					a = scan_1dim[((int*)labelingImg.data)[i*Cols + j - 1]];
					b = scan_1dim[((int*)labelingImg.data)[(i - 1)*Cols + j + 1]];
					if (a <= b)
					{
						max = b;
						min = a;
					}
					else
					{
						min = b;
						max = a;
					}
					for (int m = 1; m < label; m++)
					{
						if (scan_1dim[m] == max)
							scan_1dim[m] = min;
					}
					((int*)labelingImg.data)[i*Cols + j] = min;
					break;
				case 14:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[i*Cols + j - 1];
					break;
				case 15:
					((int*)labelingImg.data)[i*Cols + j] = ((int*)labelingImg.data)[i*Cols + j - 1];
					break;
				}
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////second scan
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Cols; j++)
		{
			if (((int*)labelingImg.data)[i*Cols + j])
			{
				((int*)labelingImg.data)[i*Cols + j] = scan_1dim[((int*)labelingImg.data)[i*Cols + j]];
			}
		}
	}
#ifdef IMG_SHOW
	cv::threshold(grayImg, binaryImg_show, bin_th, 255, cv::THRESH_BINARY);
	Mat outimg_show(Rows, Cols, CV_8UC1);
	int max = 0;
	for (int i = 0; i < label;i++) {
		if (scan_1dim[i] > max) max = scan_1dim[i];
	}
	for(int i=0;i<Rows;i++)
		for (int j = 0; j < Cols; j++)
		{
			outimg_show.data[i*Cols + j] = ((int*)labelingImg.data)[i*Cols + j] * 255 / max;
		}
	imwrite("result.bmp", outimg_show);
	imshow("binary_IMG", binaryImg_show);
	imshow("outImg", outimg_show);
	waitKey();
#endif

}
