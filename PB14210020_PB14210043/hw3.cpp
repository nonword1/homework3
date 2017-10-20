#include “ConnectedComponentLabeling.h”

int FirstScan(Mat img, int *table)
{
	int &m = table[0];
	m = 1;
	for (int i = 1; i < img.rows - 1; i++)
	{
		for (int j = 1; j < img.cols - 1; j++)
		{
			if (img.ptr<int>(i)[j] != PIX_BACK)
			{
				int a = img.ptr<int>(i - 1)[j - 1];
				int b = img.ptr<int>(i - 1)[j];
				int c = img.ptr<int>(i - 1)[j + 1];
				int d = img.ptr<int>(i)[j - 1];
				int abcd = ((a != PIX_BACK) << 3) | ((b != PIX_BACK) << 2)
					| ((c != PIX_BACK) << 1) | (d != PIX_BACK);
				switch (abcd) //switch中的操作取自[1]Appendix A
				{
				case 0:
					img.ptr<int>(i)[j] = table[m] = m;
					m++;
					break;
				case 1:
					img.ptr<int>(i)[j] = table[d];
					break;
				case 2:
					img.ptr<int>(i)[j] = table[c];
					break;
				case 3:
					if (table[c] < table[d]) img.ptr<int>(i)[j] = table[d] = table[c];
					else img.ptr<int>(i)[j] = table[c] = table[d];
					break;
				case 4:
					img.ptr<int>(i)[j] = table[b];
					break;
				case 5:
					img.ptr<int>(i)[j] = table[d];
					break;
				case 6:
					img.ptr<int>(i)[j] = table[c];
					break;
				case 7:
					if (table[c] < table[d]) img.ptr<int>(i)[j] = table[d] = table[c];
					else img.ptr<int>(i)[j] = table[c] = table[d];
					break;
				case 8:
					img.ptr<int>(i)[j] = table[a];
					break;
				case 9:
					img.ptr<int>(i)[j] = table[d];
					break;
				case 10:
					if (table[a] < table[c]) img.ptr<int>(i)[j] = table[c] = table[a];
					else img.ptr<int>(i)[j] = table[a] = table[c];
					break;
				case 11:
					if (table[c] < table[d]) img.ptr<int>(i)[j] = table[d] = table[c];
					else img.ptr<int>(i)[j] = table[c] = table[d];
					break;
				case 12:
					img.ptr<int>(i)[j] = table[b];
					break;
				case 13:
					img.ptr<int>(i)[j] = table[d];
					break;
				case 14:
					img.ptr<int>(i)[j] = table[c];
					break;
				case 15:
					if (table[c] < table[d]) img.ptr<int>(i)[j] = table[d] = table[c];
					else img.ptr<int>(i)[j] = table[c] = table[d];
					break;
				default:
					break;
				}
			}
		}
	}
	return 0;
}

int LaterScan(Mat img, int *table, char direction)
{
	int &m = table[0];
	int unfinished = 0;
	for (int i = img.rows - 2; i > 0; i--)
	{
		for (int j = img.cols - 2; j > 0; j--)
		{
			if (img.ptr<int>(i)[j] != PIX_BACK)
			{
				int a, b, c, d;
				if (direction == 'f')
				{
					a = img.ptr<int>(i - 1)[j - 1];
					b = img.ptr<int>(i - 1)[j];
					c = img.ptr<int>(i - 1)[j + 1];
					d = img.ptr<int>(i)[j - 1];
				}
				else
				{
					a = img.ptr<int>(i + 1)[j + 1];
					b = img.ptr<int>(i + 1)[j];
					c = img.ptr<int>(i + 1)[j - 1];
					d = img.ptr<int>(i)[j + 1];
				}
				int e = img.ptr<int>(i)[j];
				int abcd = ((a != PIX_BACK) << 3) | ((b != PIX_BACK) << 2)
					| ((c != PIX_BACK) << 1) | (d != PIX_BACK);
				switch (abcd) //switch中的操作取自[1]Appendix A
				{
				case 0:
					img.ptr<int>(i)[j] = table[e];
					break;
				case 1:
					if (table[d] < table[e]) img.ptr<int>(i)[j] = table[e] = table[d];
					else img.ptr<int>(i)[j] = table[d] = table[e];
					break;
				case 2:
					if (table[c] < table[e])img.ptr<int>(i)[j] = table[e] = table[c];
					else img.ptr<int>(i)[j] = table[c] = table[e];
					break;
				case 3:
					if (table[c] < table[d] && table[c] < table[e])
						img.ptr<int>(i)[j] = table[d] = table[e] = table[c];
					else
					{
						if (table[d] < table[e])
							img.ptr<int>(i)[j] = table[c] = table[e] = table[d];
						else
							img.ptr<int>(i)[j] = table[c] = table[d] = table[e];
					}
					break;
				case 4:
					if (table[b] < table[e]) img.ptr<int>(i)[j] = table[e] = table[b];
					else img.ptr<int>(i)[j] = table[b] = table[e];
					break;
				case 5:
					if (table[d] < table[e]) img.ptr<int>(i)[j] = table[e] = table[d];
					else img.ptr<int>(i)[j] = table[d] = table[e];
					break;
				case 6:
					if (table[c] < table[e]) img.ptr<int>(i)[j] = table[e] = table[c];
					else img.ptr<int>(i)[j] = table[c] = table[e];
					break;
				case 7:
					if (table[c] < table[d] && table[c] < table[e])
						img.ptr<int>(i)[j] = table[d] = table[e] = table[c];
					else
					{
						if (table[d] < table[e])
							img.ptr<int>(i)[j] = table[c] = table[e] = table[d];
						else 
							img.ptr<int>(i)[j] = table[c] = table[d] = table[e];
					}
					break;
				case 8:
					if (table[a] < table[e]) img.ptr<int>(i)[j] = table[e] = table[a];
					else img.ptr<int>(i)[j] = table[a] = table[e];
					break;
				case 9:
					if (table[d] < table[e]) img.ptr<int>(i)[j] = table[e] = table[d];
					else img.ptr<int>(i)[j] = table[d] = table[e];
					break;
				case 10:
					if (table[a] < table[c] && table[a] < table[e])
						img.ptr<int>(i)[j] = table[c] = table[e] = table[a];
					else
					{
						if (table[c] < table[e])
							img.ptr<int>(i)[j] = table[a] = table[e] = table[c];
						else
							img.ptr<int>(i)[j] = table[a] = table[c] = table[e];
					}
					break;
				case 11:
					if (table[c] < table[d] && table[c] < table[e])
						img.ptr<int>(i)[j] = table[d] = table[e] = table[c];
					else
					{
						if (table[d] < table[e])
							img.ptr<int>(i)[j] = table[c] = table[e] = table[d];
						else
							img.ptr<int>(i)[j] = table[c] = table[d] = table[e];
					}
					break;
				case 12:
					if (table[b] < table[e]) img.ptr<int>(i)[j] = table[e] = table[b];
					else img.ptr<int>(i)[j] = table[b] = table[e];
					break;
				case 13:
					if (table[d] < table[e]) img.ptr<int>(i)[j] = table[e] = table[d];
					else img.ptr<int>(i)[j] = table[d] = table[e];
					break;
				case 14:
					if (table[c] < table[e]) img.ptr<int>(i)[j] = table[e] = table[c];
					else img.ptr<int>(i)[j] = table[c] = table[e];
					break;
				case 15:
					if (table[c] < table[d] && table[c] < table[e])
						img.ptr<int>(i)[j] = table[d] = table[e] = table[c];
					else
					{
						if (table[d] < table[e])
							img.ptr<int>(i)[j] = table[c] = table[e] = table[d];
						else
							img.ptr<int>(i)[j] = table[c] = table[d] = table[e];
					}
					break;
				default:
					break;
				}
				if (e != img.ptr<int>(i)[j]) unfinished = 1;
			}
		}
	}
	return unfinished;
}

