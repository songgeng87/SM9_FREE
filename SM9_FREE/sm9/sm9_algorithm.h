//
//  sm9_algorithm.h
//  SM9_FREE
//
//  Created by 宋赓 on 2017/3/12.
//  Copyright © 2017年 宋赓. All rights reserved.
//
#pragma once  

#ifndef sm9_algorithm_h
#define sm9_algorithm_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KDF_XOR 0
#define KDF_SM4 1

#define AKE_SENDER 0
#define AKE_RECEIVER 1


//define error code
#define LIB_NOT_INIT    1
#define NOT_ON_G1       2
#define NOT_ON_G2       3
#define SIGN_ZERO_ERROR 4
#define VERIFY_ERROR_1  5
#define VERIFY_ERROR_2  6
#define PAIRING_ERROR   7
#define BLOCK_ERROR     8
#define DECRYPT_ERROR   9

/* define Twist type */

#define SM_QUADRATIC 2
#define SM_CUBIC_M   0x3A
#define SM_CUBIC_D   0x3B
#define SM_QUARTIC_M 0x4A
#define SM_QUARTIC_D 0x4B
#define SM_SEXTIC_M  0x6A
#define SM_SEXTIC_D  0x6B

typedef struct sm9_msk_st {
    unsigned int keylen;
    unsigned char *msk;
}SM9_MSK;   //  主密钥，数据格式上不区分签名、加解密或密钥协商

typedef struct sm9_mspk_st {
    unsigned int secLevel;
    unsigned char *x1;
    unsigned char *x2;
    unsigned char *y1;
    unsigned char *y2;
}SM9_MSPK; // 签名主密钥，代表扩域上椭圆曲线的一个点

typedef struct sm9_mcpk_st {
    unsigned int secLevel;
    unsigned char *x;
    unsigned char *y;
}SM9_MCPK;  //  加密主密钥，代表基域上椭圆曲线的一个点

typedef struct sm9_mkpk_st {
    unsigned int secLevel;
    unsigned char *x;
    unsigned char *y;
}SM9_MKPK; //  密钥协商主密钥，代表基域上椭圆曲线的一个点

typedef struct sm9_pubkey_st {
    unsigned int keylen;
    unsigned char *pk;
}SM9_PK;  // 用户公钥（ID）

typedef struct sm9_signseckey_st {
    unsigned int secLevel;
    unsigned char *x;
    unsigned char *y;
}SM9_SSK; //用户签名私钥，表示基域上椭圆曲线的一个点

typedef struct sm9_cipherseckey_st {
    unsigned int secLevel;
    unsigned char *x1;
    unsigned char *x2;
    unsigned char *y1;
    unsigned char *y2;
}SM9_CSK; // 用户加解密私钥，表示扩域上椭圆曲线的一个点

typedef struct sm9_keyexseckey_st {
    unsigned int secLevel;
    unsigned char *x1;
    unsigned char *x2;
    unsigned char *y1;
    unsigned char *y2;
}SM9_KSK; //用户密钥协商私钥，表示扩域上椭圆曲线的一个点

typedef struct sm9_sign_st {
    unsigned int secLevel;
    unsigned char *h;
    unsigned char *xs;
    unsigned char *ys;
    unsigned char type;
} SM9_Sign; // 签名，h表示一段hash值，xs，ys表示基域上椭圆曲线的一个点

typedef struct sm9_cipher_st {
    unsigned int secLevel;
    unsigned long entype;
    unsigned char* x;
    unsigned char* y;
    unsigned char* c3;
    unsigned char* cp;
    unsigned int cplen;
} SM9_Cipher;  // 密文，（x,y）表示基域上椭圆曲线的一个点，C3表示验证信息，cp表示xor或者对称加密密文，entype表示采用sm4加密或xor加密

typedef struct sm9_sender_st {
    unsigned int secLevel;
    unsigned char *x;
    unsigned char *y;
}SM9_Send;  //sm9密钥协商中第一阶段（发送阶段）数据结构


//初始化sm9算法
//@seclevel 表示算法安全参数，（byte长度，标准为32）
//@t sm9复承参数t
//@q sm9基域素数q
//@a sm9椭圆曲线方程参数a
//@b sm9椭圆曲线方程参数b
//@n sm9基域上椭圆曲线的阶n
//@xp1 sm9 G1群生成元x坐标
//@yp1 sm9 G1群生成元y坐标
//@xq1 sm9 G2群生成元x坐标分量
//@xq2 sm9 G2群生成元x坐标分量
//@yq1 sm9 G2群生成元y坐标分量
//@yq2 sm9 G2群生成元y坐标分量
//@curve 等于0时，配置内置标准参数，等于1时配置前面的参数
//@TWIST_TYPE 椭圆曲线twist类型，标准为MR_SEXTIC_M
//返回值
//0 设置成功
//2 G1生成元配置错误
//3 G2生成元配置错误
int SM9_Init(unsigned int curve, int TWIST_TYPE, unsigned int seclevel,unsigned char* t, unsigned char* q, unsigned char* a, unsigned char* b, unsigned char* n, unsigned char* xp1, unsigned char* yp1, unsigned char* xq1, unsigned char* xq2, unsigned char* yq1, unsigned char* yq2);


