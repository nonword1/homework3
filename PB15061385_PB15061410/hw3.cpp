//
//  connected_component_labeling.cpp
//  Connected-Component Labeling
//
//  Created by Lixian on 2017/10/12.
//  Copyright © 2017年 Lixian. All rights reserved.
//

/* Includes ------------------------------------------------------------------*/
#include "ConnectedComponentLabeling.h"

/* Private subfunctions ------------------------------------------------------*/
static inline void merge_operation(int *next, int *rtable, int *tail, int u, int v) {
    int i = v;
    while (i != -1) {
        rtable[i] = u;
        i = next[i];
    }
    next[tail[u]] = v;
    tail[u] = tail[v];
}

static void resolve(int c1, int c4, int *rtable, int *next, int *tail) {
    int u = rtable[c1], v = rtable[c4];
    if (u < v)
        merge_operation(next, rtable, tail, u, v);
    else if (u > v)
        merge_operation(next, rtable, tail, v, u);
}

static void epoch1(cv::Mat &labelImg, int *rtable, int *next, int *tail, int &label_value) {
    
    const int rows = labelImg.rows;
    const int cols = labelImg.cols;
    const int Vb = 0;
    
    int rows_index, cols_index;
    int c1, c2, c3, c4;
    int *now1, *now2, *now3, *now4;
    
    for (rows_index = 1; rows_index < rows - 1; rows_index++) {
        int *pl = labelImg.ptr<int>(rows_index); //present prosition
        int *pt = labelImg.ptr<int>(rows_index - 1);//last_row prosition
        cols_index = 1;
        //extend_loop
        for (; cols_index < cols - 1 - 4; cols_index += 4) {
            //first
            if (*(now1 = (pl + cols_index))) {
                c3 = pt[cols_index];
                
                if (c3 != Vb)
                    *now1 = c3;
                else {
                    c4 = pt[cols_index + 1];
                    c1 = pl[cols_index - 1];
                    if (c1) {
                        *now1 = c1;
                        if (c4) {
                            resolve(c1, c4, rtable, next, tail);
                        }
                    } else {
                        c2 = pt[cols_index - 1];
                        if (c2) {
                            *now1 = c2;
                            if (c4) {
                                resolve(c2, c4, rtable, next, tail);
                            }
                        } else if (c4)
                            *now1 = c4;
                        else {
                            *now1 = label_value;
                            rtable[label_value] = label_value;
                            next[label_value] = -1;
                            tail[label_value] = label_value;
                            label_value++;
                        }
                    }
                }
            }
            //second
            if (*(now2 = (pl + cols_index + 1))) {
                c3 = pt[cols_index + 1];
                
                if (c3 != Vb)
                    *now2 = c3;
                else {
                    c4 = pt[cols_index + 2];
                    c1 = pl[cols_index];
                    if (c1) {
                        *now2 = c1;
                        if (c4) {
                            resolve(c1, c4, rtable, next, tail);
                        }
                    } else {
                        c2 = pt[cols_index];
                        if (c2) {
                            *now2 = c2;
                            if (c4) {
                                resolve(c2, c4, rtable, next, tail);
                            }
                        } else if (c4)
                            *now2 = c4;
                        else {
                            *now2 = label_value;
                            rtable[label_value] = label_value;
                            next[label_value] = -1;
                            tail[label_value] = label_value;
                            label_value++;
                        }
                    }
                }
            }
            //third
            if (*(now3 = (pl + cols_index + 2))) {
                c3 = pt[cols_index + 2];
                
                if (c3 != Vb)
                    *now3 = c3;
                else {
                    c4 = pt[cols_index + 3];
                    c1 = pl[cols_index + 1];
                    if (c1) {
                        *now3 = c1;
                        if (c4) {
                            resolve(c1, c4, rtable, next, tail);
                        }
                    } else {
                        c2 = pt[cols_index + 1];
                        if (c2) {
                            *now3 = c2;
                            if (c4) {
                                resolve(c2, c4, rtable, next, tail);
                            }
                        } else if (c4)
                            *now3 = c4;
                        else {
                            *now3 = label_value;
                            rtable[label_value] = label_value;
                            next[label_value] = -1;
                            tail[label_value] = label_value;
                            label_value++;
                        }
                    }
                }
            }
            //fourth
            if (*(now4 = (pl + cols_index + 3))) {
                c3 = pt[cols_index + 3];
                
                if (c3 != Vb)
                    *now4 = c3;
                else {
                    c4 = pt[cols_index + 4];
                    c1 = pl[cols_index + 2];
                    if (c1) {
                        *now4 = c1;
                        if (c4) {
                            resolve(c1, c4, rtable, next, tail);
                        }
                    } else {
                        c2 = pt[cols_index + 2];
                        if (c2) {
                            *now4 = c2;
                            if (c4) {
                                resolve(c2, c4, rtable, next, tail);
                            }
                        } else if (c4)
                            *now4 = c4;
                        else {
                            *now4 = label_value;
                            rtable[label_value] = label_value;
                            next[label_value] = -1;
                            tail[label_value] = label_value;
                            label_value++;
                        }
                    }
                }
            }
        }
        //rest
        for (; cols_index < cols - 1; cols_index++) {
            
            if (*(now1 = (pl + cols_index))) {
                
                c3 = pt[cols_index];
                
                if (c3 != Vb)
                    *now1 = c3;
                else {
                    c4 = pt[cols_index + 1];
                    c1 = pl[cols_index - 1];
                    if (c1) {
                        *now1 = c1;
                        if (c4) {
                            resolve(c1, c4, rtable, next, tail);
                        }
                    } else {
                        c2 = pt[cols_index - 1];
                        if (c2) {
                            *now1 = c2;
                            if (c4) {
                                resolve(c2, c4, rtable, next, tail);
                            }
                        } else if (c4)
                            *now1 = c4;
                        else {
                            *now1 = label_value;
                            rtable[label_value] = label_value;
                            next[label_value] = -1;
                            tail[label_value] = label_value;
                            label_value++;
                        }
                    }
                }
            }
        }
        //end_of_cols
    }
}

