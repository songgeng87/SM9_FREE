//
//  smzzn12.h
//  SM9_FREE
//
//  Created by 宋赓 on 2019/3/15.
//  Copyright © 2019 宋赓. All rights reserved.
//

#include <stdlib.h> 
#include "smzzn12.h"
#define FUNC_BASE 226

void zzn12_mirvar(_MIPD_ zzn12*w){
    zzn4_mirvar(_MIPP_ &(w->a));
    zzn4_mirvar(_MIPP_ &(w->b));
    zzn4_mirvar(_MIPP_ &(w->c));
    w->miller = FALSE;
    w->unitary = FALSE;
}

void zzn12_kill(_MIPD_ zzn12*w){
    zzn4_kill(_MIPP_ &(w->a));
    zzn4_kill(_MIPP_ &(w->b));
    zzn4_kill(_MIPP_ &(w->c));
}

void zzn12_mark_miller(_MIPD_ zzn12* w){
    w->miller = TRUE;
}

void zzn12_mark_regular(_MIPD_ zzn12* w){
    w->miller = FALSE;
    w->unitary = FALSE;
}

void zzn12_mark_unitary(_MIPD_ zzn12* w){
    w->miller = FALSE;
    w->unitary = TRUE;
}

void zzn12_from_int(_MIPD_ int i,zzn12 *x){
#ifdef MR_OS_THREADS
    miracl *mr_mip=get_mip();
#endif
    if (mr_mip->ERNUM) return;
    
    MR_IN(FUNC_BASE+0)
    zzn4_from_int(_MIPP_ i, &(x->a));
    zzn4_zero(&(x->b));
    zzn4_zero(&(x->c));
    x->miller = FALSE;
    if (i == 1) {
        x->unitary = TRUE;
    }else{
        x->unitary = FALSE;
    }
    MR_OUT
}

BOOL zzn12_isunity(_MIPD_ zzn12 *x){
    return x->unitary;
}

BOOL zzn12_iszero(_MIPD_ zzn12 *w){
    if(zzn4_iszero(&(w->a)) && zzn4_iszero(&(w->b)) && zzn4_iszero(&(w->c))){
        return TRUE;
    }
    return FALSE;
}

