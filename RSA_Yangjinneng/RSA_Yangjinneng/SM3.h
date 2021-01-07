#pragma once
#ifndef _SM3_H_
#define _SM3_H_

/*
* SM3�㷨�����Ĺ�ϣֵ��С����λ���ֽڣ�
*/
#define SM3_HASH_SIZE 32 

/*
* SM3������
*/
typedef struct SM3Context
{
	unsigned int Hash[SM3_HASH_SIZE / 4];
	unsigned char messageBlock[64];//��Ϣ��
} SM3Context;

/*
* SM3���㺯��
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

//�ж����л����Ƿ�ΪС��
static const int endianTest = 1;
#define IsLittleEndian() (*(char *)&endianTest == 1)

//����ѭ����λ
#define LeftRotate(word, bits) ( (word) << (bits) | (word) >> (32 - (bits)) )

//��ת���ֽ������ֽ���
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

//����T
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

//��ʼ������
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

//������Ϣ��
void SM3Process_Yanjinneng(SM3Context *context)
{
	int i;
	unsigned int W[68];
	unsigned int W1[64];
	unsigned int A, B, C, D, E, F, G, H, SS1, SS2, TT1, TT2;

	/* ��Ϣ��չ */
	for (i = 0; i < 16; i++)
	{
		W[i] = *(unsigned int *)(context->messageBlock + i * 4);//����Ϣ���黮��Ϊ16����
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

	/* ��Ϣѹ�� */
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

//sm3�㷨������
unsigned char *SM3Calc_Yangjinneng(const unsigned char *message,
	unsigned int messageLen, unsigned char digest[SM3_HASH_SIZE]) {
	SM3Context context;
	unsigned int i, remainder, bitLen;

	//��ʼ��
	SM3Init(&context);

	//��ǰ�����Ϣ������д���
	for (i = 0; i < messageLen / 64; i++)//����
	{
		memcpy(context.messageBlock, message + i * 64, 64);//��ÿһ���������ݷֱ�����Ϣ����
		SM3Process_Yanjinneng(&context);
	}

	//�����Ϣ���鲢����
	bitLen = messageLen * 8;
	if (IsLittleEndian())
		ReverseWord(&bitLen);
	remainder = messageLen % 64;//����
	memcpy(context.messageBlock, message + i * 64, remainder);//�����浽��һ����Ϣ��
	context.messageBlock[remainder] = 0x80;//��1
	if (remainder < 56)//�������448����
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

	/* ���ؽ�� */
	if (IsLittleEndian())
		for (i = 0; i < 8; i++)
			ReverseWord(context.Hash + i);
	memcpy(digest, context.Hash, SM3_HASH_SIZE);

	return digest;
}


