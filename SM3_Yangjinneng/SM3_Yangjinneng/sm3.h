#pragma once
#ifndef _SM3_H_
#define _SM3_H_

/*
* SM3算法产生的哈希值大小（单位：字节）
*/
#define SM3_HASH_SIZE 32 

/*
* SM3上下文
*/
typedef struct SM3Context
{
	unsigned int Hash[SM3_HASH_SIZE / 4];
	unsigned char messageBlock[64];//消息块
} SM3Context;

/*
* SM3计算函数
*/
unsigned char *SM3Calc_Yangjinneng(const unsigned char *message,
	unsigned int messageLen, unsigned char digest[SM3_HASH_SIZE]);

#endif // _SM3_H_