void zzn12_copy(zzn12 *w,zzn12 *x){
    zzn4_copy(&(w->a), &(x->a));
    zzn4_copy(&(w->b), &(x->b));
    zzn4_copy(&(w->c), &(x->c));
    x->miller = w->miller;
    x->unitary = w->unitary;
}
void zzn12_zero(zzn12 *w){
    zzn4_zero(&(w->a));
    zzn4_zero(&(w->b));
    zzn4_zero(&(w->c));
    w->unitary = FALSE;
    w->miller = FALSE;
}
void zzn12_powq(_MIPD_ zzn12 *w,zzn2 *X){
    //ZZn2 XX=X*X;
    //ZZn2 XXX=XX*X;
    //BOOL ku=unitary;
    //BOOL km=miller;
    //a.powq(XXX); b.powq(XXX); c.powq(XXX);
    //b*=X;
    //c*=XX;
    //unitary=ku;
    //miller=km;
    
    zzn12 y;
	zzn2 XX,XXX;
    BOOL ku,km;
	
	zzn12_mirvar(_MIPP_ &y);
    
    zzn12_copy(w, &y);
    
  
    zzn2_mirvar(_MIPP_ &XX);
    zzn2_mul(_MIPP_ X, X, &XX);
    
  
    zzn2_mirvar(_MIPP_ &XXX);
    zzn2_mul(_MIPP_ &XX, X, &XXX);
    
    ku = y.unitary;
    km = y.miller;
    
    zzn4_powq(_MIPP_ &XXX, &(y.a));
    zzn4_powq(_MIPP_ &XXX, &(y.b));
    zzn4_powq(_MIPP_ &XXX, &(y.c));
    
    zzn4_smul(_MIPP_ &(y.b), X, &(y.b));
    zzn4_smul(_MIPP_ &(y.c), &XX, &(y.c));
    
    y.unitary = ku;
    y.miller = km;
    
    zzn12_copy(&y, w);
    
    zzn2_kill(_MIPP_ &XX);
    zzn2_kill(_MIPP_ &XXX);
    zzn12_kill(_MIPP_ &y);
    
}
void zzn12_pow(_MIPD_ zzn12 *x,big e){
    int i,j,nb,n,nbw,nzs;
    zzn12 u,u2,t[32];
	big ONE = mirvar(_MIPP_ 1);
    big ZERO = mirvar(_MIPP_ 0);
    
    
    zzn12_mirvar(_MIPP_ &u);
    zzn12_mirvar(_MIPP_ &u2);
    for (i=0;i<32;i++) zzn12_mirvar(_MIPP_ &(t[i]));
    
    if (zzn12_iszero(_MIPP_ x)) {
        mirkill(ONE);
        mirkill(ZERO);
        zzn12_kill(_MIPP_ &u);
        zzn12_kill(_MIPP_ &u2);
        for (i=0;i<32;i++) zzn12_kill(_MIPP_ &(t[i]));
        return;
    }
    if (mr_compare(e, ZERO)==0){
        zzn12_from_int(_MIPP_ 1,x);
        mirkill(ONE);
        mirkill(ZERO);
        zzn12_kill(_MIPP_ &u);
        zzn12_kill(_MIPP_ &u2);
        for (i=0;i<32;i++) zzn12_kill(_MIPP_ &(t[i]));
        return ;
    }
    if (mr_compare(e, ONE)==0){
        mirkill(ONE);
        mirkill(ZERO);
        zzn12_kill(_MIPP_ &u);
        zzn12_kill(_MIPP_ &u2);
        for (i=0;i<32;i++) zzn12_kill(_MIPP_ &(t[i]));
        return;
    }
    
 //   zzn12_copy(x, &u);
 //	for (i=1;i<1000;i++) zzn12_mul(_MIPP_ x, x, &u);

	zzn12_copy(x, &u);
    zzn12_mul(_MIPP_ &u, &u, &u2);
    zzn12_copy(&u, &(t[0]));
    
    for (i=1;i<32;i++) zzn12_mul(_MIPP_ &u2, &(t[i-1]), &(t[i]));
    
    nb = logb2(_MIPP_ e);
    if (nb>1) for (i=nb-2;i>=0;)
    {
        n = mr_window(_MIPP_ e, i, &nbw, &nzs, 6);
        for (j=0;j<nbw;j++) zzn12_mul(_MIPP_ &u, &u, &u);
        if (n>0)  zzn12_mul(_MIPP_ &u, &(t[n/2]), &u);
        i-=nbw;
        if (nzs)
        {
            for (j=0;j<nzs;j++) zzn12_mul(_MIPP_ &u, &u, &u);
            i-=nzs;
        }
    }
    
    zzn12_copy(&u, x);
    
    mirkill(ONE);
    mirkill(ZERO);
    zzn12_kill(_MIPP_ &u);
    zzn12_kill(_MIPP_ &u2);
    for (i=0;i<32;i++) zzn12_kill(_MIPP_ &(t[i]));
}


void zzn12_add(_MIPD_ zzn12 *w,zzn12 *x,zzn12 *y){
    zzn4_add(_MIPP_ &(w->a), &(x->a), &(y->a));
    zzn4_add(_MIPP_ &(w->b), &(x->b), &(y->b));
    zzn4_add(_MIPP_ &(w->c), &(x->c), &(y->c));
    y->miller = w->miller;
    y->unitary = FALSE;
}
void zzn12_sub(_MIPD_ zzn12 *w,zzn12 *x,zzn12 *y){
    zzn4_sub(_MIPP_ &(w->a), &(x->a), &(y->a));
    zzn4_sub(_MIPP_ &(w->b), &(x->b), &(y->b));
    zzn4_sub(_MIPP_ &(w->c), &(x->c), &(y->c));
    y->miller = w->miller;
    y->unitary = FALSE;
}
void zzn12_smul(_MIPD_ zzn12 *w,zzn4 *x,zzn12 *y){
    zzn4_mul(_MIPP_ &(w->a), x, &(y->a));
    zzn4_mul(_MIPP_ &(w->b), x, &(y->b));
    zzn4_mul(_MIPP_ &(w->c), x, &(y->c));
    y->miller = w->miller;
    y->unitary = FALSE;
}

