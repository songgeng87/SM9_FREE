//
//  sm_r-ate.c
//  SM9_FREE
//
//  Created by 宋赓 on 2019/3/15.
//  Copyright © 2019 宋赓. All rights reserved.
//

#include <stdio.h>
#include "../miracl/miracl.h"

#include "print_out.h"
#include "sm9_utils.h"
//
// R-ate Pairing Code
//


void set_frobenius_constant(_MIPD_ zzn2 *x){
 //   miracl *mip=get_mip();
 //   copy(mip->modulus,m.fn);
    big ONE = mirvar(_MIPP_ 1);
    big ZERO = mirvar(_MIPP_ 0);
    big TWO = mirvar(_MIPP_ 2);
    big sx = mirvar(_MIPP_ 6);
    big sxx = mirvar(_MIPP_ 0);
    big p = mirvar(_MIPP_ 0);
    

    copy(mr_mip->modulus, p);

#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    switch (mr_mip->pmod8)
    {
        case 5:
            zzn2_from_bigs(_MIPP_ ZERO, ONE, x);
            break;
        case 3:
            zzn2_from_bigs(_MIPP_ ONE, ONE, x);
            break;
        case 7:
            zzn2_from_bigs(_MIPP_ TWO, ONE, x);
            break;
        default:
            break;
    }
    xgcd(_MIPP_ sx, p, sx, sx, sx);
    decr(_MIPP_ p, 1, p);
    multiply(_MIPP_ sx, p, sxx);
    
    copy(mr_mip->modulus, p);
    divide(_MIPP_ sxx,p,p);
    

	zzn2_pow(_MIPP_ x, sxx, x);
    
    

    mirkill(ONE);
    mirkill(ZERO);
    mirkill(TWO);
    mirkill(sx);
    mirkill(p);
}

//
// Line from A to destination C. Let A=(x,y)
// Line Y-slope.X-c=0, through A, so intercept c=y-slope.x
// Line Y-slope.X-y+slope.x = (Y-y)-slope.(X-x) = 0
// Now evaluate at Q -> return (Qy-y)-slope.(Qx-x)
//

