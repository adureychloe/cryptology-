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
#include "stdafx.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <memory.h>
#include "sm3.h"
#include<fstream>
using namespace std;

//判断运行环境是否为小端
static const int endianTest = 1;
#define IsLittleEndian() (*(char *)&endianTest == 1)

//向左循环移位
#define LeftRotate(word, bits) ( (word) << (bits) | (word) >> (32 - (bits)) )

//反转四字节整型字节序
unsigned int *ReverseWord(unsigned int *word)
{
	unsigned char *byte, temp;

	byte = (unsigned char *)word;
	temp = byte[0];
	byte[0] = byte[3];
	byte[3] = temp;

	temp = byte[1];
	byte[1] = byte[2];
	byte[2] = temp;
	return word;
}

//常量T
unsigned int T(int i)
{
	if (i >= 0 && i <= 15)
		return 0x79CC4519;
	else if (i >= 16 && i <= 63)
		return 0x7A879D8A;
	else
		return 0;
}

//FF
unsigned int FF(unsigned int X, unsigned int Y, unsigned int Z, int i)
{
	if (i >= 0 && i <= 15)
		return X ^ Y ^ Z;
	else if (i >= 16 && i <= 63)
		return (X & Y) | (X & Z) | (Y & Z);
	else
		return 0;
}

//GG
unsigned int GG(unsigned int X, unsigned int Y, unsigned int Z, int i)
{
	if (i >= 0 && i <= 15)
		return X ^ Y ^ Z;
	else if (i >= 16 && i <= 63)
		return (X & Y) | (~X & Z);
	else
		return 0;
}

//P0
unsigned int P0(unsigned int X)
{
	return X ^ LeftRotate(X, 9) ^ LeftRotate(X, 17);
}

//P1
unsigned int P1(unsigned int X)
{
	return X ^ LeftRotate(X, 15) ^ LeftRotate(X, 23);
}

//初始化函数
void SM3Init(SM3Context *context)
{
	context->Hash[0] = 0x7380166F;
	context->Hash[1] = 0x4914B2B9;
	context->Hash[2] = 0x172442D7;
	context->Hash[3] = 0xDA8A0600;
	context->Hash[4] = 0xA96F30BC;
	context->Hash[5] = 0x163138AA;
	context->Hash[6] = 0xE38DEE4D;
	context->Hash[7] = 0xB0FB0E4E;
}

//处理消息块
void SM3Process_Yanjinneng(SM3Context *context)
{
	int i;
	unsigned int W[68];
	unsigned int W1[64];
	unsigned int A, B, C, D, E, F, G, H, SS1, SS2, TT1, TT2;

	/* 消息扩展 */
	for (i = 0; i < 16; i++)
	{
		W[i] = *(unsigned int *)(context->messageBlock + i * 4);//将消息分组划分为16个字
		if (IsLittleEndian())
			ReverseWord(W + i);

	}
	for (i = 16; i < 68; i++)
	{
		W[i] = P1(W[i - 16] ^ W[i - 9] ^ LeftRotate(W[i - 3], 15))
			^ LeftRotate(W[i - 13], 7)
			^ W[i - 6];

	}
	for (i = 0; i < 64; i++)
	{
		W1[i] = W[i] ^ W[i + 4];

	}

	/* 消息压缩 */
	A = context->Hash[0];
	B = context->Hash[1];
	C = context->Hash[2];
	D = context->Hash[3];
	E = context->Hash[4];
	F = context->Hash[5];
	G = context->Hash[6];
	H = context->Hash[7];
	for (i = 0; i < 64; i++)
	{
		SS1 = LeftRotate((LeftRotate(A, 12) + E + LeftRotate(T(i), i)), 7);
		SS2 = SS1 ^ LeftRotate(A, 12);
		TT1 = FF(A, B, C, i) + D + SS2 + W1[i];
		TT2 = GG(E, F, G, i) + H + SS1 + W[i];
		D = C;
		C = LeftRotate(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = LeftRotate(F, 19);
		F = E;
		E = P0(TT2);
	}
	context->Hash[0] ^= A;
	context->Hash[1] ^= B;
	context->Hash[2] ^= C;
	context->Hash[3] ^= D;
	context->Hash[4] ^= E;
	context->Hash[5] ^= F;
	context->Hash[6] ^= G;
	context->Hash[7] ^= H;
}

//sm3算法主函数
unsigned char *SM3Calc_Yangjinneng(const unsigned char *message,
	unsigned int messageLen, unsigned char digest[SM3_HASH_SIZE]) {
	SM3Context context;
	unsigned int i, remainder, bitLen;

	//初始化
	SM3Init(&context);

	//对前面的消息分组进行处理
	for (i = 0; i < messageLen / 64; i++)//分组
	{
		memcpy(context.messageBlock, message + i * 64, 64);//把每一个分组内容分别传入消息块中
		SM3Process_Yanjinneng(&context);
	}

	//填充消息分组并处理
	bitLen = messageLen * 8;
	if (IsLittleEndian())
		ReverseWord(&bitLen);
	remainder = messageLen % 64;//余数
	memcpy(context.messageBlock, message + i * 64, remainder);//余数存到下一个消息块
	context.messageBlock[remainder] = 0x80;//后补1
	if (remainder < 56)//如果不足448比特
	{
		memset(context.messageBlock + remainder + 1, 0, 64 - remainder - 1 - 8 + 4);
		memcpy(context.messageBlock + 64 - 4, &bitLen, 4);
		SM3Process_Yanjinneng(&context);
	}
	else
	{
		memset(context.messageBlock + remainder + 1, 0, 64 - remainder - 1);
		SM3Process_Yanjinneng(&context);
		memset(context.messageBlock, 0, 64 - 4);
		memcpy(context.messageBlock + 64 - 4, &bitLen, 4);
		SM3Process_Yanjinneng(&context);
	}

	/* 返回结果 */
	if (IsLittleEndian())
		for (i = 0; i < 8; i++)
			ReverseWord(context.Hash + i);
	memcpy(digest, context.Hash, SM3_HASH_SIZE);

	return digest;
}


