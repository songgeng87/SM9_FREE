//
//  sm9_signature.c
//  SM9_FREE
//
//  Created by 宋赓 on 2017/3/25.
//  Copyright © 2017年 宋赓. All rights reserved.
//
#include <stdio.h>
#include "sm9_algorithm.h"
#include "sm9_utils.h"



BOOL SM9_GenSignSecKey(SM9_SSK *sk, SM9_PK *pk,SM9_MSK *msk){
    miracl *mr_mip;
    unsigned char *id;
    big ssk,k;
    ecn *ppk;
    if (!sm9sign){
    //    printf("the sm9 sign lib is not init, please run SM9_SET_SIGN function\n");
        return sm9sign;
    }
    
    
    mr_mip = GenMiracl(sk->secLevel);
    id = (unsigned char *)malloc(sizeof(unsigned char)*(pk->keylen+1));
    memcpy(id, pk->pk, pk->keylen);
    memcpy(id+pk->keylen, hid, 1);
    
    k = mirvar(_MIPP_ 0);
    ppk = epoint_init(_MIPPO_);
    
    
    
    ssk = Hfun(_MIPP_ (char *)id,pk->keylen+1,sk->secLevel,1);
    
    bytes_to_big(_MIPP_ msk->keylen, (char *)msk->msk, k);
    
    add(_MIPP_ ssk, k, ssk);
    divide(_MIPP_ ssk, sm9n, sm9n);
    
    xgcd(_MIPP_ ssk, sm9n, ssk, ssk, ssk);
    multiply(_MIPP_ ssk,k,ssk);
    divide(_MIPP_ ssk, sm9n, sm9n);
    
    ecurve_mult(_MIPP_ ssk, &p1G1, ppk);
    epoint_norm(_MIPP_ ppk);
    
    epoint_get(_MIPP_ ppk, ssk, k);
    big_to_bytes(_MIPP_ sk->secLevel, ssk, (char *)sk->x, TRUE);
    big_to_bytes(_MIPP_ sk->secLevel, k, (char *)sk->y, TRUE);
    
    mirkill(ssk);
    mirkill(k);
    epoint_free(ppk);
    free(id);
    CloseMiracl(_MIPPO_);
    return TRUE;
}

int SM9_Signature(unsigned char* mes,unsigned int meslen,unsigned char* ran,SM9_SSK *sk, SM9_Sign *sign){
    miracl *mr_mip;
	big x,y,r,zero,h;
	zzn12 w;
	ecn *pa;
	int mwlen;
    unsigned char *mw;
	if (!sm9sign){
    //    printf("the sm9 sign lib is not init, please run SM9_SET_SIGN function\n");
        return LIB_NOT_INIT;
    }
    mr_mip = GenMiracl(sm9len);
    x = mirvar(_MIPP_ 0);
    y = mirvar(_MIPP_ 0);
    r = mirvar(_MIPP_ 0);
	h = mirvar(_MIPP_ 0);
	zero = mirvar(_MIPP_ 0);
    
    zzn12_mirvar(_MIPP_ &w);
    
    
    pa = epoint_init(_MIPPO_);
    
    bytes_to_big(_MIPP_ sk->secLevel, (char *)sk->x, x);
    bytes_to_big(_MIPP_ sk->secLevel, (char *)sk->y, y);
    
    if (!epoint_set(_MIPP_ x, y, 1, pa)){
        zzn12_kill(_MIPP_ &w);
        mirkill(r);
        mirkill(x);
        mirkill(y);
        mirkill(h);
        epoint_free(pa);
        CloseMiracl(_MIPPO_);
        return  NOT_ON_G1;
    }
    
    zzn12_copy(&gGt, &w);
    bytes_to_big(_MIPP_ sk->secLevel, (char *)ran, r);
    zzn12_pow(_MIPP_ &w, r);
    
    mwlen = meslen+sk->secLevel*12;
    mw = (unsigned char *)malloc(sizeof(unsigned char)*(mwlen));
    
    memcpy(mw, mes, meslen);
    
    zzn12_tochar(_MIPP_ &w, mw+meslen,sm9len);
    
    h = Hfun(_MIPP_ (char *)mw, mwlen, sk->secLevel, 2);
    
    subtract(_MIPP_ r, h, r);
    divide(_MIPP_ r, sm9n, sm9n);
    
    
    
    if (mr_compare(zero, r)==0){
        zzn12_kill(_MIPP_ &w);
        mirkill(r);
        mirkill(x);
        mirkill(y);
        mirkill(h);
        mirkill(zero);
        free(mw);
        epoint_free(pa);
        CloseMiracl(_MIPPO_);
        return  SIGN_ZERO_ERROR;
    }
    
    ecurve_mult(_MIPP_ r, pa, pa);
    epoint_norm(_MIPP_ pa);
    
    epoint_get(_MIPP_ pa, x, y);
    
    big_to_bytes(_MIPP_ sk->secLevel, x, (char *)sign->xs, TRUE);
    big_to_bytes(_MIPP_ sk->secLevel, y, (char *)sign->ys, TRUE);
    big_to_bytes(_MIPP_ sk->secLevel, h, (char *)sign->h, TRUE);
    
    zzn12_kill(_MIPP_ &w);
    mirkill(r);
    mirkill(x);
    mirkill(y);
    mirkill(h);
    mirkill(zero);
    free(mw);
    epoint_free(pa);
    CloseMiracl(_MIPPO_);
    return 0;
}

