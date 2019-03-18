//
//  print_out.h
//  SM9_FREE
//
//  Created by 宋赓 on 2017/3/19.
//  Copyright © 2017年 宋赓. All rights reserved.
//

#ifndef print_out_h
#define print_out_h

#include <stdio.h>
#include "../miracl/miracl.h"
#include "smzzn12.h"
#include "sm9_utils.h"
void print_hex(unsigned char *pbuf, int len); //打印一个字符串pbuf，长度为len Byte
void print_big(_MIPD_ big x, int len);       //打印一个大整数x，长度为len Byte
void print_zzn2(_MIPD_ zzn2 *w,int len);     //打印一个二阶扩域数w，长度为len Byte
void print_zzn12(_MIPD_ zzn12 *w,int len);   //打印一个十二阶扩域w的最后一个分量，长度为len Byte
void print_ecn2(_MIPD_ ecn2 *q, int len);    //打印一个扩域上的椭圆曲线点q，分量长度为len Byte
void print_ecn(_MIPD_ ecn *q, int len);      //打印椭圆曲线的一个点q，分量长度为len Byte
#endif /* print_out_h */