void line(_MIPD_ ecn2 *A,ecn2 *C,ecn2 *B,zzn2 *slope,zzn2 *extra,BOOL Doubling,zzn Qx,zzn Qy,zzn12 *wt)
{
    zzn12 w;
    
    zzn4 nn,dd,cc;
    zzn2 X,Y;
    zzn2 Z3,txx;
    zzn2 w1,w2,w3;
    
    zzn12_mirvar(_MIPP_ &w);
    zzn4_mirvar(_MIPP_ &nn);
    zzn4_mirvar(_MIPP_ &dd);
    zzn4_mirvar(_MIPP_ &cc);
    zzn2_mirvar(_MIPP_ &X);
    zzn2_mirvar(_MIPP_ &Y);
    zzn2_mirvar(_MIPP_ &Z3);
    zzn2_mirvar(_MIPP_ &w1);
    zzn2_mirvar(_MIPP_ &w2);
    zzn2_mirvar(_MIPP_ &w3);
    zzn2_mirvar(_MIPP_ &txx);
    
    ecn2_getz(_MIPP_ C, &Z3);
    
    
    if (Doubling)
    {
        zzn2 Z,ZZ;
        zzn2_mirvar(_MIPP_ &Z);
        zzn2_mirvar(_MIPP_ &ZZ);
        ecn2_get(_MIPP_ A, &X, &Y, &Z);
        zzn2_copy(&Z, &ZZ);
        zzn2_mul(_MIPP_ &ZZ, &ZZ, &ZZ);
        if (mr_mip->TWIST==MR_SEXTIC_M)
        { // "multiplied across" by i to simplify
            
            zzn2_mul(_MIPP_ &ZZ, &Z3, &w1); //
            zzn2_from_zzn((big)Qy, &txx);
            zzn2_txx(_MIPP_ &txx);
            zzn2_mul(_MIPP_ &w1, &txx, &w1);
            
            zzn2_mul(_MIPP_ slope, &X, &w2);
            zzn2_sub(_MIPP_ &w2, extra, &w2);
            
            zzn4_from_zzn2s(&w1, &w2, &nn);
            //nn.set((Z3*ZZ)*txx((ZZn2)Qy),slope*X-extra);

            zzn2_mul(_MIPP_ &ZZ,slope, &w1);
            zzn2_from_zzn((big)Qx, &w2);
            zzn2_mul(_MIPP_ &w1,&w2, &w1);
            zzn2_negate(_MIPP_ &w1, &w1);
            
            zzn4_from_zzn2h(&w1, &cc);
                        //cc.seth(-(ZZ*slope)*Qx);
        }
        if (mr_mip->TWIST==MR_SEXTIC_D)
        {
            //todo
          //  printf("to do \n");
            
            //nn.set((Z3*ZZ)*Qy,slope*X-extra);
            //dd.set(-(ZZ*slope)*Qx);
        }
        zzn2_kill(_MIPP_ &Z);
        zzn2_kill(_MIPP_ &ZZ);
    }
    else
    {
        zzn2 X2,Y2;
        zzn2_mirvar(_MIPP_ &X2);
        zzn2_mirvar(_MIPP_ &Y2);
        
        ecn2_getxy(B, &X2, &Y2);
        
        if (mr_mip->TWIST==MR_SEXTIC_M)
        {
            zzn2_from_zzn((big)Qy, &txx);
            zzn2_txx(_MIPP_ &txx);
            zzn2_mul(_MIPP_ &Z3, &txx, &w1);
            zzn2_mul(_MIPP_ slope, &X2, &w2);
            zzn2_mul(_MIPP_ &Y2, &Z3, &w3);
            zzn2_sub(_MIPP_ &w2, &w3, &w2);
            zzn4_from_zzn2s(&w1, &w2, &nn);
            //nn.set(Z3*txx((ZZn2)Qy),slope*X2-Y2*Z3);
            
            zzn2_from_zzn((big)Qx, &w1);
            zzn2_mul(_MIPP_ slope, &w1, &w1);
            zzn2_negate(_MIPP_ &w1, &w1);
            zzn4_from_zzn2h(&w1, &cc);
            
            //cc.seth(-slope*Qx);
        }
        if (mr_mip->TWIST==MR_SEXTIC_D)
        {
         //   printf("to do \n");
            // todo
            // nn.set(Z3*Qy,slope*X2-Y2*Z3);
           // dd.set(-slope*Qx);
        }
        zzn2_kill(_MIPP_ &X2);
        zzn2_kill(_MIPP_ &Y2);
    }
    zzn12_from_zzn4s(&nn, &dd, &cc, &w);
    //w.set(nn,dd,cc);
    
    zzn12_copy(&w, wt);
    zzn12_kill(_MIPP_ &w);
    zzn4_kill(_MIPP_ &nn);
    zzn4_kill(_MIPP_ &dd);
    zzn4_kill(_MIPP_ &cc);
    zzn2_kill(_MIPP_ &X);
    zzn2_kill(_MIPP_ &Y);
    zzn2_kill(_MIPP_ &Z3);
    zzn2_kill(_MIPP_ &w1);
    zzn2_kill(_MIPP_ &w2);
    zzn2_kill(_MIPP_ &w3);
    zzn2_kill(_MIPP_ &txx);
    return ;
}