//初始化sm9签名模块，完成预计算
//@x1 sm9签名主公钥的x分量1
//@x2 sm9签名主公钥的x分量2
//@y1 sm9签名主公钥的y分量1
//@y1 sm9签名主公钥的y分量2
//@gGtchar 初始化SM9需要用到的预计算结果
//返回值 预计算结果
//第一次调用gc = SM9_Set_Sign(x1,x2,y1,y2,NULL)，以后可以调用SM9_Set_Sign(NULL,NULL,NULL,NULL,gc)完成初始化
//可将gc写入文件或数据库，以后从文件或数据库中读取
unsigned char* SM9_Set_Sign(unsigned char* x1, unsigned char* x2, unsigned char* y1, unsigned char* y2, unsigned char* gGtchar);

//初始化sm9加密模块，完成预计算
//@x sm9加密主公钥的x分量
//@y sm9加密主公钥的y分量
//@eGtchar 初始化SM9需要用到的预计算结果
//返回值 预计算结果
//第一次调用gc = SM9_Set_Encrypt(x,y,NULL)，以后可以调用SM9_Set_Encrypt(NULL,NULL,gc)完成初始化
//可将gc写入文件或数据库，以后从文件或数据库中读取
unsigned char* SM9_Set_Encrypt(unsigned char* x, unsigned char* y, unsigned char* eGtchar);


//初始化sm9密钥协商模块，完成预计算
//@x sm9密钥协商主公钥的x分量
//@y sm9密钥协商主公钥的y分量
//@eGtchar 初始化SM9需要用到的预计算结果
//返回值 预计算结果
//第一次调用gc = SM9_Set_KeyExchange(x,y,NULL)，以后可以调用SM9_Set_KeyExchange(NULL,NULL,gc)完成初始化
//可将gc写入文件或数据库，以后从文件或数据库中读取
unsigned char* SM9_Set_KeyExchange(unsigned char* x, unsigned char* y,unsigned char* kGtchar);



//根据密钥字符串w，初始化一个主私钥
SM9_MSK SM9_MSK_New(int secLevel,unsigned char* w);

//根据安全参数secLevel初始化一个签名主公钥，标准为32
SM9_MSPK SM9_MSPK_New(int secLevel);
//根据安全参数secLevel初始化一个加密主公钥，标准为32
SM9_MCPK SM9_MCPK_New(int secLevel);
//根据安全参数secLevel初始化一个密钥协商主公钥，标准为32
SM9_MKPK SM9_MKPK_New(int secLevel);

//根据id生产一个用户公钥
SM9_PK SM9_PK_New(int len,unsigned char* w);

//根据安全参数secLevel初始化一个用户签名私钥，标准为32
SM9_SSK SM9_SSK_New(int secLevel);
//根据安全参数secLevel初始化一个用户加解密私钥，标准为32
SM9_CSK SM9_CSK_New(int secLevel);
//根据安全参数secLevel初始化一个用户密钥协商私钥，标准为32
SM9_KSK SM9_KSK_New(int secLevel);

//根据安全参数secLevel初始化一个签名结构体，标准为32
SM9_Sign SM9_Sign_New(int secLevel);
//根据安全参数secLevel初始化一个密文结构体，标准为32
SM9_Cipher SM9_Cipher_New(int secLevel);
//根据安全参数secLevel初始化一个发送结构体，标准为32
SM9_Send SM9_Send_New(int secLevel);




//释放一个主私钥
void SM9_MSK_Free(SM9_MSK *msk);
//释放一个签名主公钥
void SM9_MSPK_Free(SM9_MSPK *mpk);
//释放一个加解密主公钥
void SM9_MCPK_Free(SM9_MCPK *mpk);
//释放一个密钥协商主公钥
void SM9_MKPK_Free(SM9_MKPK *mpk);

//释放一个用户公钥
void SM9_PK_Free(SM9_PK *pk);
//释放一个用户签名私钥
void SM9_SSK_Free(SM9_SSK *sk);
//释放一个用户加解密私钥
void SM9_CSK_Free(SM9_CSK *sk);
//释放一个用户密钥协商私钥
void SM9_KSK_Free(SM9_KSK *sk);


//释放一个签名结构体
void SM9_Sign_Free(SM9_Sign *s);
//释放一个密文结构体
void SM9_Cipher_Free(SM9_Cipher *c);
//释放一个发送结构体
void SM9_Send_Free(SM9_Send *s);

//根据主私钥生成主签名公钥
//返回码
//0 生成成功
//1 系统为初始化
int SM9_GenMSignPubKey(SM9_MSK *msk, SM9_MSPK *mspk);

//根据主私钥生成主加解密公钥
//返回码
//0 生成成功
//1 系统为初始化
int SM9_GenMEncryptPubKey(SM9_MSK *msk, SM9_MCPK *mcpk);

