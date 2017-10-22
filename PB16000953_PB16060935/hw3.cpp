#include “ConnectedComponentLabeling.h”

//功能说明：找到二值图像中所有的连通域，并且进行标记
//grayImg：用来进行连通域分析的目标图，8UC1
//labelImg：用来存储标记结果的图像，CV_32SC1，尺寸与grayImg一致
//bin_th：用来对灰度图进行二值化的阈值
//返回值：MY_OK或者MY_FAIL

int ustc_ConnectedComponentLabeling(
	Mat grayImg,
	Mat& labelImg,
	int bin_th)
{
	//判断图片是否为空
	if (NULL == grayImg.data || NULL == labelImg.data)
	{
		cout << "one of the IMG is NULL" << endl;
		return MY_FAIL;
	}
	//判断阈值是否合法
	if (bin_th < 0 || bin_th>255)
	{
		cout << "bin_th ERROR" << endl;
	}

	int width = grayImg.cols;
	int height = grayImg.rows;

	//判断灰度图和连通域标记图是否大小一致
	if (labelImg.cols != width || labelImg.rows != height)
	{
		cout << "the size of two IMGs not Match" << endl;
	}
	//此图用于储存二值化图像
	Mat binaryImg(height, width, CV_8UC1);
	//ustc_Threshold(grayImg, binaryImg, bin_th);
	threshold(grayImg,binaryImg,bin_th,1,THRESH_BINARY);

	int width_ex = width + 2;
	int height_ex = height + 2;
	//将二值化图像向外延拓长度为1的一圈得到拓展图
	Mat expand_binary(height_ex, width_ex, CV_8UC1);
	expand_binary.setTo(0);//二值化拓展图初始化清零
						   //将二值化小图粘贴到二值化拓展图中央
	Mat Img_ROI = expand_binary(Rect(1, 1, width, height));
	binaryImg.copyTo(Img_ROI);

#ifdef IMG_SHOW
	namedWindow("expand_binary", 0);
	imshow("expand_binary", expand_binary);
	waitKey();
#endif

	//分配一个足够的空间给T数组，用于储存各标号之间的联通关系
	int* T = NULL;
	T = new int[width*height / 4 + 1];
	memset(T, 0, (width*height / 4 + 1) * sizeof(int));//T数组清零
	Mat labelImg_ex(height_ex, width_ex, CV_32SC1);
	labelImg_ex.setTo(0);//连通域拓展图初始化清零

						 //在拓展图中扫描的结束坐标
	int end_x = width + 1;
	int end_y = height + 1;

	//当前最大的标号值
	int current_max_label = 0;

	for (int row_y = 1; row_y < end_y; row_y++)
	{
		//label_ex图上bcd位置的width*height
		int bcd_temp = (row_y - 1)*width_ex;

		for (int col_x = 1; col_x < end_x; col_x++)
		{
			//计算扫描器abcd在拓展图上的相对位置
			int c_pos = bcd_temp + col_x;
			int b_pos = c_pos - 1;
			int d_pos = c_pos + 1;
			int a_pos = b_pos + width_ex;
			int e_pos = a_pos + 1;

			//若当前扫描到的像素点为背景色，跳过该点
			if (0 == expand_binary.data[e_pos])
				continue;

			//按二值化的图片中扫描器上abcd点的像素值进行分类
			int a = expand_binary.data[a_pos];
			int b = expand_binary.data[b_pos];
			int c = expand_binary.data[c_pos];
			int d = expand_binary.data[d_pos];
			int type = (a << 3) + (b << 2) + (c << 1) + d;

			//记下连通域拓展图上当前扫描器abcd位置的标号
			int label_a = ((int*)labelImg_ex.data)[a_pos];
			int label_b = ((int*)labelImg_ex.data)[b_pos];
			int label_c = ((int*)labelImg_ex.data)[c_pos];
			int label_d = ((int*)labelImg_ex.data)[d_pos];

			//扫描器上的abcd的标号 所连通的的标号 中的最小值
			int T_min = 0;
			//扫描器上的abcd标号的最小值


			switch (type)//分情况讨论
			{
			case 0://孤立点
				current_max_label += 1;
				((int*)labelImg_ex.data)[e_pos] = current_max_label;
				T[current_max_label] = current_max_label;
				break;
				//1.标记非孤立点，将扫描点标记为扫描器上最小的标号，
				//2.T数组的更新，将T数组上扫描器上abcd当前非背景色的点的标号与其中最小的标号建立联系
			case 1://d的相邻点
				((int*)labelImg_ex.data)[e_pos] = T[label_d];
				break;
			case 2://c的相邻点
				((int*)labelImg_ex.data)[e_pos] = T[label_c];
				break;
			case 3://cd的相邻点
				   //比较与cd标号联系的标号
				T_min = T[label_c] < T[label_d] ? T[label_c] : T[label_d];
				//比较cd标号
				((int*)labelImg_ex.data)[e_pos] = T_min;
				T[label_c] = T_min;
				T[label_d] = T_min;
				break;
			case 4://b的相邻点
				((int*)labelImg_ex.data)[e_pos] = T[label_b];
				break;
			case 5://bd的相邻点
				   //比较与bd标号联系的标号
				T_min = T[label_b] < T[label_d] ? T[label_b] : T[label_d];
				((int*)labelImg_ex.data)[e_pos] = T_min;
				T[label_b] = T_min;
				T[label_d] = T_min;
				break;
			case 6:////bc的相邻点
				   //比较与bc标号联系的标号
				T_min = T[label_b] < T[label_c] ? T[label_b] : T[label_c];
				((int*)labelImg_ex.data)[e_pos] = T_min;
				T[label_b] = T_min;
				T[label_c] = T_min;
				break;
			case 7://bcd的相邻点
				   //比较与bcd标号联系的标号
				T_min = T[label_b] < T[label_c] ? T[label_b] : T[label_c];
				T_min = T_min < T[label_d] ? T_min : T[label_d];
				((int*)labelImg_ex.data)[e_pos] = T_min;
				T[label_b] = T_min;
				T[label_c] = T_min;
				T[label_d] = T_min;
				break;
			case 8://a的相邻点
				   //cout << label_a<<" " << e_pos-width_ex << endl;
				((int*)labelImg_ex.data)[e_pos] = T[label_a];
				break;
			case 9://ad的相邻点
				   //比较与ad标号联系的标号
				T_min = T[label_a] < T[label_d] ? T[label_a] : T[label_d];
				((int*)labelImg_ex.data)[e_pos] = T_min;
				T[label_a] = T_min;
				T[label_d] = T_min;
				break;
			case 10://ac的相邻点
					//比较与ac标号联系的标号
				T_min = T[label_a] < T[label_c] ? T[label_a] : T[label_c];
				((int*)labelImg_ex.data)[e_pos] = T_min;
				T[label_a] = T_min;
				T[label_c] = T_min;
				break;
			case 11://acd的相邻点
					//比较与acd标号联系的标号
				T_min = T[label_a] < T[label_c] ? T[label_a] : T[label_c];
				T_min = T_min < T[label_d] ? T_min : T[label_d];
				((int*)labelImg_ex.data)[e_pos] = T_min;

				T[label_a] = T_min;
				T[label_c] = T_min;
				T[label_d] = T_min;
				break;
			case 12://ab的相邻点
					//比较与ab标号联系的标号
				T_min = T[label_a] < T[label_b] ? T[label_a] : T[label_b];
				((int*)labelImg_ex.data)[e_pos] = T_min;
				T[label_a] = T_min;
				T[label_b] = T_min;
				break;
			case 13://abd的相邻点
					//比较与abd标号联系的标号
				T_min = T[label_a] < T[label_b] ? T[label_a] : T[label_b];
				T_min = T_min < T[label_d] ? T_min : T[label_d];
				((int*)labelImg_ex.data)[e_pos] = T_min;
				T[label_a] = T_min;
				T[label_b] = T_min;
				T[label_d] = T_min;
				break;
			case 14://abc的相邻点
					//比较与abc标号联系的标号
				T_min = T[label_a] < T[label_b] ? T[label_a] : T[label_b];
				T_min = T_min < T[label_c] ? T_min : T[label_c];
				((int*)labelImg_ex.data)[e_pos] = T_min;
				T[label_a] = T_min;
				T[label_b] = T_min;
				T[label_c] = T_min;
				break;
			case 15://abcd的相邻点
					//比较与abcd标号联系的标号
				T_min = T[label_a] < T[label_b] ? T[label_a] : T[label_b];
				T_min = T_min < T[label_c] ? T_min : T[label_c];
				T_min = T_min < T[label_d] ? T_min : T[label_d];
				((int*)labelImg_ex.data)[e_pos] = T_min;
				T[label_a] = T_min;
				T[label_b] = T_min;
				T[label_c] = T_min;
				T[label_d] = T_min;
				break;
			}
		}
	}//end 1st ROW

	 //labelImg = labelImg_ex(Rect(1, 1, width, height)).clone();
	 /*调试第一次循环后的显示*/
	 /*
	 labelImg_ex(Rect(1, 1, width, height)).copyTo(labelImg);
	 for (int i = 0; i < 50; i++)
	 {
	 for (int j = 0; j < 50; j++)
	 {
	 printf("%2d", ((int*)labelImg.data)[i*width + j]);
	 }
	 printf("\n");
	 }
	 cout << "*****************" << endl;
	 */
	 
	 

	 //第二次循环，将T数组中联通的标号统一变为较小的标号
	for (int row_y = 1; row_y < end_y; row_y++)
	{
		int temp = width_ex*row_y;
		for (int col_x = 1; col_x < end_x; col_x++)
		{
			int pos = temp + col_x;
			int label = ((int*)labelImg_ex.data)[pos];
			//if(label!=0)
			((int*)labelImg_ex.data)[pos] = T[label];
		}
	}//end the 2nd ROW



	 //结果返回实参
	labelImg_ex(Rect(1, 1, width, height)).copyTo(labelImg);

	return MY_OK;


}//end ConnectedComponentLabeling