static void epoch2(cv::Mat &labelImg_local, int *rtable, int label_value) {
    
    if (label_value <= 256) {
        //Fast look up table
        Mat lut(1, 256, CV_8UC1, Scalar(0));
        uchar *pl = lut.data;
        for (int i = 1; i < label_value; i++) {
            pl[i] = rtable[i];
        }
        labelImg_local.convertTo(labelImg_local, CV_8UC1);
        LUT(labelImg_local, lut, labelImg_local);
    } else {
        const int img_size = labelImg_local.rows * labelImg_local.cols;
        
        int i = 0;
        int *pl = (int *) labelImg_local.data;
        rtable[0] = 0;
        //extend_loop
        for (; i < img_size - 8; i += 8) {
            pl[i] = rtable[pl[i]];
            pl[i + 1] = rtable[pl[i + 1]];
            pl[i + 2] = rtable[pl[i + 2]];
            pl[i + 3] = rtable[pl[i + 3]];
            pl[i + 4] = rtable[pl[i + 4]];
            pl[i + 5] = rtable[pl[i + 5]];
            pl[i + 6] = rtable[pl[i + 6]];
            pl[i + 7] = rtable[pl[i + 7]];
        }
        //rest
        for (; i < img_size; i++) {
            pl[i] = rtable[pl[i]];
        }
    }
}

/* Private functions ---------------------------------------------------------*/
int ustc_ConnectedComponentLabeling(Mat grayImg, Mat &labelImg, int bin_th) {
    //Protect
    if (nullptr == grayImg.data) return MY_FAIL;
    CV_Assert(grayImg.type() == CV_8UC1);
    
    //Initialize
    Mat thresholdImg, labelImg_local;
    
    threshold(grayImg, thresholdImg, bin_th, 1, THRESH_BINARY);
    copyMakeBorder(thresholdImg, thresholdImg, 1, 1, 1, 1, 0);  //Add one round
    
    const int rows = thresholdImg.rows;
    const int cols = thresholdImg.cols;
    const int labels_size = rows * cols / 4;
    
    int *rtable = new int[labels_size + 1];
    int *next = new int[labels_size + 1];
    int *tail = new int[labels_size + 1];
    
    thresholdImg.convertTo(labelImg_local, CV_32SC1);
    
    int label_value = 1;
    //Start
    epoch1(labelImg_local, rtable, next, tail, label_value);
    
    //Final
    epoch2(labelImg_local, rtable, label_value);
    
    //reshape
    //Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height)
    labelImg_local(Rect(1, 1, cols - 2, rows - 2)).convertTo(labelImg, CV_32SC1);
    
    delete[] rtable;
    delete[] next;
    delete[] tail;
    return MY_OK;
}
