//
//  sm9_setup.c
//  SM9_FREE
//
//  Created by 宋赓 on 2017/3/12.
//  Copyright © 2017年 宋赓. All rights reserved.
//
#include "sm9_algorithm.h"
#include "sm9_utils.h"
#include "print_out.h"
#include "sm3.h"

BOOL sm9init = FALSE;
BOOL sm9sign = FALSE;
BOOL sm9encrypt = FALSE;
BOOL sm9keyexchange = FALSE;

int   sm9len = 32;
// curve parameter
big   sm9q;
big   sm9a;
big   sm9b;
big   sm9n;

// precomputed parameters
big   sm9t;
zzn2  sm9X;

ecn   p1G1;
ecn   ppG1;
ecn   keG1;

ecn2  p2G2;
ecn2  ppG2;

zzn12 gGt;
zzn12 eGt;
zzn12 kGt;

BOOL  TWIST;

// hid of algorithm
//01 signature       algorithm
//02 keyexchange     algorithm
//03 encrypt         algorithm
const unsigned char hid[3] = {0x01,0x02,0x03};


//=======================

//parameters of standard sm9

BOOL twist = SM_SEXTIC_M;

char ct[32] = {"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x60\x00\x00\x00\x00\x58\xF9\x8A"};

char cq[32] = {"\xB6\x40\x00\x00\x02\xA3\xA6\xF1\xD6\x03\xAB\x4F\xF5\x8E\xC7\x45\x21\xF2\x93\x4B\x1A\x7A\xEE\xDB\xE5\x6F\x9B\x27\xE3\x51\x45\x7D"};

char ca[32] = {"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"};

char cb[32] = {"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x05"};

char cn[32] = {"\xB6\x40\x00\x00\x02\xA3\xA6\xF1\xD6\x03\xAB\x4F\xF5\x8E\xC7\x44\x49\xF2\x93\x4B\x18\xEA\x8B\xEE\xE5\x6E\xE1\x9C\xD6\x9E\xCF\x25"};

char cxp1[32] = {"\x93\xDE\x05\x1D\x62\xBF\x71\x8F\xF5\xED\x07\x04\x48\x7D\x01\xD6\xE1\xE4\x08\x69\x09\xDC\x32\x80\xE8\xC4\xE4\x81\x7C\x66\xDD\xDD"};

char cyp1[32] = {"\x21\xFE\x8D\xDA\x4F\x21\xE6\x07\x63\x10\x65\x12\x5C\x39\x5B\xBC\x1C\x1C\x00\xCB\xFA\x60\x24\x35\x0C\x46\x4C\xD7\x0A\x3E\xA6\x16"};

char cxq1[32] = {"\x37\x22\x75\x52\x92\x13\x0B\x08\xD2\xAA\xB9\x7F\xD3\x4E\xC1\x20\xEE\x26\x59\x48\xD1\x9C\x17\xAB\xF9\xB7\x21\x3B\xAF\x82\xD6\x5B"};

char cxq2[32] = {"\x85\xAE\xF3\xD0\x78\x64\x0C\x98\x59\x7B\x60\x27\xB4\x41\xA0\x1F\xF1\xDD\x2C\x19\x0F\x5E\x93\xC4\x54\x80\x6C\x11\xD8\x80\x61\x41"};

char cyq1[32] = {"\xA7\xCF\x28\xD5\x19\xBE\x3D\xA6\x5F\x31\x70\x15\x3D\x27\x8F\xF2\x47\xEF\xBA\x98\xA7\x1A\x08\x11\x62\x15\xBB\xA5\xC9\x99\xA7\xC7"};

char cyq2[32] = {"\x17\x50\x9B\x09\x2E\x84\x5C\x12\x66\xBA\x0D\x26\x2C\xBE\xE6\xED\x07\x36\xA9\x6F\xA3\x47\xC8\xBD\x85\x6D\xC7\x6B\x84\xEB\xEB\x96"};

char cmasterkey[32] = {"\x00\x01\x30\xE7\x84\x59\xD7\x85\x45\xCB\x54\xC5\x87\xE0\x2C\xF4\x80\xCE\x0B\x66\x34\x0F\x31\x9F\x34\x8A\x1D\x5B\x1F\x2D\xC5\xF4"};

char sm9Xx[32] = {"\x3f\x23\xea\x58\xe5\x72\x0b\xdb\x84\x3c\x6c\xfa\x9c\x08\x67\x49\x47\xc5\xc8\x6e\x0d\xdd\x04\xed\xa9\x1d\x83\x54\x37\x7b\x69\x8b"};
//================================