int ustc_ConnectedComponentLabeling(Mat grayImg, Mat &labelImg, int bin_th)
{
	if (!(grayImg.data && grayImg.depth() == CV_8U && grayImg.channels() == 1
		&& labelImg.depth() == CV_32S && labelImg.channels() == 1
		&& grayImg.size() == labelImg.size()
		&& bin_th >= 0 && bin_th < 256))
		return MY_FAIL;
	Mat binImg(grayImg.size(), CV_8UC1);
	threshold(grayImg, binImg, bin_th, -PIX_OBJECT, THRESH_BINARY);
	//PIX_OBJECT定义为-1，而CV_8UC1的图像中像素不能为负数，因此这里暂时用-PIX_OBJECT表示前景像素
#ifdef SAVE_BIN_IMG
	imwrite("D:\\大四\\现代软件工程\\第1次结对编程\\binImg.jpg", 100 * binImg);
#endif
	Mat tempImg(grayImg.rows + 2, grayImg.cols + 2, CV_32SC1);
	binImg.convertTo(tempImg(Rect(1, 1, grayImg.cols, grayImg.rows)), CV_32S, -1.0, 0.0);
	//转化为CV_32SC1图像时乘以-1，用PIX_OBJECT替换binImg中的-PIX_OBJECT
	tempImg.row(0).setTo(PIX_BACK);
	tempImg.row(tempImg.rows - 1).setTo(PIX_BACK);
	tempImg.col(0).setTo(PIX_BACK);
	tempImg.col(tempImg.cols - 1).setTo(PIX_BACK);
	int *table = new int[tempImg.rows * tempImg.cols / 4];
	//连通域的个数不可能超过tempImg.rows * tempImg.cols / 4
	if (table == NULL) return -1;
	FirstScan(tempImg, table);
	int unfinished;
	char direction = 'b'; //第2次扫描为后向扫描
	do
	{
		unfinished = LaterScan(tempImg, table, direction);
		if (direction == 'b') direction = 'f';
		else direction = 'b';
		//改变扫描的方向
	} while (unfinished);
	delete[]table;
	labelImg = tempImg(Rect(1, 1, labelImg.cols, labelImg.rows)).clone();
	return MY_OK;
}

int AssembleLabel(Mat &labelImg)
{
	if (!(labelImg.data && labelImg.depth() == CV_32S && labelImg.channels() == 1)) return MY_FAIL;
	int labels[255]; //记录原标号与新标号的关系，labels[i]为原标号，将被替换为i
	int nLabel = 0;
	for (int i = 0; i < labelImg.rows; i++)
	{
		int *ithRow = labelImg.ptr<int>(i);
		for (int j = 0; j < labelImg.cols; j++)
		{
			int newLabel = 0;
			while (newLabel < nLabel && newLabel < 255 && ithRow[j] != labels[newLabel])
				newLabel++;
			if (newLabel == nLabel && nLabel < 255) labels[nLabel++] = ithRow[j];
			ithRow[j] = newLabel;
		}
	}
	return MY_OK;
}
