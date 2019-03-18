//
//  sm4.h
//  SM9_FREE
//
//  Created by 宋赓 on 2017/3/25.
//  Copyright © 2017年 宋赓. All rights reserved.
//

#ifndef sm4_h
#define sm4_h

#define keyIntLen 32

#define ENCRYPT 0
#define DECRYPT 1
#include <stdio.h>
//SM4密钥扩展函数，Key为SM4加解密密钥，rk为扩展后的密钥，CryptFlag为ENCRYPT时为加密扩展，DECRYPT为解密扩展
void SM4KeyExt(unsigned char *Key, unsigned int *rk, unsigned int CryptFlag);

//SM4 ECB加密模式，Input为输入，Output为输出，rk为扩展后的密钥
void SM4ECBEnc(unsigned char *Input, unsigned char *Output, unsigned int *rk);

//SM4 ECB解密模式，Input为输入，Output为输出，rk为扩展后的密钥
void SM4ECBDec(unsigned char *Input, unsigned char *Output, unsigned int *rk);

#endif /* sm4_h */
