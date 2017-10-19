int ustc_ConnectedComponentLabeling(Mat grayImg, Mat &labelImg, int bin_th)
{
	//defend start:
	if (NULL == grayImg.data || NULL == labelImg.data)
	{
		cout << "Empty gray image!" << endl;
		return MY_FAIL;
	}
	if (labelImg.cols != grayImg.cols || labelImg.rows != grayImg.rows)
	{
		cout << "label and gray image not match!" << endl;
		return MY_FAIL;
	}
	if (1 != grayImg.channels() || 1 != labelImg.channels())
	{
		cout << "wrong channels!" << endl;
		return MY_FAIL;
	}
	if (bin_th < 0 || bin_th>255)
	{
		cout << "bin_th error!" << endl;
		return MY_FAIL;
	}
	if (1 != grayImg.elemSize1() || 4 != labelImg.elemSize1())
	{
		cout << "Image elemsize error!" << endl;
		return MY_FAIL;
	}
	//defend finish.
	int marknum[200000] = { 0 };
	int* listPoint =marknum;
	int height = grayImg.rows, width = grayImg.cols;
	Mat binaryImg(height, width, CV_8UC1, Scalar(0));
	threshold(grayImg, binaryImg, bin_th, 255, cv::THRESH_BINARY);
	Mat bigImg(height+2, width+2, CV_8UC1, Scalar(0));

	for (int rows_i = 1; rows_i <= height; rows_i++)
		for (int cols_j = 1; cols_j <= width; cols_j++)
		{
			bigImg.data[rows_i*(width + 2) + cols_j] = binaryImg.data[(rows_i - 1)*width + cols_j - 1];
		}

	int signNum = 0;
	for (int rows_i = 1; rows_i <= height; rows_i++)
		for (int cols_j = 1; cols_j <= width; cols_j++)
		{
			int bigplace = rows_i*(width + 2) + cols_j;
			int subplace = (rows_i - 1)*width + cols_j - 1;
			if (bigImg.data[bigplace]) 
			{
				int a=0, b=0, c=0, d=0;
				uchar *bigpoint = &bigImg.data[bigplace];
				int *subpoint = &((int*)labelImg.data)[subplace];
				d = *(--bigpoint);
				bigpoint -= (width + 2);
				a = *(bigpoint++);
				b = *(bigpoint++);
				c = *(bigpoint);
				int temp = (bool)a * 8 + (bool)b * 4 + (bool)c * 2 + (bool)d;
				a = 0;
				b = 0;
				c = 0;
				d = 0;
				int fb = 0;
				int fc = 0;
				int fd = 0;
				int fa = 0;
				switch (temp)
				{
				case 0:
					*(subpoint) = ++signNum;
					listPoint[signNum] = signNum;
					break;
				case 1:
					d = *(subpoint - 1);
					*(subpoint) = d;
					break;
				case 2:
					c = *(subpoint - width + 1);
					*(subpoint) = c;
					break;
				case 3:
					c = *(subpoint - width + 1);
					d = *(subpoint - 1);
					fc = listPoint[c];
					fd = listPoint[d];
					if(fc==fd)
						*(subpoint) = c;
					else if (fc < fd) 
					{
						*(subpoint) = c;
						ustc_refresh(c,d, signNum, listPoint);
					}
					else if (fc > fd)
					{
						*(subpoint) = d;
						ustc_refresh(d,c, signNum, listPoint);
					}
					break;
				case 4:
					b = *(subpoint - width);
					*(subpoint) =b;
					break;
				case 5:
					b = *(subpoint - width);
					d = *(subpoint - 1);
				    fb = listPoint[b];
				    fd = listPoint[d];
					if (fb == fd)
						*(subpoint) = b;
					else if (fb < fd)
					{
						*(subpoint) = b;
						ustc_refresh(b, d, signNum, listPoint);
					}
					else if (fb> fd)
					{
						*(subpoint) = d;
						ustc_refresh(d, b, signNum, listPoint);
					}
					break;
				case 6:
					b = *(subpoint - width);
					c = *(subpoint - width + 1);
					fb = listPoint[b];
					fc = listPoint[c];
					if (fb == fc)
						*(subpoint) = b;
					else if (fb <fc)
					{
						*(subpoint) = b;
						ustc_refresh(b, c, signNum, listPoint);
					}
					else if (fb > fc)
					{
						*(subpoint) = c;
						ustc_refresh(c, b, signNum, listPoint);
					}
					break;
				case 7:
					b = *(subpoint - width);
					c = *(subpoint - width + 1);
					d = *(subpoint - 1);
					fb = listPoint[b];
					fc = listPoint[c];
					fd = listPoint[d];
				    temp = (bool)(fc - fb) * 2 + (bool)(fd - fb);
					switch (temp)
					{
					case 0:
						*(subpoint) = b;
						break;
					case 1:
						if (fd > fb)
						{
							*(subpoint) = b;
							ustc_refresh(b, d, signNum, listPoint);
						}
						else if (fd < fb)
						{
							*(subpoint) = d;
							ustc_refresh(d, b, signNum, listPoint);
						}
						break;
					case 2:
					case 3:
						if (fc > fb)
						{
							*(subpoint) = b;
							ustc_refresh(b, c, signNum, listPoint);
						}
						else if (fc < fb)
						{
							*(subpoint) = c;
							ustc_refresh(c, b, signNum, listPoint);
						}
						break;
					}
					break;
				case 8:
					a = *(subpoint - width - 1);
					*(subpoint)= a;
					break;
				case 9:
					a = *(subpoint - width - 1);
					d = *(subpoint - 1);
					fd = listPoint[d];
					fa = listPoint[a];
					if (fa == fd)
						*(subpoint) = a;
					else if (fa < fd)
					{
						*(subpoint) = a;
						ustc_refresh(a, d, signNum, listPoint);
					}
					else if (fa > fd)
					{
						*(subpoint) = d;
						ustc_refresh(d, a, signNum, listPoint);
					}
					break;
				case 10:
					a = *(subpoint - width - 1);
					c = *(subpoint - width + 1);
					fc = listPoint[c];
					fa = listPoint[a];
					if (fa == fc)
						*(subpoint) = a;
					else if (fa < fc)
					{
						*(subpoint) = a;
						ustc_refresh(a, c, signNum, listPoint);
					}
					else if (fa> fc)
					{
						*(subpoint) = c;
						ustc_refresh(c, a, signNum, listPoint);
					}
					break;
				case 11:
					a = *(subpoint - width - 1);
					c = *(subpoint - width + 1);
					d = *(subpoint - 1);
					fc = listPoint[c];
					fd = listPoint[d];
					fa = listPoint[a];
				    temp = (bool)(fc - fa) * 2 + (bool)(fd - fa);
					switch (temp)
					{
					case 0:
						*(subpoint) = a;
						break;
					case 1:
						if (fd > fa)
						{
							*(subpoint) = a;
							ustc_refresh(a, d, signNum, listPoint);
						}
						else if (fd < fa)
						{
							*(subpoint) = d;
							ustc_refresh(d, a, signNum, listPoint);
						}
						break;
					case 2:
					case 3:
						if (fc > fa)
						{
							*(subpoint) = a;
							ustc_refresh(a, c, signNum, listPoint);
						}
						else if (fc < fa)
						{
							*(subpoint) = c;
							ustc_refresh(c,a, signNum, listPoint);
						}
						break;
					}
					break;
				case 12:
					a = *(subpoint - width - 1);
					b = *(subpoint - width);
					fb = listPoint[b];
					fa = listPoint[a];
					if (fa ==fb)
						*(subpoint) = a;
					else if (fa <fb)
					{
						*(subpoint) = a;
						ustc_refresh(a,b, signNum, listPoint);
					}
					else if (fa > fb)
					{
						*(subpoint) = b;
						ustc_refresh(b, a, signNum, listPoint);
					}
					break;
				case 13:
					a = *(subpoint - width - 1);
					b = *(subpoint - width);
					d = *(subpoint - 1);
					fb = listPoint[b];
					fd = listPoint[d];
					fa = listPoint[a];
					temp = (bool)(fb - fa) * 2 + (bool)(fd - fa);
					switch (temp)
					{
					case 0:
						*(subpoint) = a;
						break;
					case 1:
						if (fd > fa)
						{
							*(subpoint) = a;
							ustc_refresh(a, d, signNum, listPoint);
						}
						else if (fd < fa)
						{
							*(subpoint) = d;
							ustc_refresh(d, a, signNum, listPoint);
						}
						break;
					case 2:
					case 3:
						if (fb > fa)
						{
							*(subpoint) = a;
							ustc_refresh(a, b, signNum, listPoint);
						}
						else if (fb < fa)
						{
							*(subpoint) = b;
							ustc_refresh(b, a, signNum, listPoint);
						}
						break;
					}
					break;
				case 14:
					a = *(subpoint - width - 1);
					b = *(subpoint - width);
					c = *(subpoint - width + 1);
					fb = listPoint[b];
					fc = listPoint[c];
				        fa = listPoint[a];
					temp = (bool)(fb - fa) * 2 + (bool)(fc - fa);
					switch (temp)
					{
					case 0:
						*(subpoint) = a;
						break;
					case 1:
						if (fc > fa)
						{
							*(subpoint) = a;
							ustc_refresh(a,c, signNum, listPoint);
						}
						else if (fc < fa)
						{
							*(subpoint) = c;
							ustc_refresh(c, a, signNum, listPoint);
						}
						break;
					case 2:
					case 3:
						if (fb > fa)
						{
							*(subpoint) = a;
							ustc_refresh(a, b, signNum, listPoint);
						}
						else if (fb < fa)
						{
							*(subpoint) = b;
							ustc_refresh(b, a, signNum, listPoint);
						}
						break;
					}
					break;
				case 15:	
					a = *(subpoint - width - 1);
					b = *(subpoint - width);
					c = *(subpoint - width + 1);
					d = *(subpoint - 1);
					fb = listPoint[b];
					fc = listPoint[c];
					fd = listPoint[d];
					fa = listPoint[a];
					temp = (bool)(fb - fa) * 4 + (bool)(fc - fa) * 2 + (bool)(fd - fa);
					switch (temp)
					{
					case 0:
						*(subpoint) = a;
						break;
					case 1:
					case 3:
					case 5:
					case 7:
						if (fa<fd)
						{
							*(subpoint) = a;
							ustc_refresh(a, d, signNum, listPoint);
						}
						if (fa>fd)
						{
							*(subpoint) = d;
							ustc_refresh(d, a, signNum, listPoint);
						}
						break;
					case 2:
					case 6:
						if (fa<fc)
						{
							*(subpoint) = a;
							ustc_refresh(a, c, signNum, listPoint);
						}
						if (fa>fc)
						{
							*(subpoint) = c;
							ustc_refresh(c, a, signNum, listPoint);
						}
						break;
					case 4:
						if (fa<fb)
						{
							*(subpoint) = a;
							ustc_refresh(a, b, signNum, listPoint);
						}
						if (fa>fb)
						{
							*(subpoint) = b;
							ustc_refresh(b, a, signNum, listPoint);
						}
						break;
					}
					break;
				}
			}
		}
		
		for (int rows_i = 0; rows_i < height; rows_i++)
			for (int cols_j = 0; cols_j < width; cols_j++)
			{
				int val =((int*)labelImg.data)[rows_i*width + cols_j];
				switch (val)
				{
				case 0:
					break;
				default :
					((int*)labelImg.data)[rows_i*width + cols_j] =*(listPoint+val);
				}
			}
	return MY_OK;
}
///////////////////////////////////////////////////////////////////////////



int ustc_refresh(int num1, int num2,int len,int *point)//num1 min  mun2 max
{
	num1 = *(point + num1);
	num2 = *(point + num2);
	for (int i = 1; i <= len; i++)
	{
		if (point[i] == num2)
		{
			point[i] = num1;
		}
	}
	return MY_OK;
}