//根据主私钥生成主密钥协商公钥
//返回码
//0 生成成功
//1 系统为初始化
int SM9_GenMKeyExchangePubKey(SM9_MSK *msk, SM9_MKPK *mcpk);

//根据主私钥，用户公钥生成用户签名私钥
//返回码
//0 生成成功
//1 系统为初始化
int SM9_GenSignSecKey(SM9_SSK *sk, SM9_PK *pk,SM9_MSK *msk);

//根据主私钥，用户公钥生成用户加解密私钥
//返回码
//0 生成成功
//1 系统为初始化
int SM9_GenEncryptSecKey(SM9_CSK *sk, SM9_PK *pk,SM9_MSK *msk);

//根据主私钥，用户公钥生成用户密钥协商私钥
//返回码
//0 生成成功
//1 系统为初始化
int SM9_GenKeyExchangeSecKey(SM9_KSK *sk, SM9_PK *pk,SM9_MSK *msk);


//=============For Signature algorithm=================
//@mes      the message for signature
//@meslen   the length of the message
//@ran      the random number for sign (must be 32 char, if it's length less than 32 , plese add 0x00 at the first)
//@sk       the secretkey for signature
//@sign     the signature

// 返回码
//0 计算成功
//1 没有完成预配置，库没有启动
//2 G1生成元配置错误
//3 G2生成元配置错误
int SM9_Signature(unsigned char* mes,unsigned int meslen,unsigned char* ran,SM9_SSK *sk, SM9_Sign *sign);

//@mes      the message for signature
//@meslen   the length of the message
//@sing     the signature
//@pk       the public key(id) of the signer
//@mpk      the master public key for signtuer, please set it as NULL

// 返回码
//0 计算成功
//1 没有完成预配置，库没有启动
//2 G1生成元配置错误
//3 G2生成元配置错误
int SM9_Verify(unsigned char *mes,unsigned int meslen, SM9_Sign *sign, SM9_PK *pk, SM9_MSPK *mpk);
//=============For Encryption algorithm=================
//
//@mes      the message for encryption
//@meslen   the length of the message
//@KDF_ID   KDF_XOR for xor encrypt function, KDF_SM4 for SM4 and the mes will be add 0xC0 at last,
//@ran      the random number for encrypt (must be 32 char, if it's length less than 32 , plese add 0x00 at the first)
//@pk       the public key(id) for the receiver
//@cip      the ciphertext

// 返回码
//0 计算成功
//1 没有完成预配置，库没有启动
//2 G1生成元配置错误
//3 G2生成元配置错误
int SM9_Encrypt(unsigned char *mes, unsigned int meslen, unsigned int KDF_ID, unsigned char *ran, SM9_PK *pk, SM9_Cipher *cip);

//@pk       the public key(id) for the receiver
//@KDF_ID  KDF_XOR for xor encrypt function, KDF_SM4 for SM4 and the mes will be add 0xC0 at last,
//@sk       the secret key for the receiver
//@cip      the ciphertext
//@mes      the plaintext
//@meslen   the length of the plaintext

// 返回码
//0 计算成功
//1 没有完成预配置，库没有启动
//2 G1生成元配置错误
//3 G2生成元配置错误
int SM9_Decrypt(SM9_PK *pk, unsigned int KDF_ID, SM9_CSK *sk, SM9_Cipher *cip,unsigned char *mes,unsigned int *meslen);
//=============For KeyExchange algorithn================

//@ran      the random number for sendstep (must be 32 char, if it's length less than 32 , plese add 0x00 at the first)
//@pk       the publickey(id) of the receiver
//@se       the send in the first step

// 返回码
//0 计算成功
//1 没有完成预配置，库没有启动
//2 G1生成元配置错误
//3 G2生成元配置错误
int SM9_SendStep(unsigned char *ran, SM9_PK *pk, SM9_Send *se);

//@ran      the random number used in sendstep (must be 32 char, if it's length less than 32 , plese add 0x00 at the first)
//@S        the "send" generater by self
//@R        the "send" from the another party
//@SP       the publickey(id) of self
//@RP       the publickey(id) of the receiver (another party)
//@ksk      the secret key for keyexchange of self
//@sklen    the length of the sk;
//@S1       hold S1 for checkstep
//@S2       send S2 to receiver for the checkstep
//@SK       the exchanged key
//@SorR     set AKE_SENDER if you are the sender party, and AKE_RECEIVER if you are the receiver party



// 返回码
//0 计算成功
//1 没有完成预配置，库没有启动
//2 G1生成元配置错误
//3 G2生成元配置错误
int SM9_ReceiveStep(unsigned char *ran,SM9_Send *S, SM9_Send *R,SM9_PK *SP, SM9_PK *RP, SM9_KSK *ksk, unsigned int sklen,unsigned char *S1, unsigned char *S2, unsigned char *SK,unsigned int SorR);

//对比R与G是否一致
//0 表示一致
//1 表示不一致
int SM9_CheckStep(unsigned char *R, unsigned char *G);

#endif /* sm9_algorithm_h */