void zzn12_mul(_MIPD_ zzn12 *w,zzn12 *x,zzn12 *y){
    
    
    zzn4 tx;
	zzn12_copy(w, y);
    zzn4_mirvar(_MIPP_ &tx);
    if (zzn12_compare(w, x)){
        zzn4 A,B,C,D;
        
        zzn4_mirvar(_MIPP_ &A);
        zzn4_mirvar(_MIPP_ &B);
        zzn4_mirvar(_MIPP_ &C);
        zzn4_mirvar(_MIPP_ &D);
        
        if (w->unitary){
            // Granger & Scott PKC 2010 - only 3 squarings!
            //A=a; a*=a; D=a; a+=a; a+=D; A.conj(); A+=A; a-=A;
            zzn4_copy(&(y->a), &A);
            zzn4_mul(_MIPP_ &(y->a), &(y->a), &(y->a));
            zzn4_copy(&(y->a), &D);
            zzn4_add(_MIPP_ &(y->a), &(y->a), &(y->a));
            zzn4_add(_MIPP_ &(y->a), &D, &(y->a));
            zzn4_conj(_MIPP_ &A, &A);
            zzn4_add(_MIPP_ &A, &A, &A);
            zzn4_sub(_MIPP_ &(y->a), &A, &(y->a));
            
            //B=c; B*=B; B=tx(B); D=B; B+=B; B+=D;
            zzn4_copy(&(y->c), &B);
            zzn4_mul(_MIPP_ &B, &B, &B);
            zzn4_tx(_MIPP_ &B);
            zzn4_copy(&B, &D);
            zzn4_add(_MIPP_ &B, &B, &B);
            zzn4_add(_MIPP_ &B, &D, &B);
            
            //C=b; C*=C;          D=C; C+=C; C+=D;
            zzn4_copy(&(y->b), &C);
            zzn4_mul(_MIPP_ &C, &C, &C);
            zzn4_copy(&C, &D);
            zzn4_add(_MIPP_ &C, &C, &C);
            zzn4_add(_MIPP_ &C, &D, &C);
            
            //b.conj(); b+=b; c.conj(); c+=c; c=-c;
            zzn4_conj(_MIPP_ &(y->b), &(y->b));
            zzn4_add(_MIPP_ &(y->b), &(y->b), &(y->b));
            zzn4_conj(_MIPP_ &(y->c), &(y->c));
            zzn4_add(_MIPP_ &(y->c), &(y->c), &(y->c));
            zzn4_negate(_MIPP_ &(y->c), &(y->c));
            //b+=B; c+=C;
            zzn4_add(_MIPP_ &(y->b), &B, &(y->b));
            zzn4_add(_MIPP_ &(y->c), &C, &(y->c));
        }else{
            if (!(w->miller)){
                // Chung-Hasan SQR2
                //A=a; A*=A;
                zzn4_copy(&(y->a), &A);
                zzn4_mul(_MIPP_ &A, &A, &A);
                //B=b*c; B+=B;
                zzn4_mul(_MIPP_ &(y->b), &(y->c), &B);
                zzn4_add(_MIPP_ &B, &B, &B);
                //C=c; C*=C;
                zzn4_copy(&(y->c), &C);
                zzn4_mul(_MIPP_ &C, &C, &C);
                //D=a*b; D+=D;
                zzn4_mul(_MIPP_ &(y->a), &(y->b), &D);
                zzn4_add(_MIPP_ &D, &D, &D);
                //c+=(a+b); c*=c;
                zzn4_add(_MIPP_ &(y->a), &(y->b), &(y->c));
                zzn4_add(_MIPP_ &(y->c), &(y->c), &(y->c));
                zzn4_mul(_MIPP_ &(y->c), &(y->c), &(y->c));
                //a=A+tx(B);
                zzn4_copy(&B, &tx);
                zzn4_tx(_MIPP_ &tx);
                zzn4_add(_MIPP_ &A, &tx, &(y->a));
                //b=D+tx(C);
                zzn4_copy(&C, &tx);
                zzn4_tx(_MIPP_ &tx);
                zzn4_add(_MIPP_ &D, &tx, &(y->b));
                //c-=(A+B+C+D);
                zzn4_sub(_MIPP_ &(y->c), &A, &(y->c));
                zzn4_sub(_MIPP_ &(y->c), &B, &(y->c));
                zzn4_sub(_MIPP_ &(y->c), &C, &(y->c));
                zzn4_sub(_MIPP_ &(y->c), &D, &(y->c));
            }else{
                // Chung-Hasan SQR3 - actually calculate 2x^2 !
                // Slightly dangerous - but works as will be raised to p^{k/2}-1
                // which wipes out the 2.
                
     //           A=a; A*=A;       // a0^2    = S0
                zzn4_copy(&(y->a), &A);
                zzn4_mul(_MIPP_ &A, &A, &A);
     //           C=c; C*=b; C+=C; // 2a1.a2  = S3
                zzn4_copy(&(y->c), &C);
                zzn4_mul(_MIPP_ &C, &(y->b), &C);
                zzn4_add(_MIPP_ &C, &C, &C);
     //           D=c; D*=D;       // a2^2    = S4
                zzn4_copy(&(y->c), &D);
                zzn4_mul(_MIPP_ &D, &D, &D);
     //          c+=a;            // a0+a2
                zzn4_add(_MIPP_ &(y->c), &(y->a), &(y->c));
     //           B=b; B+=c; B*=B; // (a0+a1+a2)^2  =S1
                zzn4_copy(&(y->b), &B);
                zzn4_add(_MIPP_ &B, &(y->c), &B);
                zzn4_mul(_MIPP_ &B, &B, &B);
     //           c-=b; c*=c;      // (a0-a1+a2)^2  =S2
                zzn4_sub(_MIPP_ &(y->c), &(y->b), &(y->c));
                zzn4_mul(_MIPP_ &(y->c), &(y->c), &(y->c));
     //           C+=C; A+=A; D+=D;
                zzn4_add(_MIPP_ &C, &C, &C);
                zzn4_add(_MIPP_ &A, &A, &A);
                zzn4_add(_MIPP_ &D, &D, &D);
     //           a=A+tx(C);
                zzn4_copy(&C, &tx);
                zzn4_tx(_MIPP_ &tx);
                zzn4_add(_MIPP_ &A, &tx, &(y->a));
                
     //           b=B-c-C+tx(D);
                zzn4_copy(&D, &tx);
                zzn4_tx(_MIPP_ &tx);
                zzn4_sub(_MIPP_ &B, &(y->c), &(y->b));
                zzn4_sub(_MIPP_ &(y->b), &C, &(y->b));
                zzn4_add(_MIPP_ &(y->b), &tx, &(y->b));
     //           c+=B-A-D;  // is this code telling me something...?
                zzn4_add(_MIPP_ &(y->c), &B, &(y->c));
                zzn4_sub(_MIPP_ &(y->c), &A, &(y->c));
                zzn4_sub(_MIPP_ &(y->c), &D, &(y->c));
            }
        
        }
        zzn4_kill(_MIPP_ &A);
        zzn4_kill(_MIPP_ &B);
        zzn4_kill(_MIPP_ &C);
        zzn4_kill(_MIPP_ &D);
    }else{
        // Karatsuba
        zzn4 Z0,Z1,Z2,Z3,T0,T1;
        //BOOL zero_c,zero_b;
        BOOL zero_c,zero_b;
        
        zzn4_mirvar(_MIPP_ &Z0);
        zzn4_mirvar(_MIPP_ &Z1);
        zzn4_mirvar(_MIPP_ &Z2);
        zzn4_mirvar(_MIPP_ &Z3);
        zzn4_mirvar(_MIPP_ &T0);
        zzn4_mirvar(_MIPP_ &T1);
        //zero_c=(x.c).iszero();
        //zero_b=(x.b).iszero();
        zero_c = zzn4_iszero(&(x->c));
        zero_b = zzn4_iszero(&(x->b));
        //Z0=a*x.a;  //9
        zzn4_mul(_MIPP_ &(y->a), &(x->a), &Z0);
        //if (!zero_b) Z2=b*x.b;  //+6
        if (!zero_b) {
            zzn4_mul(_MIPP_ &(y->b), &(x->b), &Z2);
        }
        
        
        //T0=a+b;
        //T1=x.a+x.b;
        //Z1=T0*T1;  //+9
        //Z1-=Z0;
        //if (!zero_b) Z1-=Z2;
        //T0=b+c;
        //T1=x.b+x.c;
        //Z3=T0*T1;  //+6
        
        zzn4_add(_MIPP_ &(y->a), &(y->b), &T0);
        zzn4_add(_MIPP_ &(x->a), &(x->b), &T1);
        zzn4_mul(_MIPP_ &T0, &T1, &Z1);
        zzn4_sub(_MIPP_ &Z1, &Z0, &Z1);
        if (!zero_b) {
            zzn4_sub(_MIPP_ &Z1, &Z2, &Z1);
        }
        zzn4_add(_MIPP_ &(y->b), &(y->c), &T0);
        zzn4_add(_MIPP_ &(x->b), &(x->c), &T1);
        zzn4_mul(_MIPP_ &T0, &T1, &Z3);
        
        //if (!zero_b) Z3-=Z2;
        if (!zero_b){
            zzn4_sub(_MIPP_ &Z3, &Z2, &Z3);
        }
        
        //T0=a+c;
        zzn4_add(_MIPP_ &(y->a), &(y->c), &T0);
        //T1=x.a+x.c;
        //T0*=T1;   //+9=39 for "special case"
        //if (!zero_b) Z2+=T0;
        //else         Z2=T0;
        //Z2-=Z0;
        zzn4_add(_MIPP_ &(x->a), &(x->c), &T1);
        zzn4_mul(_MIPP_ &T0, &T1, &T0);
        if (!zero_b){
            zzn4_add(_MIPP_ &Z2, &T0, &Z2);
        }else{
            zzn4_copy(&T0, &Z2);
        }
        zzn4_sub(_MIPP_ &Z2, &Z0, &Z2);
        
        
       // b=Z1;
       // if (!zero_c)
       // { // exploit special form of BN curve line function
       //     T0=c*x.c;
       //     Z2-=T0;
       //     Z3-=T0;
       //     b+=tx(T0);
       // }
        zzn4_copy(&Z1, &(y->b));
        if (!zero_c){
            zzn4_mul(_MIPP_ &(y->c), &(x->c), &T0);
            zzn4_sub(_MIPP_ &Z2, &T0, &Z2);
            zzn4_sub(_MIPP_ &Z3, &T0, &Z3);
            zzn4_tx(_MIPP_ &T0);
            zzn4_add(_MIPP_ &(y->b), &T0, &(y->b));
        }
        
        //a=Z0+tx(Z3);
        //c=Z2;
        zzn4_tx(_MIPP_ &Z3);
        zzn4_add(_MIPP_ &Z0, &Z3, &(y->a));
        zzn4_copy(&Z2, &(y->c));
        //if (!x.unitary) unitary=FALSE;
        if(!(x->unitary)) y->unitary = FALSE;
        
        zzn4_kill(_MIPP_ &Z0);
        zzn4_kill(_MIPP_ &Z1);
        zzn4_kill(_MIPP_ &Z2);
        zzn4_kill(_MIPP_ &Z3);
        zzn4_kill(_MIPP_ &T0);
        zzn4_kill(_MIPP_ &T1);
    }
    zzn4_kill(_MIPP_ &tx);
}