int SM9_Init(unsigned int curve, BOOL TWIST_TYPE,unsigned int seclevel, unsigned char* t, unsigned char* q, unsigned char* a, unsigned char* b, unsigned char* n, unsigned char* xp1, unsigned char* yp1, unsigned char* xq1, unsigned char* xq2, unsigned char* yq1, unsigned char* yq2){
    miracl *mr_mip = mr_first_alloc();
	zzn2 xx;
    zzn2 yy;
	big x;
    big y;
#ifdef MR_OS_THREADS
    miracl *mr_mip = get_mip();
#endif
    mr_mip = mirsys(128,0);
    
    
    sm9q = mirvar(_MIPP_ 0);
    sm9a = mirvar(_MIPP_ 0);
    sm9b = mirvar(_MIPP_ 0);
    sm9n = mirvar(_MIPP_ 0);
    sm9t = mirvar(_MIPP_ 0);
    
    zzn2_mirvar(_MIPP_ &sm9X);
    
    p1G1 = *(epoint_init(_MIPPO_));
    ppG1 = *(epoint_init(_MIPPO_));
    keG1 = *(epoint_init(_MIPPO_));
    
    ecn2_mirvar(_MIPP_ &p2G2);
    ecn2_mirvar(_MIPP_ &ppG2);
    
    
    zzn12_mirvar(_MIPP_ &gGt);
    zzn12_mirvar(_MIPP_ &eGt);
    zzn12_mirvar(_MIPP_ &kGt);
    //========================
   
    zzn2_mirvar(_MIPP_ &xx);
    zzn2_mirvar(_MIPP_ &yy);
    
 
 
    x = mirvar(_MIPP_ 0);
    y = mirvar(_MIPP_ 0);

    
    if (curve){
        sm9len = seclevel;
        TWIST = TWIST_TYPE;
        bytes_to_big(_MIPP_ sm9len, (char *)t, sm9t);
        bytes_to_big(_MIPP_ sm9len, (char *)q, sm9q);
        bytes_to_big(_MIPP_ sm9len, (char *)a, sm9a);
        bytes_to_big(_MIPP_ sm9len, (char *)b, sm9b);
        bytes_to_big(_MIPP_ sm9len, (char *)n, sm9n);
        
        
        mr_mip->TWIST = TWIST;
        ecurve_init(_MIPP_ sm9a,sm9b,sm9q,MR_PROJECTIVE);
        
        
        bytes_to_big(_MIPP_ sm9len, (char *)xp1, x);
        bytes_to_big(_MIPP_ sm9len, (char *)yp1, y);
        
        if (!epoint_set(_MIPP_ x, y, 1, &p1G1)) {
            mirkill(sm9q);
            mirkill(sm9a);
            mirkill(sm9b);
            mirkill(sm9n);
            mirkill(sm9t);
            
            zzn2_kill(_MIPP_ &sm9X);
            epoint_free(&p1G1);
            epoint_free(&ppG1);
            epoint_free(&keG1);
            ecn2_kill(_MIPP_ &p2G2);
            ecn2_kill(_MIPP_ &ppG2);
            zzn12_kill(_MIPP_ &gGt);
            zzn12_kill(_MIPP_ &eGt);
            zzn12_kill(_MIPP_ &kGt);
            
            
            zzn2_kill(_MIPP_ &xx);
            zzn2_kill(_MIPP_ &yy);
            mirkill(x);
            mirkill(y);
            
            CloseMiracl(_MIPPO_);
            return 1;
        }
        
        bytes_to_big(_MIPP_ sm9len, (char *)xq1, x);
        bytes_to_big(_MIPP_ sm9len, (char *)xq2, y);
        zzn2_from_bigs(_MIPP_ x, y, &xx);
        
        bytes_to_big(_MIPP_ sm9len, (char *)yq1, x);
        bytes_to_big(_MIPP_ sm9len, (char *)yq2, y);
        zzn2_from_bigs(_MIPP_ x, y, &yy);
        
        if(!ecn2_set(_MIPP_ &xx, &yy, &p2G2)){
            mirkill(sm9q);
            mirkill(sm9a);
            mirkill(sm9b);
            mirkill(sm9n);
            mirkill(sm9t);
            
            zzn2_kill(_MIPP_ &sm9X);
            epoint_free(&p1G1);
            epoint_free(&ppG1);
            epoint_free(&keG1);
            ecn2_kill(_MIPP_ &p2G2);
            ecn2_kill(_MIPP_ &ppG2);
            zzn12_kill(_MIPP_ &gGt);
            zzn12_kill(_MIPP_ &eGt);
            zzn12_kill(_MIPP_ &kGt);
            
            zzn2_kill(_MIPP_ &xx);
            zzn2_kill(_MIPP_ &yy);
            mirkill(x);
            mirkill(y);
            
            CloseMiracl(_MIPPO_);
            return 2;
        }
        set_frobenius_constant(_MIPP_ &sm9X);
    }else{
        sm9len = 32;
        TWIST = twist;
        bytes_to_big(_MIPP_ sm9len, (char *)ct, sm9t);
        bytes_to_big(_MIPP_ sm9len, (char *)cq, sm9q);
        bytes_to_big(_MIPP_ sm9len, (char *)ca, sm9a);
        bytes_to_big(_MIPP_ sm9len, (char *)cb, sm9b);
        bytes_to_big(_MIPP_ sm9len, (char *)cn, sm9n);
        
        
        mr_mip->TWIST = TWIST;
        ecurve_init(_MIPP_ sm9a,sm9b,sm9q,MR_PROJECTIVE);
        
        bytes_to_big(_MIPP_ sm9len, (char *)cxp1, x);
        bytes_to_big(_MIPP_ sm9len, (char *)cyp1, y);
        
        if (!epoint_set(_MIPP_ x, y, 1, &p1G1)) {
            mirkill(sm9q);
            mirkill(sm9a);
            mirkill(sm9b);
            mirkill(sm9n);
            mirkill(sm9t);
            
            zzn2_kill(_MIPP_ &sm9X);
            epoint_free(&p1G1);
            epoint_free(&ppG1);
            epoint_free(&keG1);
            ecn2_kill(_MIPP_ &p2G2);
            ecn2_kill(_MIPP_ &ppG2);
            zzn12_kill(_MIPP_ &gGt);
            zzn12_kill(_MIPP_ &eGt);
            zzn12_kill(_MIPP_ &kGt);
            
            
            zzn2_kill(_MIPP_ &xx);
            zzn2_kill(_MIPP_ &yy);
            mirkill(x);
            mirkill(y);
            
            CloseMiracl(_MIPPO_);
            return 1;
        }
        
        bytes_to_big(_MIPP_ sm9len, (char *)cxq1, x);
        bytes_to_big(_MIPP_ sm9len, (char *)cxq2, y);
        zzn2_from_bigs(_MIPP_ x, y, &xx);
        
        bytes_to_big(_MIPP_ sm9len, (char *)cyq1, x);
        bytes_to_big(_MIPP_ sm9len, (char *)cyq2, y);
        zzn2_from_bigs(_MIPP_ x, y, &yy);
        
        if(!ecn2_set(_MIPP_ &xx, &yy, &p2G2)){
            mirkill(sm9q);
            mirkill(sm9a);
            mirkill(sm9b);
            mirkill(sm9n);
            mirkill(sm9t);
            
            zzn2_kill(_MIPP_ &sm9X);
            epoint_free(&p1G1);
            epoint_free(&ppG1);
            epoint_free(&keG1);
            ecn2_kill(_MIPP_ &p2G2);
            ecn2_kill(_MIPP_ &ppG2);
            zzn12_kill(_MIPP_ &gGt);
            zzn12_kill(_MIPP_ &eGt);
            zzn12_kill(_MIPP_ &kGt);
            
            zzn2_kill(_MIPP_ &xx);
            zzn2_kill(_MIPP_ &yy);
            mirkill(x);
            mirkill(y);
            
            CloseMiracl(_MIPPO_);
            return 2;
        }
        bytes_to_big(_MIPP_ sm9len, sm9Xx, x);
        zzn2_from_big(_MIPP_ x, &sm9X);
    }
    zzn2_kill(_MIPP_ &xx);
    zzn2_kill(_MIPP_ &yy);
    mirkill(x);
    mirkill(y);
    
    CloseMiracl(_MIPPO_);
    sm9init = TRUE;
    return sm9init;
}

