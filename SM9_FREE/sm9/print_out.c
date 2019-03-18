//
//  print_out.c
//  SM9_FREE
//
//  Created by 宋赓 on 2017/3/19.
//  Copyright © 2017年 宋赓. All rights reserved.
//

#include "print_out.h"

void print_hex(unsigned char *pbuf, int len)
{
	    int i;
	printf("==================\n");

	if (len==0){
        printf("0x00\n");
        return ;
    }
    
    for( i=0;i<len;i++ )
    {
        if( i && i%16==0 )
            printf("\n");
        printf("0x%02x,", pbuf[i]);
    }
    printf("\n");
    return ;
}

void print_big(_MIPD_ big x, int len){
    unsigned char xc[50];
    int n;
    //n=cotstr(x,mr_mip->IOBUFF);
    
    n = big_to_bytes(_MIPP_ len, x, (char *)xc, FALSE);
    print_hex(xc,n<len?n:len);
}

void print_zzn2(_MIPD_ zzn2 *w,int len){
	    big x;
	printf("===out  zzn2====\n");
    x = mirvar(_MIPP_ 0);
    redc(_MIPP_ w->a,x);
    print_big(_MIPP_ x, len);
    redc(_MIPP_ w->b,x);
    print_big(_MIPP_ x, len);
    mirkill(x);
}

void print_zzn12(_MIPD_ zzn12 *w,int len){
    printf("===out  zzn12====\n");
    //print_big(_MIPP_ w->a.a.a, 32);
    print_zzn2(_MIPP_ &(w->a.a), 32);
}


void print_ecn2(_MIPD_ ecn2 *q, int len){
    printf("===out  ecn2====\n");
    print_zzn2(_MIPP_ &(q->x), 32);
    print_zzn2(_MIPP_ &(q->y), 32);
}

void print_ecn(_MIPD_ ecn *q, int len){
    big x;
    printf("===out  ecn====\n");
	x = mirvar(_MIPP_ 0);
    redc(_MIPP_ q->X,x);
    print_big(_MIPP_ x, len);
    redc(_MIPP_ q->Y,x);
    print_big(_MIPP_ x, len);
    mirkill(x);
}