void zzn12_inv(_MIPD_ zzn12 *w){
    zzn12 y;
    zzn4 f0,f1;
	zzn12_mirvar(_MIPP_ &y);
    //if (w.unitary)
    //{
    //    y=conj(w);
    //    return y;
    //}

    if( w->unitary){
        zzn12_conj(_MIPP_ w, &y);
        zzn12_copy(&y, w);
        zzn12_kill(_MIPP_ &y);
        return;
    }
    
    
    zzn4_mirvar(_MIPP_ &f0);
    zzn4_mirvar(_MIPP_ &f1);
    //y.a=w.a*w.a-tx(w.b*w.c);
    zzn4_mul(_MIPP_ &(w->a), &(w->a), &(y.a));
    zzn4_mul(_MIPP_ &(w->b), &(w->c), &f0);
    zzn4_tx(_MIPP_ &f0);
    zzn4_sub(_MIPP_ &(y.a), &f0, &(y.a));
    //y.b=tx(w.c*w.c)-w.a*w.b;
    zzn4_mul(_MIPP_ &(w->c), &(w->c), &(y.b));
    zzn4_tx(_MIPP_ &(y.b));
    zzn4_mul(_MIPP_ &(w->a), &(w->b), &f0);
    zzn4_sub(_MIPP_ &(y.b), &f0, &(y.b));
    //y.c=w.b*w.b-w.a*w.c;
    zzn4_mul(_MIPP_ &(w->b), &(w->b), &(y.c));
    zzn4_mul(_MIPP_ &(w->a), &(w->c), &f0);
    zzn4_sub(_MIPP_ &(y.c), &f0, &(y.c));
    
    //f0=tx(w.b*y.c)+w.a*y.a+tx(w.c*y.b);
    zzn4_mul(_MIPP_ &(w->b), &(y.c), &f0);
    zzn4_tx(_MIPP_ &f0);
    zzn4_mul(_MIPP_ &(w->a), &(y.a), &f1);
    zzn4_add(_MIPP_ &f0, &f1, &f0);
    zzn4_mul(_MIPP_ &(w->c), &(y.b), &f1);
    zzn4_tx(_MIPP_ &f1);
    zzn4_add(_MIPP_ &f0, &f1, &f0);
    //f0=inverse(f0);
    zzn4_inv(_MIPP_ &f0);
    //y.c*=f0;
    //y.b*=f0;
    //y.a*=f0;
    zzn4_mul(_MIPP_ &(y.a), &f0, &(y.a));
    zzn4_mul(_MIPP_ &(y.b), &f0, &(y.b));
    zzn4_mul(_MIPP_ &(y.c), &f0, &(y.c));
    
    zzn12_copy(&y, w);
    zzn4_kill(_MIPP_ &f0);
    zzn4_kill(_MIPP_ &f1);
    zzn12_kill(_MIPP_ &y);
}