void endomorph(_MIPD_ ecn *A,zzn *Beta)
{ // apply endomorphism (x,y) = (Beta*x,y) where Beta is cube root of unity
    big x;
    x = mirvar(_MIPP_ 0);
    copy(A->X,x);
    multiply(_MIPP_ x, (big) Beta, x);
    divide(_MIPP_ x,mr_mip->modulus,mr_mip->modulus);
    copy(x, A->X);
    mirkill(x);
    
    
    //ZZn x;
    //x=(A.get_point())->X;
    //x*=Beta;
    //copy(getbig(x),(A.get_point())->X);
}
void q_power_frobenius(_MIPD_ ecn2 *A,zzn2 *F)
{// apply endomorphism (x,y) = (Beta*x,y) where Beta is cube root of unity
    zzn2 x,y,z,w,r,cj;
    zzn2_mirvar(_MIPP_ &x);
    zzn2_mirvar(_MIPP_ &y);
    zzn2_mirvar(_MIPP_ &z);
    zzn2_mirvar(_MIPP_ &w);
    zzn2_mirvar(_MIPP_ &r);
    zzn2_mirvar(_MIPP_ &cj);
    
    ecn2_get(_MIPP_ A, &x, &y, &z);
    
    
    
    zzn2_copy(F, &r);
    
    if (mr_mip->TWIST==MR_SEXTIC_M) zzn2_inv(_MIPP_ &r);  // could be precalculated
    
    zzn2_mul(_MIPP_ &r, &r, &w);
    
    zzn2_conj(_MIPP_ &x, &cj);
    zzn2_mul(_MIPP_ &cj, &w, &x);
    
    
    zzn2_conj(_MIPP_ &y, &cj);
    zzn2_mul(_MIPP_ &cj, &w, &y);
    zzn2_mul(_MIPP_ &y, &r, &y);
    zzn2_conj(_MIPP_ &z, &z);
    
    ecn2_setxyz(_MIPP_ &x, &y, &z, A);
    //ZZn2 x,y,z,w,r;
    
    //A.get(x,y,z);
    //w=F*F;
    //r=F;
    //x=w*conj(x);
    //y=r*w*conj(y);
    //z.conj();
    //A.set(x,y,z);


    zzn2_kill(_MIPP_ &x);
    zzn2_kill(_MIPP_ &y);
    zzn2_kill(_MIPP_ &z);
    zzn2_kill(_MIPP_ &w);
    zzn2_kill(_MIPP_ &r);
    zzn2_kill(_MIPP_ &cj);
}


//
// Add A=A+B  (or A=A+A)
// Return line function value
//
void g(_MIPD_ ecn2 *A,ecn2 *B,zzn Qx,zzn Qy, zzn12 *wr)
{
    zzn2 lam,extra;
    zzn12 r;
    ecn2 P;//=A;
    BOOL Doubling;
  
    zzn2_mirvar(_MIPP_ &lam);
    zzn2_mirvar(_MIPP_ &extra);
    zzn12_mirvar(_MIPP_ &r);
    ecn2_mirvar(_MIPP_ &P);
    
    ecn2_copy(A, &P);
    
// Evaluate line from A
    
//    Doubling=A.add(B,lam,extra);
//    if (A.iszero())   return (ZZn12)1;
//    r=line(P,A,B,lam,extra,Doubling,Qx,Qy);
    
    Doubling = ecn2_add2(_MIPP_ B, A, &lam, &extra);
    
    if (ecn2_iszero(A)) {
        zzn12_from_int(_MIPP_ 1, &r);
    }else{
        line(_MIPP_ &P,A,B,&lam, &extra, Doubling, Qx, Qy, &r);
    }
    
    zzn12_copy(&r, wr);
    
    zzn2_kill(_MIPP_ &lam);
    zzn2_kill(_MIPP_ &extra);
    ecn2_kill(_MIPP_ &P);
    zzn12_kill(_MIPP_ &r);
    return ;
}


