//
//  sm9_encrypt.c
//  SM9_FREE
//
//  Created by 宋赓 on 2017/3/25.
//  Copyright © 2017年 宋赓. All rights reserved.
//
#include <stdio.h>
#include "sm9_algorithm.h"
#include "sm9_utils.h"
#include "print_out.h"
#include "sm4.h"
#include "sm3.h"
#define block  16
#define macklen 32



BOOL SM9_GenEncryptSecKey(SM9_CSK *sk, SM9_PK *pk,SM9_MSK *msk){
    miracl *mr_mip;
    unsigned char *id;
    big ssk,k;
    ecn2 psk;
    if (!sm9encrypt){
    //    printf("the sm9 encrypt lib is not init, please run SM9_SET_ENCRYPT function\n");
        return sm9encrypt;
    }
    mr_mip = GenMiracl(sk->secLevel);
    id = (unsigned char *)malloc(sizeof(unsigned char)*(pk->keylen+1));
    memcpy(id, pk->pk, pk->keylen);
    memcpy(id+pk->keylen, hid+2, 1);
    
    
    k = mirvar(_MIPP_ 0);
    
    
    
    ecn2_mirvar(_MIPP_ &psk);
    
    
    ssk = Hfun(_MIPP_ (char *)id,pk->keylen+1,sk->secLevel,1);
    
    bytes_to_big(_MIPP_ msk->keylen, (char *)msk->msk, k);
    
    add(_MIPP_ ssk, k, ssk);
    divide(_MIPP_ ssk, sm9n, sm9n);
    
    xgcd(_MIPP_ ssk, sm9n, ssk, ssk, ssk);
    multiply(_MIPP_ ssk,k,ssk);
    divide(_MIPP_ ssk, sm9n, sm9n);
    
    ecn2_copy(&p2G2, &psk);
    ecn2_mul(_MIPP_ ssk, &psk);
    ecn2_norm(_MIPP_ &psk);
    
    redc(_MIPP_ psk.x.a, k);
    big_to_bytes(_MIPP_ sk->secLevel, k,(char *)sk->x2, TRUE);
    redc(_MIPP_ psk.x.b, k);
    big_to_bytes(_MIPP_ sk->secLevel, k,(char *)sk->x1, TRUE);
    redc(_MIPP_ psk.y.a, k);
    big_to_bytes(_MIPP_ sk->secLevel, k,(char *)sk->y2, TRUE);
    redc(_MIPP_ psk.y.b, k);
    big_to_bytes(_MIPP_ sk->secLevel, k,(char *)sk->y1, TRUE);
    
    ecn2_kill(_MIPP_ &psk);
    mirkill(k);
    mirkill(ssk);
    free(id);
    CloseMiracl(_MIPPO_);
    return TRUE;
}


