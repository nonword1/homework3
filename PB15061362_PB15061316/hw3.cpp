#include "ConnectedComponentLabeling.h"
#define TWO_MIN(X,Y)  (Y ^ (( X ^ Y ) & - ( X <Y )))

// 当临域中有两个不是背景时对label数组的修改
void transLabel(int* label, int min, int max,int labelMax)
{
	int i = label[min];
	int j = label[max];
	if (i == j)
	{
		return;
	}
	//找到label数组中两个背景标号里存的数中小的一个和大的一个
	int minNumber = TWO_MIN(i, j);
	int maxNumber = i + j - minNumber;
	//从大数开始遍历之后的数组，将label数组中寸maxNumber改成minNumber
	label[maxNumber] = minNumber;
	for (int x = maxNumber + 1 ; x < labelMax; x++)
	{
		//使用位运算，若label[x]是maxNumber， 则变为minNumber，否则不变
		int temp = (((label[x] - maxNumber) >> 31) + ((maxNumber - label[x]) >> 31) + 1) * minNumber;
		label[x] = (-((temp - 1) >> 31)) * label[x] + temp;
	}
}

// 当临域中有三个不是背景时对label数组的修改
void transLabel2(int* label, int num1, int num2, int num3, int labelMax)
{
	int i = label[num1];
	int j = label[num2];
	int k = label[num3];
	if ((i == j) && (j == k))
	{
		return;
	}
	//找到label数组中三个背景标号里存的数中，最小的一个存在minNumber中，剩下的两个存在 maxNumber1，maxNumber2中，然后比较两者中较小的存在minNumber1中
	int minNumber1 = TWO_MIN(i, j);
	int maxNumber1 = i + j - minNumber1;
	int minNumber = TWO_MIN(k, minNumber1);
	int maxNumber2 = k + minNumber1 - minNumber;
	 minNumber1 = TWO_MIN(maxNumber1, maxNumber2);
	label[minNumber1] = minNumber;
	//从minNumber1开始，将数组中存的值为maxNumber1或maxNumber2的改为minNumber，否则不变
	for (int x = minNumber1 + 1; x < labelMax; x++)
	{
		int temp = (((label[x] - maxNumber1) >> 31) + ((maxNumber1 - label[x]) >> 31) + 1) * minNumber;
		label[x] = (-((temp - 1) >> 31)) * label[x] + temp;
		temp = (((label[x] - maxNumber2) >> 31) + ((maxNumber2 - label[x]) >> 31) + 1) * minNumber;
		label[x] = (-((temp - 1) >> 31)) * label[x] + temp;
	}
	
}