void zzn12_from_zzn4s(zzn4 * a,zzn4 *b,zzn4 *c,zzn12 *w){
    zzn4_copy(a, &(w->a));
    zzn4_copy(b, &(w->b));
    zzn4_copy(c, &(w->c));
    w->miller = FALSE;
    w->unitary = FALSE;
}

void zzn12_conj(_MIPD_ zzn12 *w,zzn12 *x){
    zzn4_conj(_MIPP_ &(w->a), &(x->a));
    zzn4_conj(_MIPP_ &(w->b), &(x->b));
    zzn4_negate(_MIPP_ &(x->b), &(x->b));
    zzn4_conj(_MIPP_ &(w->c), &(x->c));
    x->unitary = w->unitary;
    x->miller = w->miller;
}

BOOL zzn12_compare(zzn12 *w,zzn12 *x){
    if (zzn4_compare(&(w->a), &(x->a)) &&
        zzn4_compare(&(w->b), &(x->b)) &&
        zzn4_compare(&(w->c), &(x->c)) ){
        return TRUE;
    }
    return FALSE;
}

void zzn12_tochar(_MIPD_ zzn12* w,unsigned char *c,unsigned int bInt){
    int hlen = 0;
    big m = mirvar(_MIPP_ 0);
    redc(_MIPP_ w->c.b.b, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->c.b.a, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->c.a.b, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->c.a.a, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->b.b.b, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->b.b.a, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->b.a.b, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->b.a.a, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->a.b.b, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->a.b.a, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->a.a.b, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    
    redc(_MIPP_ w->a.a.a, m);
    big_to_bytes(_MIPP_ bInt, m, (char *)c+hlen, TRUE);
    hlen += bInt;
    mirkill(m);

}

void zzn12_fromchar(_MIPD_ zzn12* w,unsigned char *c,unsigned int bInt){
    int hlen = 0;
    big m = mirvar(_MIPP_ 0);
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->c.b.b);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->c.b.a);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->c.a.b);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->c.a.a);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->b.b.b);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->b.b.a);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->b.a.b);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->b.a.a);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->a.b.b);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->a.b.a);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->a.a.b);
    hlen += bInt;
    
    bytes_to_big(_MIPP_ bInt, (char *)c+hlen, m);
    nres(_MIPP_ m, w->a.a.a);
    hlen += bInt;
    
    w->miller = FALSE;
    w->unitary = TRUE;
    mirkill(m);
}