int SM9_Encrypt(unsigned char *mes, unsigned int meslen, unsigned int KDF_ID, unsigned char *ran, SM9_PK *pk, SM9_Cipher *cip){
    miracl *mr_mip;
	int re;
    
    big h,y,r;
    
    ecn *QB;
    
    zzn12 w;
	unsigned int rk[keyIntLen];
    unsigned char ecb[block];
    
    unsigned char *id;
	unsigned char *mw;
	
	int klen;
    int i,j;
	int mwlen;
	unsigned char *kout;
	if (!sm9encrypt){
    //    printf("the sm9 encrypt lib is not init, please run SM9_SET_ENCRYPT function\n");
        return LIB_NOT_INIT;
    }
    mr_mip = GenMiracl(sm9len);
    
    y  = mirvar(_MIPP_ 0);
    r  = mirvar(_MIPP_ 0);
    QB = epoint_init(_MIPPO_);
    zzn12_mirvar(_MIPP_ &w);
    
    
    
    id = (unsigned char *)malloc(sizeof(unsigned char)*(pk->keylen+1));
    memcpy(id, pk->pk, pk->keylen);
    memcpy(id+pk->keylen, hid+2, 1);
    
    h = Hfun(_MIPP_ (char *)id, pk->keylen+1, cip->secLevel, 1);
    
    
    ecurve_mult(_MIPP_ h, &p1G1, QB);
    ecurve_add(_MIPP_ &ppG1, QB);


    bytes_to_big(_MIPP_ cip->secLevel, (char *)ran, r);
    
    ecurve_mult(_MIPP_ r, QB, QB);
    epoint_get(_MIPP_ QB, h, y);
        
    big_to_bytes(_MIPP_ cip->secLevel, h, (char*)cip->x, TRUE);
    big_to_bytes(_MIPP_ cip->secLevel, y, (char*)cip->y, TRUE);
    
    zzn12_copy(&eGt, &w);
    zzn12_pow(_MIPP_ &w, r);
    
    
    mwlen = pk->keylen+cip->secLevel*(12+2);
    mw = (unsigned char *)malloc(sizeof(unsigned char)*(mwlen));
    memcpy(mw, cip->x, cip->secLevel);
    memcpy(mw+cip->secLevel, cip->y, cip->secLevel);
    zzn12_tochar(_MIPP_ &w, mw+cip->secLevel*2,sm9len);
    memcpy(mw+cip->secLevel*(12+2), pk->pk, pk->keylen);
    
    
    
    

    
    if (KDF_ID){
        klen = block;
        cip->cplen = block * (unsigned int)(meslen/block);
        if (cip->cplen < meslen) cip->cplen += block;
        cip->cp = (unsigned char *)malloc(sizeof(unsigned char)*(cip->cplen));
    }else{
        klen = meslen;
        cip->cp = (unsigned char *)malloc(sizeof(unsigned char)*(meslen));
    }
    
    kout = (unsigned char *)malloc(sizeof(unsigned char)*(klen+macklen));
    
    kdf((char *)mw, mwlen, (klen+macklen)*8, (char *)kout);
    
    j=0;
    if (KDF_ID){
        // sm4
        re = meslen;
        SM4KeyExt(kout, rk, ENCRYPT);
        while (re>0){
            if ( re < block){
                memcpy(ecb, mes+j, re);
                for(i=re;i<block;i++){
                    ecb[i] = 0x0C;
                }
            }else{
                memcpy(ecb, mes+j, block);
            }
            
            SM4ECBEnc(ecb, cip->cp+j, rk);
            j += block;
            re -= block;
        }
    }else{
        //xor
        cip->cplen = meslen;
        xorAlgor(kout,meslen,mes,cip->cp);
    }
    
    
    cip->entype = KDF_ID;
    
    MAC(kout+klen, macklen, cip->cp, cip->cplen, cip->c3);
    
    mirkill(h);
    mirkill(y);
    mirkill(r);
    epoint_free(QB);
    zzn12_kill(_MIPP_ &w);
    free(mw);
    free(id);
    free(kout);
    CloseMiracl(_MIPPO_);
    return 0;
}