// 当临域中有四个不是背景时对label数组的修改
void transLabel3(int* label, int num1, int num2, int num3, int num4, int labelMax)
{
	
	int i = label[num1];
	int j = label[num2];
	int k = label[num3];
	int z = label[num4];
	if ((i == j) && (j == k) && (k == z))
	{
		return;
	}
	//找到label数组中四个背景标号里存的数中，最小的一个存在minNumber中，剩下的三个存在 maxNumber1，maxNumber2，maxNumber3中，然后比较三者中最小的存在minNumber1中
	int minNumber1 = TWO_MIN(i, j);
	int maxNumber1 = i + j - minNumber1;
	int minNumber2 = TWO_MIN(k, minNumber1);
	int maxNumber2 = k + minNumber1 - minNumber2;
	int minNumber = TWO_MIN(z, minNumber2);
	int maxNumber3 = z + minNumber2 - minNumber;
	minNumber1 = TWO_MIN(maxNumber1, maxNumber2);
	minNumber1 = TWO_MIN(minNumber1, maxNumber3);
	//从minNumber1开始，将数组中存的值为maxNumber1或maxNumber2或maxNumber3的改为minNumber，否则不变
	label[minNumber1] = minNumber;
	for (int x = minNumber1 + 1; x < labelMax; x++)
	{
		int temp = (((label[x] - maxNumber1) >> 31) + ((maxNumber1 - label[x]) >> 31) + 1) * minNumber;
		label[x] = (-((temp - 1) >> 31)) * label[x] + temp;
		temp = (((label[x] - maxNumber2) >> 31) + ((maxNumber2 - label[x]) >> 31) + 1) * minNumber;
		label[x] = (-((temp - 1) >> 31)) * label[x] + temp;
		temp = (((label[x] - maxNumber3) >> 31) + ((maxNumber3 - label[x]) >> 31) + 1) * minNumber;
		label[x] = (-((temp - 1) >> 31)) * label[x] + temp;
	}
	

}
int ustc_ConnectedComponentLabeling(Mat grayImg, Mat& labelImg, int bin_th)
{
	if (NULL == grayImg.data || NULL == labelImg.data)
	{
		cout << "image is NULL." << endl;
		return MY_FAIL;
	}
	int height = grayImg.rows + 2 ;
	int width = grayImg.cols + 2;
	Mat tempImg;
	Mat binaryImg(height, width, CV_8UC1);
    //tempImg存将grayImg上下左右各扩一排后的图
	copyMakeBorder(grayImg, tempImg, 1, 1, 1, 1, 0, 0);
	//binaryImg存将tempImg二值化后的图
	threshold(tempImg, binaryImg, bin_th,1, THRESH_BINARY);
	//connection存连通域结果，用0表示背景
	Mat connectImg(height , width , CV_32SC1);
	connectImg.setTo(0);
	int label[10000] ;
	label[0] = 0;
	int labelMax = 1;
	int *connect = (int *)connectImg.data;
	for (int i = 1; i < height - 1; ++i)
	{
		for (int j = 1; j < width - 1; ++j)
		{
			int xy = i * width + j;
			int value = binaryImg.data[xy];
			//如果为背景 value会变为- 16 ，若不是背景，为0
			value = (value - 1) << 4;
			int col_i = i - 1;
			int xy2 = col_i * width + j;
			//若value = -16 ，result 结果必定小于0，在switch中 会进入 default 中，若不是背景，则不会影响result的值
			int result = (binaryImg.data[xy2 - 1] << 3) + (binaryImg.data[xy2] << 2) +
				         (binaryImg.data[xy2 + 1] << 1) + (binaryImg.data[xy - 1]) + value;
			switch (result)
			{
			    case 0:
				{ 
					connect[xy] = labelMax;
					label[labelMax] = labelMax;
					labelMax++;
					break;
				 }
				case 1:
				{
					connect[xy] = connect[xy - 1];
					break;
				}
				case 2:
				{
					connect[xy] = connect[xy2 + 1];
					break;
				}
				case 3:
				{
					int num1 = connect[xy - 1];
					int num2 = connect[xy2 + 1];
					int minNumber = TWO_MIN( num1, num2);
					connect[xy] = minNumber;
					transLabel(label,num1, num2, labelMax);
					break;
				}
				case 4:
				{
					connect[xy] = connect[xy2];
					break;
				}
				case 5:
				{
					int num1 = connect[xy - 1];
					int num2 = connect[xy2];
					int minNumber = TWO_MIN(num1, num2);
					connect[xy] = minNumber;
					transLabel(label, num1,  num2, labelMax);
					break;
				}
				case 6:
				{
					int num1 = connect[xy2 + 1];
					int num2 = connect[xy2];
					int minNumber = TWO_MIN(num1, num2);
					connect[xy] = minNumber;
					transLabel(label, num1, num2, labelMax);
					break;
				}
				case 7:
				{
					int num1 = connect[xy2 + 1];
					int num2 = connect[xy2];
					int num3 = connect[xy - 1];
					int minNumber1 = TWO_MIN(num1, num2);
					int minNumber2 = TWO_MIN(num3, minNumber1);
					connect[xy] = minNumber2;
					transLabel2(label, num1, num2 ,num3, labelMax);
					break;
				}
				case 8:
				{
					connect[xy] = connect[xy2 - 1];
					break;
				}
				case 9:
				{
					int num1 = connect[xy2 - 1];
					int num2 = connect[xy - 1];
					int minNumber = TWO_MIN(num1, num2);
					connect[xy] = minNumber;
					transLabel(label, num1 ,num2 , labelMax);
					break;
				}
				case 10:
				{
					int num1 = connect[xy2 - 1];
					int num2 = connect[xy2 + 1];
					int minNumber = TWO_MIN(num1, num2);
					connect[xy] = minNumber;
					transLabel(label, num1, num2, labelMax);
					break;
				}
				case 11:
				{
					int num1 = connect[xy2 - 1];
					int num2 = connect[xy2 + 1];
					int num3 = connect[xy - 1];
					int minNumber1 = TWO_MIN(num1, num2);
					int minNumber2 = TWO_MIN(num3, minNumber1);
					connect[xy] = minNumber2;
					transLabel2(label, num1, num2, num3, labelMax);
					break;
				}
				case 12:
				{
					int num1 = connect[xy2 - 1];
					int num2 = connect[xy2];
					int minNumber = TWO_MIN(num1, num2);
					connect[xy] = minNumber;
					transLabel(label, num1, num2 , labelMax);
					break;
				}
				case 13:
				{
					int num1 = connect[xy2 - 1];
					int num2 = connect[xy2];
					int num3 = connect[xy - 1];
					int minNumber1 = TWO_MIN(num1, num2);
					int minNumber2 = TWO_MIN(num3, minNumber1);
					connect[xy] = minNumber2;
					transLabel2(label, num1, num2, num3, labelMax);
					break;
				}
				case 14:
				{
					int num1 = connect[xy2 - 1];
					int num2 = connect[xy2];
					int num3 = connect[xy2 + 1];
					int minNumber1 = TWO_MIN(num1, num2);
					int minNumber2 = TWO_MIN(num3, minNumber1);
					connect[xy] = minNumber2;
					transLabel2(label, num1, num2, num3, labelMax);
					break;
				}
				case 15:
				{
					int num1 = connect[xy2 - 1];
					int num2 = connect[xy2];
					int num3 = connect[xy2 + 1];
					int num4 = connect[xy - 1];
					int minNumber1 = TWO_MIN(num1, num2);
					int minNumber2 = TWO_MIN(num3, minNumber1);
					int minNumber = TWO_MIN(num4, minNumber2);
					connect[xy] = minNumber;
					transLabel3(label, num1, num2, num3,num4, labelMax);
					break;
				}
			default:
				break;
			}	  
		}
	}
	labelImg = connectImg(Rect(1, 1, width - 2, height - 2)).clone();

}