unsigned char* SM9_Set_Sign(unsigned char* x1, unsigned char* x2, unsigned char* y1, unsigned char* y2, unsigned char* gGtchar){
    miracl *mr_mip;
    big x,y;
    zzn2 xx,yy;
    unsigned char *gc;
	if (!sm9init){
    //    printf("the sm9 lib is not init, please run SM9_INIT function");
        return NULL;
    }
    mr_mip = GenMiracl(sm9len);
    
    x = mirvar(_MIPP_ 0);
    y = mirvar(_MIPP_ 0);
    zzn2_mirvar(_MIPP_ &xx);
    zzn2_mirvar(_MIPP_ &yy);
    
    if(gGtchar == NULL){
        bytes_to_big(_MIPP_ sm9len, (char *)x2, x);
        bytes_to_big(_MIPP_ sm9len, (char *)x1, y);
        zzn2_from_bigs(_MIPP_ x, y, &xx);
    
        bytes_to_big(_MIPP_ sm9len, (char *)y2, x);
        bytes_to_big(_MIPP_ sm9len, (char *)y1, y);
        zzn2_from_bigs(_MIPP_ x, y, &yy);
    
        ecn2_set(_MIPP_ &xx, &yy, &ppG2);
    
        sm9sign = ecap(_MIPP_ &ppG2, &p1G1, sm9t, &sm9X, &gGt);
        gc = (unsigned char*)malloc(sizeof(unsigned char)*12*sm9len);
        zzn12_tochar(_MIPP_ &gGt, gc,sm9len);
    }else{
        zzn12_fromchar(_MIPP_ &gGt, gGtchar,sm9len);
    }
    mirkill(x);
    mirkill(y);
    zzn2_kill(_MIPP_ &xx);
    zzn2_kill(_MIPP_ &yy);
    CloseMiracl(_MIPPO_);
    return gc;
}
unsigned char* SM9_Set_Encrypt(unsigned char* x, unsigned char* y, unsigned char* eGtchar){
    miracl *mr_mip;
    big a,b;
    unsigned char *gc;
	if (!sm9init){
    //    printf("the sm9 lib is not init, please run SM9_INIT function");
        return NULL;
    }
    mr_mip = GenMiracl(sm9len);

    a = mirvar(_MIPP_ 0);
    b = mirvar(_MIPP_ 0);
    if (eGtchar == NULL){
        bytes_to_big(_MIPP_ sm9len, (char *) x, a);
        bytes_to_big(_MIPP_ sm9len, (char *) y, b);
    
        if(!epoint_set(_MIPP_ a, b, 1, &ppG1)){
            mirkill(a);
            mirkill(b);
            CloseMiracl(_MIPPO_);
            return NULL;
        }
    
        sm9encrypt = ecap(_MIPP_ &p2G2, &ppG1, sm9t, &sm9X, &eGt);
        gc = (unsigned char*)malloc(sizeof(unsigned char)*12*sm9len);
        zzn12_tochar(_MIPP_ &eGt, gc,sm9len);
    }else{
        zzn12_fromchar(_MIPP_ &eGt, gc,sm9len);
    }
    mirkill(a);
    mirkill(b);
    CloseMiracl(_MIPPO_);
    return gc;
}

