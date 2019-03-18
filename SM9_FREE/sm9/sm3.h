#ifndef _SM3_H_
#define _SM3_H_ 

#define SM3_HASH_LEN  32

typedef struct {
	unsigned int state[8];
	unsigned long long tc;
	unsigned int bc;
	unsigned char buffer[64];
} SM3_CTX;   //SM3算法的基本结构体


//SM3算法三件套，首先调用SM3Init，然后调用SM3Update（可重复调用）逐步将消息读入，最后调用SM3Final得到SM3摘要值digest

void SM3Init(SM3_CTX* context);
void SM3Update(SM3_CTX* context, unsigned char* data, unsigned int len);
void SM3Final(unsigned char digest[32], SM3_CTX* context);


//SM3算法一步到位，输入消息和长度，得到摘要digest
void SM3(unsigned char* data, unsigned int len, unsigned char digest[32]);

#endif