//
// R-ate Pairing G2 x G1 -> GT
//
// P is a point of order q in G1. Q(x,y) is a point of order q in G2.
// Note that P is a point on the sextic twist of the curve over Fp^2, Q(x,y) is a point on the
// curve over the base field Fp
//
BOOL fast_pairing(_MIPD_ ecn2 *P,zzn Qx,zzn Qy,big x,zzn2 *X,zzn12 *res)
{
    ecn2 A,KA;
    zzn2 AX,AY;
    int i,nb;
    big n,ZERO;
    zzn12 r;
    zzn12 t0,t1;
    zzn12 x0,x1,x2,x3,x4,x5;
    zzn12 w1,w2;
    
    ecn2_mirvar(_MIPP_ &A);
    ecn2_mirvar(_MIPP_ &KA);
    zzn2_mirvar(_MIPP_ &AX);
    zzn2_mirvar(_MIPP_ &AY);
    n = mirvar(_MIPP_ 0);
    ZERO = mirvar(_MIPP_ 0);
    zzn12_mirvar(_MIPP_ &r);
    zzn12_mirvar(_MIPP_ &t0);
    zzn12_mirvar(_MIPP_ &t1);
    zzn12_mirvar(_MIPP_ &x0);
    zzn12_mirvar(_MIPP_ &x1);
    zzn12_mirvar(_MIPP_ &x2);
    zzn12_mirvar(_MIPP_ &x3);
    zzn12_mirvar(_MIPP_ &x4);
    zzn12_mirvar(_MIPP_ &x5);
    zzn12_mirvar(_MIPP_ &w1);
    zzn12_mirvar(_MIPP_ &w2);
    
/*    if (x<0) n=-(6*x+2);
    else n=6*x+2;
    A=P;
    nb=bits(n);
    r=1;
    // Short Miller loop
    r.mark_as_miller();
*/

    if ((mr_compare(x, ZERO))==-1){
        premult(_MIPP_ x, 6, n);
        incr(_MIPP_ n, 2, n);
        negify(n, n);
    }else{
        premult(_MIPP_ x, 6, n);
        incr(_MIPP_ n, 2, n);
    }
    
    ecn2_copy(P, &A);
    nb = logb2(_MIPP_ n);
    
    zzn12_from_int(_MIPP_ 1, &r);
    zzn12_mark_miller(_MIPP_ &r);
    /*
     for (i=nb-2;i>=0;i--)
     {
     r*=r;
     r*=g(A,A,Qx,Qy);
     if (bit(n,i))
     r*=g(A,P,Qx,Qy);
     }
     */
    for (i=nb-2;i>=0;i--){
        zzn12_mul(_MIPP_ &r, &r, &r);
        g(_MIPP_ &A, &A, Qx, Qy,&t0);
        zzn12_mul(_MIPP_ &r, &t0, &r);
        if (mr_testbit(_MIPP_ n,i)){
            g(_MIPP_ &A, P, Qx, Qy,&t0);
            zzn12_mul(_MIPP_ &r, &t0, &r);
        }
    }
    // Combining ideas due to Longa, Aranha et al. and Naehrig
/*    KA=P;
    q_power_frobenius(KA,X);
    if (x<0) {A=-A; r.conj();}
    r*=g(A,KA,Qx,Qy);
    q_power_frobenius(KA,X); KA=-KA;
    r*=g(A,KA,Qx,Qy);
*/
    ecn2_copy(P,&KA);
    q_power_frobenius(_MIPP_ &KA, X);
    if ((mr_compare(x, ZERO))<0){
        ecn2_negate(_MIPP_ &A, &A);
        zzn12_conj(_MIPP_ &r, &r);
    }
    g(_MIPP_ &A, &KA, Qx, Qy,&t0);
    zzn12_mul(_MIPP_ &r, &t0, &r);
    q_power_frobenius(_MIPP_ &KA, X);
    ecn2_negate(_MIPP_ &KA, &KA);
    g(_MIPP_ &A, &KA, Qx, Qy,&t0);
    zzn12_mul(_MIPP_ &r, &t0, &r);
    
   
   
//    if (r.iszero()) return FALSE;
    if (zzn12_iszero(_MIPP_ &r)) {
        ecn2_kill(_MIPP_ &A);
        ecn2_kill(_MIPP_ &KA);
        zzn2_kill(_MIPP_ &AX);
        zzn2_kill(_MIPP_ &AY);
        mirkill(n);
        mirkill(ZERO);
        zzn12_kill(_MIPP_ &r);
        zzn12_kill(_MIPP_ &t0);
        zzn12_kill(_MIPP_ &t1);
        zzn12_kill(_MIPP_ &x0);
        zzn12_kill(_MIPP_ &x1);
        zzn12_kill(_MIPP_ &x2);
        zzn12_kill(_MIPP_ &x3);
        zzn12_kill(_MIPP_ &x4);
        zzn12_kill(_MIPP_ &x5);
        zzn12_kill(_MIPP_ &w1);
        zzn12_kill(_MIPP_ &w2);
        return FALSE;
    }
    
    // The final exponentiation
    
/*    t0=r;
    
    r.conj();
    
    r/=t0;    // r^(p^6-1)
    r.mark_as_regular();  // no longer "miller"
    
    t0=r;
    r.powq(X); r.powq(X);
    r*=t0;    // r^[(p^6-1)*(p^2+1)]
    
    r.mark_as_unitary();  // from now on all inverses are just conjugates !! (and squarings are faster)
    
    res=r;*/
    
    zzn12_copy(&r, &t0);
    zzn12_conj(_MIPP_ &r, &r);
    zzn12_inv(_MIPP_ &t0);
    zzn12_mul(_MIPP_ &r, &t0, &r);
    zzn12_mark_regular(_MIPP_ &r);
    
    zzn12_copy(&r, &t0);
    zzn12_powq(_MIPP_ &r, X);
    zzn12_powq(_MIPP_ &r, X);
    zzn12_mul(_MIPP_ &r, &t0, &r);
    zzn12_mark_unitary(_MIPP_ &r);
    
    zzn12_copy(&r, res);
    
    // Newer new idea...
    // See "On the final exponentiation for calculating pairings on ordinary elliptic curves"
    // Michael Scott and Naomi Benger and Manuel Charlemagne and Luis J. Dominguez Perez and Ezekiel J. Kachisa
 /*   t0=res;    t0.powq(X);
    x0=t0;   x0.powq(X);
    
    x0*=(res*t0);
    x0.powq(X);
    
    x1=inverse(res);  // just a conjugation!
    
    x4=pow(res,-x);  // x is sparse..
    x3=x4; x3.powq(X);
*/
    zzn12_copy(res, &t0);   zzn12_powq(_MIPP_ &t0, X);
    zzn12_copy(&t0, &x0);   zzn12_powq(_MIPP_ &x0, X);
    
    zzn12_mul(_MIPP_ res, &t0, &w1);
    zzn12_mul(_MIPP_ &x0, &w1, &x0);
    zzn12_powq(_MIPP_ &x0, X);
    
    zzn12_copy(res, &x1);
    zzn12_inv(_MIPP_ &x1);
    
    zzn12_copy(res, &x4);
    zzn12_pow(_MIPP_ &x4, x);
    zzn12_inv(_MIPP_ &x4);
    
    zzn12_copy(&x4, &x3);
    zzn12_powq(_MIPP_ &x3, X);
    
    
/*
    x2=pow(x4,-x);
    x5=inverse(x2);
    t0=pow(x2,-x);
    
    x2.powq(X);
    x4/=x2;
    
    x2.powq(X);
    
    res=t0; res.powq(X); t0*=res;
*/
    zzn12_copy(&x4, &x2);
    zzn12_pow(_MIPP_ &x2, x);
    zzn12_copy(&x2, &x5);
    zzn12_copy(&x2, &t0);
    zzn12_inv(_MIPP_ &x2);
    zzn12_pow(_MIPP_ &t0, x);
    
    zzn12_powq(_MIPP_ &x2, X);
    
    zzn12_copy(&x2, &w1);
    zzn12_inv(_MIPP_ &w1);
    zzn12_mul(_MIPP_ &x4, &w1, &x4);
    
    zzn12_powq(_MIPP_ &x2, X);
    
    zzn12_copy(&t0,res);
    zzn12_powq(_MIPP_ res, X);

    zzn12_mul(_MIPP_ &t0, res, &t0);
    
/*
    t0*=t0;
    t0*=x4;
    t0*=x5;
    
    res=x3*x5;
    res*=t0;
    t0*=x2;
    
    res*=res;
    res*=t0;
    res*=res;
    
    t0=res*x1;
    res*=x0;
    t0*=t0;
    t0*=res;
*/
    zzn12_mul(_MIPP_ &t0, &t0, &t0);
    zzn12_mul(_MIPP_ &t0, &x4, &t0);
    zzn12_mul(_MIPP_ &t0, &x5, &t0);
    
    zzn12_mul(_MIPP_ &x3, &x5, res);
    zzn12_mul(_MIPP_ res, &t0, res);
    zzn12_mul(_MIPP_ &t0, &x2, &t0);
    
    zzn12_mul(_MIPP_ res, res, res);
    zzn12_mul(_MIPP_ res, &t0, res);
    zzn12_mul(_MIPP_ res, res, res);
    
    zzn12_mul(_MIPP_ res, &x1, &t0);
    zzn12_mul(_MIPP_ res, &x0, res);
    zzn12_mul(_MIPP_ &t0, &t0, &t0);
    zzn12_mul(_MIPP_ &t0, res, &t0);
    
    zzn12_copy(&t0, res);
    
    ecn2_kill(_MIPP_ &A);
    ecn2_kill(_MIPP_ &KA);
    zzn2_kill(_MIPP_ &AX);
    zzn2_kill(_MIPP_ &AY);
    mirkill(n);
    mirkill(ZERO);
    zzn12_kill(_MIPP_ &r);
    zzn12_kill(_MIPP_ &t0);
    zzn12_kill(_MIPP_ &t1);
    zzn12_kill(_MIPP_ &x0);
    zzn12_kill(_MIPP_ &x1);
    zzn12_kill(_MIPP_ &x2);
    zzn12_kill(_MIPP_ &x3);
    zzn12_kill(_MIPP_ &x4);
    zzn12_kill(_MIPP_ &x5);
    zzn12_kill(_MIPP_ &w1);
    zzn12_kill(_MIPP_ &w2);
    return TRUE;
}

//
// ecap(.) function
//

BOOL ecap(_MIPD_ ecn2 *P,ecn *Q,big x,zzn2 *X,zzn12 *r)
{
    /*BOOL Ok;
     Big xx,yy;
     ZZn Qx,Qy;
     
     P.norm();
     Q.get(xx,yy); Qx=xx; Qy=yy;
     
     Ok=fast_pairing(P,Qx,Qy,x,X,r);
     
     if (Ok) return TRUE;
     return FALSE;*/
    BOOL Ok;
    big xx,yy;
    zzn Qx,Qy;
    
    xx = mirvar(_MIPP_  0);
    yy = mirvar(_MIPP_  0);
    Qx = mirvar(_MIPP_  0);
    Qy = mirvar(_MIPP_  0);
    
    ecn2_norm(_MIPP_ P);
    epoint_get(_MIPP_ Q, xx, yy);
    
    nres(_MIPP_ xx, Qx);
    nres(_MIPP_ yy, Qy);
    


    Ok = fast_pairing(_MIPP_ P, Qx, Qy, x, X, r);
    
    
    mirkill(xx);
    mirkill(yy);
    mirkill(Qx);
    mirkill(Qy);
    
    if (Ok) return TRUE;
    return FALSE;
}