int SM9_Decrypt(SM9_PK *pk, unsigned int KDF_ID, SM9_CSK *sk, SM9_Cipher *cip,unsigned char *mes,unsigned int *meslen){
    miracl *mr_mip;
    int re,klen,j;
    unsigned int rk[keyIntLen];
    unsigned char ecb[block];
    unsigned char hmac[SM3_HASH_LEN];
    big x,y;
    epoint *C;
    ecn2 csk;
    zzn12 w;
    
    zzn2 xx;
    zzn2 yy;
	
	unsigned char *mw;
	int mwlen;
	unsigned char *kout;
	
    
	
	if (!sm9encrypt){
    //    printf("the sm9 encrypt lib is not init, please run SM9_SET_ENCRYPT function\n");
        return LIB_NOT_INIT;
    }
    mr_mip = GenMiracl(sm9len);
    x = mirvar(_MIPP_ 0);
    y = mirvar(_MIPP_ 0);
    ecn2_mirvar(_MIPP_ &csk);
    zzn12_mirvar(_MIPP_ &w);
    zzn2_mirvar(_MIPP_ &xx);
    zzn2_mirvar(_MIPP_ &yy);
    C = epoint_init(_MIPPO_);
    
    bytes_to_big(_MIPP_ cip->secLevel, (char *)cip->x, x);
    bytes_to_big(_MIPP_ cip->secLevel, (char *)cip->y, y);
    
    if(!epoint_set(_MIPP_ x, y, 1, C)){
        mirkill(x);
        mirkill(y);
        epoint_free(C);
        ecn2_kill(_MIPP_ &csk);
        zzn2_kill(_MIPP_ &xx);
        zzn2_kill(_MIPP_ &yy);
        zzn12_kill(_MIPP_ &w);
        
        CloseMiracl(_MIPPO_);
        return NOT_ON_G1;
    }
    
    bytes_to_big(_MIPP_ sk->secLevel, (char *)sk->x2, x);
    bytes_to_big(_MIPP_ sk->secLevel, (char *)sk->x1, y);
    zzn2_from_bigs(_MIPP_ x, y, &xx);
    
    bytes_to_big(_MIPP_ sk->secLevel, (char *)sk->y2, x);
    bytes_to_big(_MIPP_ sk->secLevel, (char *)sk->y1, y);
    zzn2_from_bigs(_MIPP_ x, y, &yy);
    
    
    if(!ecn2_set(_MIPP_ &xx, &yy, &csk)){
        mirkill(x);
        mirkill(y);
        epoint_free(C);
        ecn2_kill(_MIPP_ &csk);
        zzn2_kill(_MIPP_ &xx);
        zzn2_kill(_MIPP_ &yy);
        zzn12_kill(_MIPP_ &w);
        
        CloseMiracl(_MIPPO_);
        return NOT_ON_G2;
    }
    
    if(!ecap(_MIPP_ &csk, C, sm9t, &sm9X, &w)){
        mirkill(x);
        mirkill(y);
        epoint_free(C);
        ecn2_kill(_MIPP_ &csk);
        zzn2_kill(_MIPP_ &xx);
        zzn2_kill(_MIPP_ &yy);
        zzn12_kill(_MIPP_ &w);
        
        CloseMiracl(_MIPPO_);
        return PAIRING_ERROR;
    }
    
    
    mwlen = pk->keylen+sk->secLevel*(12+2);
    mw = (unsigned char *)malloc(sizeof(unsigned char)*(mwlen));
    memcpy(mw, cip->x, cip->secLevel);
    memcpy(mw+cip->secLevel, cip->y, cip->secLevel);
    zzn12_tochar(_MIPP_ &w, mw+cip->secLevel*2,sm9len);
    memcpy(mw+cip->secLevel*(12+2), pk->pk, pk->keylen);
    
    if (KDF_ID){
        klen = block;
    }else{
        klen = cip->cplen;
    }
    
    kout = (unsigned char *)malloc(sizeof(unsigned char)*(klen+macklen));
    kdf((char *)mw, mwlen, (klen+macklen)*8, (char *)kout);
    
    j = 0;
    if (KDF_ID){
        if ((cip->cplen % block) != 0){
            mirkill(x);
            mirkill(y);
            epoint_free(C);
            ecn2_kill(_MIPP_ &csk);
            zzn2_kill(_MIPP_ &xx);
            zzn2_kill(_MIPP_ &yy);
            zzn12_kill(_MIPP_ &w);
            
            free(mw);
            free(kout);
            CloseMiracl(_MIPPO_);
            return BLOCK_ERROR;
        }
        re = cip->cplen;
        SM4KeyExt(kout, rk, DECRYPT);
        while (re>0){
            memcpy(ecb, cip->cp+j, block);
            SM4ECBDec(ecb, mes+j, rk);
            j += block;
            re -= block;
        }
    }else{
        xorAlgor(kout,cip->cplen,cip->cp,mes);
    }
    meslen[0] = cip->cplen;
    
    MAC(kout+klen, macklen, cip->cp, cip->cplen, hmac);
    
    re = 0;
    for (j=0;j<SM3_HASH_LEN;j++){
        if (hmac[j] != cip->c3[j]) re = DECRYPT_ERROR;
    }
    
    mirkill(x);
    mirkill(y);
    epoint_free(C);
    ecn2_kill(_MIPP_ &csk);
    zzn2_kill(_MIPP_ &xx);
    zzn2_kill(_MIPP_ &yy);
    zzn12_kill(_MIPP_ &w);
    
    free(mw);
    free(kout);
    CloseMiracl(_MIPPO_);
    return re;
}

