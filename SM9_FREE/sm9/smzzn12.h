//
//  smzzn12.h
//  SM9_FREE
//
//  Created by 宋赓 on 2019/3/15.
//  Copyright © 2019 宋赓. All rights reserved.
//

#ifndef smzzn12_h
#define smzzn12_h
#include "../miracl/miracl.h"
typedef struct
{
    zzn4 a;
    zzn4 b;
    zzn4 c;
    BOOL unitary;
    BOOL miller;
} zzn12;


extern void zzn12_mark_miller(_MIPD_ zzn12* );
extern void zzn12_mark_regular(_MIPD_ zzn12* );
extern void zzn12_mark_unitary(_MIPD_ zzn12* );
extern void zzn12_mirvar(_MIPD_ zzn12*);
extern void zzn12_kill(_MIPD_ zzn12*);
extern BOOL zzn12_iszero(_MIPD_ zzn12 *);
extern BOOL zzn12_isunity(_MIPT_ zzn12 *);
extern void zzn12_from_int(_MIPT_ int,zzn12 *);
extern void zzn12_copy(zzn12 *,zzn12 *);
extern void zzn12_zero(zzn12 *);
extern void zzn12_negate(_MIPT_ zzn12 *,zzn12 *);
extern void zzn12_powq(_MIPT_ zzn12 *,zzn2 *);
extern void zzn12_pow(_MIPT_ zzn12 *,big );
extern void zzn12_add(_MIPT_ zzn12 *,zzn12 *,zzn12 *);
extern void zzn12_sub(_MIPT_ zzn12 *,zzn12 *,zzn12 *);
extern void zzn12_smul(_MIPT_ zzn12 *,zzn4 *,zzn12 *);
extern void zzn12_sqr(_MIPT_ zzn12 *,zzn12 *);
extern void zzn12_mul(_MIPT_ zzn12 *,zzn12 *,zzn12 *);
extern void zzn12_inv(_MIPT_ zzn12 *);
extern void zzn12_timesi(_MIPT_ zzn12 *);
extern void zzn12_tx(_MIPT_ zzn12 *);
extern void zzn12_from_zzn4s(zzn4 *,zzn4 *,zzn4 *,zzn12 *);
extern void zzn12_from_zzn4(zzn4 *,zzn12 *);
extern void zzn12_div2(_MIPT_ zzn12 *);
extern void zzn12_conj(_MIPT_ zzn12 *,zzn12 *);
extern void zzn12_imul(_MIPT_ zzn12 *,int,zzn12 *);
extern void zzn12_lmul(_MIPT_ zzn12 *,big,zzn12 *);
extern BOOL zzn12_compare(zzn12 *,zzn12 *);

void zzn12_tochar(_MIPD_ zzn12* ,unsigned char *,unsigned int bInt);
void zzn12_fromchar(_MIPD_ zzn12* ,unsigned char *,unsigned int bInt);
#endif /* smzzn12_h */
