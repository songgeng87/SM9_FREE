#include <stdio.h>
#include <string.h>
#include <time.h>


#include "sm9/sm9_algorithm.h"
#include "sm9/print_out.h"

#define TEST 100

int main(int argc, char **argv)
{
    printf("begin test \n");
//===========签名用到的参数==========
    unsigned char cks[32] = "\x86\xDC\xD6\x4F\xEB\x81\xA7\x19\x63\x59\x59\xF1\xA5\xC2\xF9\x88\xBD\x39\x43\x1B\x08\xA8\x63\xF0\x42\x8D\x21\xDF\xFA\xF2\xBF\x89";
    unsigned char id[5] = {"\x41\x6C\x69\x63\x65"};
    unsigned char rand[32] = {"\x1A\x23\x29\x77\xBA\x9F\xA2\xD1\xC5\x58\xF2\xD4\x67\xFE\x7B\xE7\x04\x05\x41\x26\x73\xF8\xBE\x64\x9B\xBD\xD4\xA0\x95\xBE\x1B\x4B"};
    unsigned char msg[20] = {"\x43\x68\x69\x6E\x65\x73\x65\x20\x49\x42\x53\x20\x73\x74\x61\x6E\x64\x61\x72\x64"};
//========加密用到的参数===================
    unsigned char cmsg[20] = {"\x43\x68\x69\x6E\x65\x73\x65\x20\x49\x42\x45\x20\x73\x74\x61\x6E\x64\x61\x72\x64"};
    unsigned char eks[32] = {"\x00\x01\xED\xEE\x37\x78\xF4\x41\xF8\xDE\xA3\xD9\xFA\x0A\xCC\x4E\x07\xEE\x36\xC9\x3F\x9A\x08\x61\x8A\xF4\xAD\x85\xCE\xDE\x1C\x22"};
    unsigned char eid[3] = {"\x42\x6F\x62"};
    unsigned char er[32] = {"\x00\x00\xAA\xC0\x54\x17\x79\xC8\xFC\x45\xE3\xE2\xCB\x25\xC1\x2B\x5D\x25\x76\xB2\x12\x9A\xE8\xBB\x5E\xE2\xCB\xE5\xEC\x9E\x78\x5C"};
    unsigned char plain[32];
    unsigned int plainlen;
//==============密钥交换用到的参数================
    unsigned char kks[32] = {"\x00\x02\xE6\x5B\x07\x62\xD0\x42\xF5\x1F\x0D\x23\x54\x2B\x13\xED\x8C\xFA\x2E\x9A\x0E\x72\x06\x36\x1E\x01\x3A\x28\x39\x05\xE3\x1F"};
    unsigned char alice[5] = {"\x41\x6C\x69\x63\x65"};
    unsigned char bob[3] = {"\x42\x6F\x62"};
    
    unsigned char ra[32] = {"\x00\x00\x58\x79\xDD\x1D\x51\xE1\x75\x94\x6F\x23\xB1\xB4\x1E\x93\xBA\x31\xC5\x84\xAE\x59\xA4\x26\xEC\x10\x46\xA4\xD0\x3B\x06\xC8"};
    
    unsigned char rb[32] = {"\x00\x01\x8B\x98\xC4\x4B\xEF\x9F\x85\x37\xFB\x7D\x07\x1B\x2C\x92\x8B\x3B\xC6\x5B\xD3\xD6\x9E\x1E\xEE\x21\x35\x64\x90\x56\x34\xFE"};
    
    time_t start,end;
    int res;
    
    unsigned char *gg;
    
    unsigned char AS1[32];
    unsigned char AS2[32];
    unsigned char ASK[16];
    
    unsigned char BS1[32];
    unsigned char BS2[32];
    unsigned char BSK[16];


    
//============独立验证时用到的参数==========
    unsigned char x1[32] = {"\x0f\xbd\x02\x40\x35\xb4\xca\x2f\x94\x14\x4f\xdf\x33\x3b\xf8\xe1\x81\x74\x7c\x8d\xd8\x5d\xe4\x12\xad\xc7\x18\x18\x98\x72\x43\x27"};
    unsigned char x2[32] = {"\x13\x2d\x39\x44\x38\x7b\x63\x1e\xe4\xb0\x55\x89\x9f\x6c\xfe\x42\x8f\xb9\xfc\x9f\xa2\x75\x68\x9f\xeb\x36\xbe\x2b\x60\x21\xea\xbe"};
    unsigned char y1[32] = {"\x07\x81\x20\x17\xdb\x06\x91\xd3\x43\x11\x36\x83\x21\x19\xe0\xe1\x94\x4e\x85\x5a\xed\xfc\xa7\x10\xed\xd1\x46\x6e\x69\x01\x8e\xa1"};
    unsigned char y2[32] = {"\x89\xf9\xf0\x0f\x5e\x05\x10\x6c\xb3\x4f\x60\x0f\xd9\x48\x92\x4b\x87\x24\x7b\xde\x11\xdf\x34\x01\x3d\xb6\xb9\x54\xbb\xf1\x9a\x55"};
    
    unsigned char h[32] = {"\xa1\xc3\xa6\xce\x30\x90\xea\x0d\xbd\x07\xe2\x1b\xcb\xdb\x43\x40\x4b\x80\x98\xf1\x90\x4d\xd6\xe7\x28\x3c\x7d\xfc\xdb\x23\x65\x91"};
    unsigned char xs[32] = {"\x2f\x5f\xe6\x33\xb6\x04\x16\x76\xfe\x77\x0d\x3f\xd3\x09\x6e\x0b\x64\xb0\x01\x0e\x1d\xa5\x50\x96\x5c\xd1\x94\xad\x53\x51\xa4\x98"};
    unsigned char ys[32] = {"\x51\x2b\x48\xf2\xae\xde\x4b\x10\xef\x25\xca\x58\x46\x49\x93\x8b\xfc\xd6\x3e\xe8\xb4\x8f\xbb\x86\xeb\x82\xa8\x22\x42\x97\x63\x14"};
    
    
    //如下可以使用SM9 第五部分 指定的曲线类型和主密钥初始化系统
    //也可以使用自定义曲线
    //
    SM9_Init(0,0,32,NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );

    
// ========================================================
    SM9_MSK msk = SM9_MSK_New(32, cks);  // 申明一个签名主密钥
    SM9_MSPK mspk = SM9_MSPK_New(32);   //申明一个主签名公钥

    SM9_GenMSignPubKey(&msk, &mspk);  // 生成签名主公钥
    
    gg = SM9_Set_Sign(mspk.x1, mspk.x2, mspk.y1, mspk.y2, NULL); // 启动签名lib
    
    if (gg == NULL){
        printf("init sign lib error\n");
        return 1;
    }
    SM9_PK pk = SM9_PK_New(5, id);       // 申明一个签名公钥
    SM9_SSK sk = SM9_SSK_New(32);            // 申明一个签名私钥
    
    SM9_GenSignSecKey(&sk, &pk, &msk); // 由公钥（id）生成签名私钥
    
    SM9_Sign sign = SM9_Sign_New(32);   // 申明一个签名体

//===============signature test===============
      //使用私钥sk和随机数ran，对消息mes签名
    time(&start);
 
    for(int i=0;i<TEST;i++){
        SM9_Signature(msg, 20, rand, &sk, &sign);
    }
    time(&end);
    
    printf("sign %d time is %ld sec\n",TEST, end-start);
 
    time(&start);
    for(int i=0;i<TEST;i++){// 验证函数
        res = SM9_Verify(msg, 20, &sign, &pk, NULL);
        if (res) printf("verify error at %d = %d\n", i,res);
    }
    
    time(&end);
    printf("verify %d time is %ld sec\n",TEST, end-start);
 
//===============independent verify test===============
    SM9_Free();  //注销SM9
    
    //重新启动SM9
    SM9_Init(0,0,32,NULL,NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
    
    SM9_MSPK inmspk = SM9_MSPK_New(32);
    SM9_Sign insign = SM9_Sign_New(32);
    SM9_PK inpk = SM9_PK_New(5, id);
    
    inmspk.secLevel =32;
    inmspk.x1 = x1;
    inmspk.x2 = x2;
    inmspk.y1 = y1;
    inmspk.y2 = y2;
    
    gg = SM9_Set_Sign(inmspk.x1, inmspk.x2, inmspk.y1, inmspk.y2, NULL); // 启动签名lib
    
    if (gg == NULL){
        printf("init sign lib error\n");
        return 1;
    }
    
    insign.h = h;
    insign.xs = xs;
    insign.ys = ys;
    int inres;
    inres = SM9_Verify(msg, 20, &insign, &inpk, NULL);
    if (inres) {
        printf("independent verify error = %d\n",inres);
    }else{
        printf("independent verify success! \n");
    }
    
//===============encryption test===============
    SM9_MSK esk = SM9_MSK_New(32, eks);  // 申明一个加密主密钥
    SM9_MCPK mcpk = SM9_MCPK_New(32);   //申明一个主加密公钥
    SM9_GenMEncryptPubKey(&esk, &mcpk); // 生成加密主公钥
    gg = SM9_Set_Encrypt(mcpk.x, mcpk.y,NULL);  // 启动加密lib
    if (gg == NULL){
        printf("init encryption lib error\n");
        return 1;
    }
    
    SM9_PK epk = SM9_PK_New(3, eid);   //申明一个加密公钥
    SM9_CSK csk = SM9_CSK_New(32);   //申明一个加密私钥
    SM9_GenEncryptSecKey(&csk, &epk, &esk);  // 由公钥（id）生成加密私钥
    
    SM9_Cipher cipher = SM9_Cipher_New(32);

    time(&start);
    for(int i=0;i<TEST;i++){
        SM9_Encrypt(cmsg, 20, KDF_SM4, er, &epk, &cipher);
    }
    time(&end);
    printf("Encrypt %d time is %ld sec\n",TEST, end-start);
    
    time(&start);
    for(int i=0;i<TEST;i++){
        res = SM9_Decrypt(&epk, KDF_SM4, &csk, &cipher, plain, &plainlen);
        if (res) printf("decrypt error code is %d \n", res);
    }
    time(&end);
    printf("Decrypt %d time is %ld sec\n",TEST, end-start);

    
//===============keyexchange test===============
    SM9_MSK ksk = SM9_MSK_New(32, kks);
    
    SM9_MKPK mkpk = SM9_MKPK_New(32);   //申明一个主协商公钥
    SM9_GenMKeyExchangePubKey(&ksk, &mkpk); // 生成协商主公钥
    
    gg = SM9_Set_KeyExchange(mkpk.x, mkpk.y,NULL);  // 启动协商lib
    
    if (gg == NULL){
        printf("init keyexchange lib error\n");
        return 1;
    }
    
    SM9_PK  apk = SM9_PK_New(5, alice);
    SM9_KSK ask = SM9_KSK_New(32);
    SM9_GenKeyExchangeSecKey(&ask, &apk, &ksk);
    
    SM9_PK  bpk = SM9_PK_New(3, bob);
    SM9_KSK bsk = SM9_KSK_New(32);
    SM9_GenKeyExchangeSecKey(&bsk, &bpk, &ksk);
    
    SM9_Send ase = SM9_Send_New(32);
    SM9_Send bse = SM9_Send_New(32);
    
    SM9_SendStep(ra, &bpk, &ase);
    SM9_SendStep(rb, &apk, &bse);

  
    
    time(&start);
    for(int i=0;i<TEST;i++){
        SM9_ReceiveStep(ra, &ase, &bse, &apk, &bpk, &ask, 16, AS1, AS2, ASK, AKE_SENDER);
        SM9_ReceiveStep(rb, &bse, &ase, &bpk, &apk, &bsk, 16, BS1, BS2, BSK, AKE_RECEIVER);
        if(SM9_CheckStep(AS1, BS2)) printf("error at step 1\n");
        if(SM9_CheckStep(AS2, BS1)) printf("error at step 2\n");
    }
    time(&end);
    printf("key exchange %d time is %ld sec\n", TEST*2,end-start);
    printf("end test \n ================== \n");
    return 0;
}