int SM9_Verify(unsigned char *mes,unsigned int meslen, SM9_Sign *sign, SM9_PK *pk, SM9_MSPK *mpk){
    miracl *mr_mip;
	
	
    
    int re;
    
    big h,x,y,h1,h2;
    ecn *S;
    ecn2 pp;
    ecn2 P;
    zzn12 t;
    zzn12 g;
    
    unsigned char *id;

	 int mwlen;
    unsigned char *mw;
    
    if (!sm9sign){
    //    printf("the sm9 sign lib is not init, please run SM9_SET_SIGN function\n");
        return LIB_NOT_INIT;
    }
	mr_mip = GenMiracl(sm9len);
    h = mirvar(_MIPP_ 0);
    x = mirvar(_MIPP_ 0);
    y = mirvar(_MIPP_ 0);
    S = epoint_init(_MIPPO_);
    ecn2_mirvar(_MIPP_ &pp);
    ecn2_mirvar(_MIPP_ &P);
    zzn12_mirvar(_MIPP_ &t);
    zzn12_mirvar(_MIPP_ &g);
    
    bytes_to_big(_MIPP_ sign->secLevel, (char *)sign->h, h);
    
    if((mr_compare(h, sm9n) >= 0) || (mr_compare(h, x)) < 0){
        mirkill(h);
        mirkill(x);
        mirkill(y);
        epoint_free(S);
        zzn12_kill(_MIPP_ &t);
        zzn12_kill(_MIPP_ &g);
        ecn2_kill(_MIPP_ &pp);
        ecn2_kill(_MIPP_ &P);
        return VERIFY_ERROR_1;
    }
    
    bytes_to_big(_MIPP_ sign->secLevel, (char *)sign->xs, x);
    bytes_to_big(_MIPP_ sign->secLevel, (char *)sign->ys, y);
    
    if(!(epoint_set(_MIPP_ x, y, 1, S))){
        mirkill(h);
        mirkill(x);
        mirkill(y);
        epoint_free(S);
        zzn12_kill(_MIPP_ &t);
        zzn12_kill(_MIPP_ &g);
        ecn2_kill(_MIPP_ &pp);
        ecn2_kill(_MIPP_ &P);
        return NOT_ON_G1;
    }
    
    if (mpk == NULL){
        zzn12_copy(&gGt, &g);
        ecn2_copy(&ppG2, &pp);
    }else{
        //todo
        zzn12_copy(&gGt, &g);
        ecn2_copy(&ppG2, &pp);
    }
    ecn2_copy(&p2G2, &P);
    
    zzn12_pow(_MIPP_ &g, h);
    
    id = (unsigned char *)malloc(sizeof(unsigned char)*(pk->keylen+1));
    memcpy(id, pk->pk, pk->keylen);
    memcpy(id+pk->keylen, hid, 1);
    
    
    h1 = Hfun(_MIPP_ (char *)id, pk->keylen+1, sign->secLevel, 1);
    
    
    
    ecn2_mul(_MIPP_ h1, &P);
    
    ecn2_add(_MIPP_ &pp, &P);
    
    //   ecn2_norm(_MIPP_ &P);
    
    if(!ecap(_MIPP_ &P, S, sm9t, &sm9X, &t)){
        mirkill(h);
        mirkill(x);
        mirkill(y);
        epoint_free(S);
        zzn12_kill(_MIPP_ &t);
        zzn12_kill(_MIPP_ &g);
        ecn2_kill(_MIPP_ &pp);
        ecn2_kill(_MIPP_ &P);
        free(id);
        return VERIFY_ERROR_2;
    }
    
    zzn12_mul(_MIPP_ &t, &g, &t);
    
    
    
   
    mwlen = meslen+sign->secLevel*12;
    mw = (unsigned char *)malloc(sizeof(unsigned char)*(mwlen));
    memcpy(mw, mes, meslen);
    zzn12_tochar(_MIPP_ &t, mw+meslen,sm9len);
    
    
    h2 = Hfun(_MIPP_ (char *)mw, mwlen, sign->secLevel, 2);
    
    re = mr_compare(h2, h);
    if (re!=0) re=VERIFY_ERROR_2;
    mirkill(h);
    mirkill(h1);
    mirkill(h2);
    mirkill(x);
    mirkill(y);
    epoint_free(S);
    zzn12_kill(_MIPP_ &t);
    zzn12_kill(_MIPP_ &g);
    ecn2_kill(_MIPP_ &pp);
    ecn2_kill(_MIPP_ &P);
    free(id);
    free(mw);
    CloseMiracl(_MIPPO_);
    return re;
}
