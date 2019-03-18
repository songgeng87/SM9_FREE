/*
 *   MIRACL compiler/hardware definitions - mirdef.h
 *   Copyright (c) 1988-2001 mus Software Ltd.
 */





//32bits 在32位环境下使用，并用mrmuldv.c32替换mrmuldv.c文件
/*
#define MR_GENERIC_AND_STATIC
#define MR_GENERIC_MT
#define MIRACL 32
#define MR_LITTLE_ENDIAN
//#define MR_BIG_ENDIAN  //大端环境
#define mr_utype int
#define MR_IBITS 32
#define MR_LBITS 32
#define mr_unsign32 unsigned int
#define mr_dltype long long
#define mr_unsign64 unsigned long long
#define MR_NOASM
#define MAXBASE ((mr_small)1<<(MIRACL-1))
*/

//在64位环境下使用，使用gcc编译时，需要用mrmuldv.c64替换mrmuldv.c文件
//使用vs编译时需要使用mrduldv.w64替换mrmuldv.c文件，并定义 “#define MR_NO_INTRINSICS”
#define MR_GENERIC_MT
#define MR_LITTLE_ENDIAN
#define MIRACL 64
#define mr_utype long
#define mr_unsign64 unsigned long
#define MR_IBITS 32
#define MR_LBITS 64
#define mr_unsign32 unsigned int
#define MR_FLASH 52
#define MAXBASE ((mr_small)1<<(MIRACL-1))
#define MR_BITSINCHAR 8