unsigned char* SM9_Set_KeyExchange(unsigned char* x, unsigned char* y,unsigned char* kGtchar){
    miracl *mr_mip;
    big a,b;
    unsigned char* gc;
    if (!sm9init){
    //    printf("the sm9 lib is not init, please run SM9_INIT function");
        return NULL;
    }
    mr_mip = GenMiracl(sm9len);
    a = mirvar(_MIPP_ 0);
    b = mirvar(_MIPP_ 0);
    if (kGtchar == NULL){
        bytes_to_big(_MIPP_ sm9len, (char *) x, a);
        bytes_to_big(_MIPP_ sm9len, (char *) y, b);
        
        if (!epoint_set(_MIPP_ a, b, 1, &keG1)){
            mirkill(a);
            mirkill(b);
            CloseMiracl(_MIPPO_);
            return NULL;
        }
        sm9keyexchange = ecap(_MIPP_ &p2G2, &keG1, sm9t, &sm9X, &kGt);
        gc = (unsigned char*)malloc(sizeof(unsigned char)*12*sm9len);
        zzn12_tochar(_MIPP_ &kGt, gc,sm9len);
    }else{
        zzn12_fromchar(_MIPP_ &kGt, gc,sm9len);
    }
    mirkill(a);
    mirkill(b);
    CloseMiracl(_MIPPO_);
    return gc;
}
SM9_MSK SM9_MSK_New(int secLevel,unsigned char* w){
    SM9_MSK msk;
    msk.keylen = secLevel;
    msk.msk = (unsigned char *)malloc(sizeof(char)*secLevel);
    memcpy(msk.msk, w, secLevel);
    return msk;
}

