//
//  sm9_keyexchange.c
//  SM9_FREE
//
//  Created by 宋赓 on 2017/3/26.
//  Copyright © 2017年 宋赓. All rights reserved.
//
#include <stdio.h>
#include "sm9_algorithm.h"
#include "sm9_utils.h"
#include "print_out.h"
#include "sm3.h"

BOOL SM9_GenKeyExchangeSecKey(SM9_KSK *sk, SM9_PK *pk,SM9_MSK *msk){
    miracl *mr_mip;
    unsigned char *id;
	ecn2 psk;
	big ssk,k;
	if (!sm9keyexchange){
    //    printf("the sm9 keyexchange lib is not init, please run SM9_SET_KYEEXCHANGE function\n");
        return LIB_NOT_INIT;
    }
    mr_mip = GenMiracl(sk->secLevel);
    id = (unsigned char *)malloc(sizeof(unsigned char)*(pk->keylen+1));
    memcpy(id, pk->pk, pk->keylen);
    memcpy(id+pk->keylen, hid+1, 1);
    
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
    return 0;
}

int SM9_SendStep(unsigned char *ran, SM9_PK *pk, SM9_Send *se){
    miracl *mr_mip;
	big h,r;
	ecn *QB;
	unsigned char *id;
	if (!sm9keyexchange){
    //    printf("the sm9 keyexchange lib is not init, please run SM9_SET_ENCRYPT function\n");
        return LIB_NOT_INIT;
    }
    mr_mip = GenMiracl(sm9len);
    
	r = mirvar(_MIPP_ 0);
    
    
    QB = epoint_init(_MIPPO_);
    
    
    id = (unsigned char *)malloc(sizeof(unsigned char)*(pk->keylen+1));
    memcpy(id, pk->pk, pk->keylen);
    memcpy(id+pk->keylen, hid+1, 1);
    
    h = Hfun(_MIPP_ (char *)id, pk->keylen+1, sm9len, 1);
    
    bytes_to_big(_MIPP_ sm9len, (char *)ran, r);
    
    ecurve_mult(_MIPP_ h, &p1G1, QB);
    ecurve_add(_MIPP_ &keG1, QB);
    ecurve_mult(_MIPP_ r, QB, QB);
    
    epoint_get(_MIPP_ QB, h, r);
    
    big_to_bytes(_MIPP_ sm9len, h, (char *)se->x, TRUE);
    big_to_bytes(_MIPP_ sm9len, r, (char *)se->y, TRUE);
    
    mirkill(h);
    mirkill(r);
    epoint_free(QB);
    free(id);
    
    CloseMiracl(_MIPPO_);
    return 0;
}