SM9_MSPK SM9_MSPK_New(int secLevel){
    SM9_MSPK mpk;
    mpk.secLevel = secLevel;
    mpk.x1 = (unsigned char *)malloc(sizeof(char)*secLevel);
    mpk.x2 = (unsigned char *)malloc(sizeof(char)*secLevel);
    mpk.y1 = (unsigned char *)malloc(sizeof(char)*secLevel);
    mpk.y2 = (unsigned char *)malloc(sizeof(char)*secLevel);
    return mpk;
}

SM9_MCPK SM9_MCPK_New(int secLevel){
    SM9_MCPK mpk;
    mpk.secLevel = secLevel;
    mpk.x = (unsigned char *)malloc(sizeof(char)*secLevel);
    mpk.y = (unsigned char *)malloc(sizeof(char)*secLevel);
    return mpk;
}

SM9_PK SM9_PK_New(int len,unsigned char* w){
    SM9_PK pk;
    pk.keylen = len;
    pk.pk = (unsigned char *)malloc(sizeof(char)*len);
    memcpy(pk.pk, w, len);
    return pk;
}

SM9_SSK SM9_SSK_New(int secLevel){
    SM9_SSK sk;
    sk.secLevel = secLevel;
    sk.x = (unsigned char *)malloc(sizeof(char)*secLevel);
    sk.y = (unsigned char *)malloc(sizeof(char)*secLevel);
    return sk;
}

SM9_CSK SM9_CSK_New(int secLevel){
    SM9_CSK sk;
    sk.secLevel = secLevel;
    sk.x1 = (unsigned char *)malloc(sizeof(char)*secLevel);
    sk.x2 = (unsigned char *)malloc(sizeof(char)*secLevel);
    sk.y1 = (unsigned char *)malloc(sizeof(char)*secLevel);
    sk.y2 = (unsigned char *)malloc(sizeof(char)*secLevel);
    return sk;
}


SM9_Sign SM9_Sign_New(int secLevel){
    SM9_Sign sign;
    sign.secLevel = secLevel;
    sign.type = 0x04;
    sign.h = (unsigned char *)malloc(sizeof(char)*secLevel);
    sign.xs = (unsigned char *)malloc(sizeof(char)*secLevel);
    sign.ys = (unsigned char *)malloc(sizeof(char)*secLevel);
    return sign;
}

SM9_Cipher SM9_Cipher_New(int secLevel){
    SM9_Cipher cip;
    cip.secLevel = secLevel;
    cip.cplen = 0;
    cip.x = (unsigned char *)malloc(sizeof(char)*secLevel);
    cip.y = (unsigned char *)malloc(sizeof(char)*secLevel);
    cip.c3 = (unsigned char *)malloc(sizeof(char)*SM3_HASH_LEN);
    return cip;
}


SM9_MKPK SM9_MKPK_New(int secLevel){
    SM9_MKPK mpk;
    mpk.secLevel = secLevel;
    mpk.x = (unsigned char *)malloc(sizeof(char)*secLevel);
    mpk.y = (unsigned char *)malloc(sizeof(char)*secLevel);
    return mpk;
}

SM9_KSK SM9_KSK_New(int secLevel){
    SM9_KSK sk;
    sk.secLevel = secLevel;
    sk.x1 = (unsigned char *)malloc(sizeof(char)*secLevel);
    sk.x2 = (unsigned char *)malloc(sizeof(char)*secLevel);
    sk.y1 = (unsigned char *)malloc(sizeof(char)*secLevel);
    sk.y2 = (unsigned char *)malloc(sizeof(char)*secLevel);
    return sk;
}

SM9_Send SM9_Send_New(int secLevel){
    SM9_Send se;
    se.secLevel = secLevel;
    se.x = (unsigned char *)malloc(sizeof(char)*secLevel);
    se.y = (unsigned char *)malloc(sizeof(char)*secLevel);
    return se;
}


void SM9_MSK_Free(SM9_MSK *msk){
    free(msk->msk);
}
void SM9_MSPK_Free(SM9_MSPK *mpk){
    free(mpk->x1);
    free(mpk->x2);
    free(mpk->y1);
    free(mpk->y2);
}
void SM9_MCPK_Free(SM9_MCPK *mpk){
    free(mpk->x);
    free(mpk->y);
}


void SM9_PK_Free(SM9_PK *pk){
    free(pk->pk);
}
void SM9_SSK_Free(SM9_SSK *sk){
    free(sk->x);
    free(sk->y);
}
void SM9_CSK_Free(SM9_CSK *sk){
    free(sk->x1);
    free(sk->x2);
    free(sk->y1);
    free(sk->y2);
}

void SM9_MKPK_Free(SM9_MKPK *mpk){
    free(mpk->x);
    free(mpk->y);
}

void SM9_KSK_Free(SM9_KSK *sk){
    free(sk->x1);
    free(sk->x2);
    free(sk->y1);
    free(sk->y2);
}

void SM9_Sign_Free(SM9_Sign *s){
    free(s->h);
    free(s->xs);
    free(s->ys);
}
void SM9_Cipher_Free(SM9_Cipher *c){
    free(c->c3);
    free(c->x);
    free(c->y);
    if(!c->cplen) free(c->cp);
}

void SM9_Send_Free(SM9_Send *s){
    free(s->x);
    free(s->y);
}
BOOL SM9_GenMSignPubKey(SM9_MSK *msk, SM9_MSPK *mspk){
    miracl *mr_mip;
    big mk;
	if (!sm9init){
    //    printf("the sm9 lib is not init, please run SM9_INIT function");
        return sm9init;
    }
    mr_mip = GenMiracl(mspk->secLevel);
	mk = mirvar(_MIPP_ 0);
    bytes_to_big(_MIPP_ mspk->secLevel, (char *)msk->msk, mk);
    
    ecn2_copy(&p2G2, &ppG2);
    ecn2_mul(_MIPP_ mk, &ppG2);
    ecn2_norm(_MIPP_ &ppG2);
    
    redc(_MIPP_ ppG2.x.a, mk);
    big_to_bytes(_MIPP_ mspk->secLevel, mk, (char *)mspk->x2, TRUE);
    
    redc(_MIPP_ ppG2.x.b, mk);
    big_to_bytes(_MIPP_ mspk->secLevel, mk, (char *)mspk->x1, TRUE);
    
    redc(_MIPP_ ppG2.y.a, mk);
    big_to_bytes(_MIPP_ mspk->secLevel, mk, (char *)mspk->y2, TRUE);
    
    redc(_MIPP_ ppG2.y.b, mk);
    big_to_bytes(_MIPP_ mspk->secLevel, mk, (char *)mspk->y1, TRUE);
    
    
    mirkill(mk);
    CloseMiracl(_MIPPO_);
    return TRUE;
}

BOOL SM9_GenMEncryptPubKey(SM9_MSK *msk, SM9_MCPK *mcpk){
    miracl *mr_mip;
    big mk;
	if (!sm9init){
    //    printf("the sm9 lib is not init, please run SM9_INIT function");
        return sm9init;
    }
    mr_mip = GenMiracl(mcpk->secLevel);
	mk = mirvar(_MIPP_ 0);
    bytes_to_big(_MIPP_ mcpk->secLevel, (char *)msk->msk, mk);
    
    ecurve_mult(_MIPP_ mk, &p1G1, &ppG1);
    epoint_norm(_MIPP_ &ppG1);

    redc(_MIPP_ ppG1.X, mk);
    big_to_bytes(_MIPP_ mcpk->secLevel, mk, (char *)mcpk->x, TRUE);
    redc(_MIPP_ ppG1.Y, mk);
    big_to_bytes(_MIPP_ mcpk->secLevel, mk, (char *)mcpk->y, TRUE);
    
    mirkill(mk);
    CloseMiracl(_MIPPO_);
    return TRUE;
}

BOOL SM9_GenMKeyExchangePubKey(SM9_MSK *msk, SM9_MKPK *mcpk){
    miracl *mr_mip;
    big mk;
    
    if (!sm9init){
    //    printf("the sm9 lib is not init, please run SM9_INIT function");
        return sm9init;
    }
    mr_mip = GenMiracl(mcpk->secLevel);
    mk = mirvar(_MIPP_ 0);
    
    bytes_to_big(_MIPP_ mcpk->secLevel, (char *)msk->msk, mk);
    
    ecurve_mult(_MIPP_ mk, &p1G1, &keG1);
    epoint_norm(_MIPP_ &keG1);
    
    redc(_MIPP_ keG1.X, mk);
    big_to_bytes(_MIPP_ mcpk->secLevel, mk, (char *)mcpk->x, TRUE);
    redc(_MIPP_ keG1.Y, mk);
    big_to_bytes(_MIPP_ mcpk->secLevel, mk, (char *)mcpk->y, TRUE);
    
    mirkill(mk);
    CloseMiracl(_MIPPO_);
    return TRUE;
}