int SM9_ReceiveStep(unsigned char *ran,SM9_Send *S, SM9_Send *R,SM9_PK *SP, SM9_PK *RP, SM9_KSK *ksk, unsigned int sklen,unsigned char *S1, unsigned char *S2, unsigned char *SK,unsigned int SorR){
    unsigned char ftmp[SM3_HASH_LEN];

    unsigned char HS[2] = {"\x82\x83"};
    zzn2 xx,yy;
    zzn12 g1,g2,g3;
    ecn *RR;
    ecn2 dsa;
    big x,y;
	miracl *mr_mip;
	
	SM9_Send *A;
    SM9_Send *B;
    SM9_PK   *AP;
    SM9_PK   *BP;
    
	SM3_CTX sm3;

	unsigned char *mw;
    int mwlen = sm9len*(12);
	int mwtmp = 0;
	if (!sm9keyexchange){
    //    printf("the sm9 encrypt lib is not init, please run SM9_SET_ENCRYPT function\n");
        return LIB_NOT_INIT;
    }
    mr_mip = GenMiracl(sm9len);
    
    
    
    x = mirvar(_MIPP_ 0);
    y = mirvar(_MIPP_ 0);
    
    zzn2_mirvar(_MIPP_ &xx);
    zzn2_mirvar(_MIPP_ &yy);
    zzn12_mirvar(_MIPP_ &g1);
    zzn12_mirvar(_MIPP_ &g2);
    zzn12_mirvar(_MIPP_ &g3);
    
    RR = epoint_init(_MIPPO_);
    ecn2_mirvar(_MIPP_ &dsa);
    
    
    
    A  = S;
    B  = R;
    AP = SP;
    BP = RP;
    
    bytes_to_big(_MIPP_ sm9len, (char *) B->x, x);
    bytes_to_big(_MIPP_ sm9len, (char *) B->y, y);
    
    if (!epoint_set(_MIPP_ x, y, 1, RR)){
        mirkill(x);
        mirkill(y);
        
        zzn2_kill(_MIPP_ &xx);
        zzn2_kill(_MIPP_ &yy);
        zzn12_kill(_MIPP_ &g1);
        zzn12_kill(_MIPP_ &g2);
        zzn12_kill(_MIPP_ &g3);
        
        epoint_free(RR);
        ecn2_kill(_MIPP_ &dsa);
        
        CloseMiracl(_MIPPO_);
        return NOT_ON_G1;
    }
    
    bytes_to_big(_MIPP_ sm9len, (char *) ksk->x2, x);
    bytes_to_big(_MIPP_ sm9len, (char *) ksk->x1, y);
    zzn2_from_bigs(_MIPP_ x, y, &xx);
    
    bytes_to_big(_MIPP_ sm9len, (char *) ksk->y2, x);
    bytes_to_big(_MIPP_ sm9len, (char *) ksk->y1, y);
    zzn2_from_bigs(_MIPP_ x, y, &yy);
    
    if (!ecn2_set(_MIPP_ &xx, &yy, &dsa)){
        mirkill(x);
        mirkill(y);
        
        zzn2_kill(_MIPP_ &xx);
        zzn2_kill(_MIPP_ &yy);
        zzn12_kill(_MIPP_ &g1);
        zzn12_kill(_MIPP_ &g2);
        zzn12_kill(_MIPP_ &g3);
        
        epoint_free(RR);
        ecn2_kill(_MIPP_ &dsa);
        
        CloseMiracl(_MIPPO_);
        return NOT_ON_G2;
    }
    
    if(!SorR){
        bytes_to_big(_MIPP_ sm9len, (char *)ran, x);
        zzn12_copy(&kGt, &g1);
        zzn12_pow(_MIPP_ &g1, x);
        
        if (!ecap(_MIPP_ &dsa, RR, sm9t, &sm9X, &g2)){
            mirkill(x);
            mirkill(y);
            
            zzn2_kill(_MIPP_ &xx);
            zzn2_kill(_MIPP_ &yy);
            zzn12_kill(_MIPP_ &g1);
            zzn12_kill(_MIPP_ &g2);
            zzn12_kill(_MIPP_ &g3);
            
            epoint_free(RR);
            ecn2_kill(_MIPP_ &dsa);
            
            CloseMiracl(_MIPPO_);
            return PAIRING_ERROR;
        }
        
        bytes_to_big(_MIPP_ sm9len, (char *)ran, x);
        zzn12_copy(&g2, &g3);
        zzn12_pow(_MIPP_ &g3, x);
    }else{
        bytes_to_big(_MIPP_ sm9len, (char *)ran, x);
        zzn12_copy(&kGt, &g2);
        zzn12_pow(_MIPP_ &g2, x);
        
        if (!ecap(_MIPP_ &dsa, RR, sm9t, &sm9X, &g1)){
            mirkill(x);
            mirkill(y);
            
            zzn2_kill(_MIPP_ &xx);
            zzn2_kill(_MIPP_ &yy);
            zzn12_kill(_MIPP_ &g1);
            zzn12_kill(_MIPP_ &g2);
            zzn12_kill(_MIPP_ &g3);
            
            epoint_free(RR);
            ecn2_kill(_MIPP_ &dsa);
            
            CloseMiracl(_MIPPO_);
            return PAIRING_ERROR;
        }
        
        bytes_to_big(_MIPP_ sm9len, (char *)ran, x);
        zzn12_copy(&g1, &g3);
        zzn12_pow(_MIPP_ &g3, x);
    }
    
    
    
    if(!SorR){
        A  = S;
        B  = R;
        AP = SP;
        BP = RP;
    }else{
        A  = R;
        B  = S;
        AP = RP;
        BP = SP;
    }
    
    
    mw = (unsigned char *)malloc(sizeof(unsigned char)*(mwlen));
    
    
    SM3Init(&sm3);
    
    zzn12_tochar(_MIPP_ &g2, mw,sm9len);
    SM3Update(&sm3, mw, mwlen);
    
    zzn12_tochar(_MIPP_ &g3, mw,sm9len);
    SM3Update(&sm3, mw, mwlen);
    
    SM3Update(&sm3, AP->pk,AP->keylen);
    SM3Update(&sm3, BP->pk,BP->keylen);
    
    SM3Update(&sm3, A->x,A->secLevel);
    SM3Update(&sm3, A->y,A->secLevel);
    SM3Update(&sm3, B->x,A->secLevel);
    SM3Update(&sm3, B->y,A->secLevel);
    SM3Final(ftmp, &sm3);
    
    SM3Init(&sm3);
    SM3Update(&sm3, HS, 1);
    zzn12_tochar(_MIPP_ &g1, mw,sm9len);
    SM3Update(&sm3, mw, mwlen);
    SM3Update(&sm3, ftmp, SM3_HASH_LEN);
    if (!SorR){
        SM3Final(S1, &sm3);
    }else{
        SM3Final(S2, &sm3);
    }
    
    SM3Init(&sm3);
    SM3Update(&sm3, HS+1, 1);
    zzn12_tochar(_MIPP_ &g1, mw,sm9len);
    SM3Update(&sm3, mw, mwlen);
    SM3Update(&sm3, ftmp, SM3_HASH_LEN);
    if (!SorR){
        SM3Final(S2, &sm3);
    }else{
        SM3Final(S1, &sm3);
    }
    
    free(mw);
    mwlen = AP->keylen + BP->keylen + sm9len*(4+12*3);
    mw = (unsigned char *)malloc(sizeof(unsigned char)*(mwlen));
    
    
    
    memcpy(mw + mwtmp, AP->pk, AP->keylen);
    mwtmp += AP->keylen;
    
    memcpy(mw + mwtmp, BP->pk, BP->keylen);
    mwtmp += BP->keylen;
    
    memcpy(mw + mwtmp, A->x, A->secLevel);
    mwtmp += A->secLevel;
    
    memcpy(mw + mwtmp, A->y, A->secLevel);
    mwtmp += A->secLevel;
    
    memcpy(mw + mwtmp, B->x, B->secLevel);
    mwtmp += B->secLevel;
    
    memcpy(mw + mwtmp, B->y, B->secLevel);
    mwtmp += B->secLevel;
    
    zzn12_tochar(_MIPP_ &g1, mw + mwtmp,sm9len);
    mwtmp += sm9len*12;
    
    zzn12_tochar(_MIPP_ &g2, mw + mwtmp,sm9len);
    mwtmp += sm9len*12;
    
    zzn12_tochar(_MIPP_ &g3, mw + mwtmp,sm9len);
    mwtmp += sm9len*12;
    
    kdf((char *)mw, mwlen, sklen*8, (char *)SK);
    
    mirkill(x);
    mirkill(y);
    
    zzn2_kill(_MIPP_ &xx);
    zzn2_kill(_MIPP_ &yy);
    zzn12_kill(_MIPP_ &g1);
    zzn12_kill(_MIPP_ &g2);
    zzn12_kill(_MIPP_ &g3);
    
    epoint_free(RR);
    ecn2_kill(_MIPP_ &dsa);
    free(mw);
    CloseMiracl(_MIPPO_);
    return 0;
}

int SM9_CheckStep(unsigned char *R, unsigned char *G){
    int i=0;
    for (i=0;i<SM3_HASH_LEN;i++){
        if (R[i]!=G[i]){
            return 1;
        }
    }
    return  0;
}
