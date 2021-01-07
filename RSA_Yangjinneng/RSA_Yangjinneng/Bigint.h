#pragma once
#include<iostream>
using namespace std;

#define SIZE 65
#define MULSIZE 2*SIZE


typedef struct Bigint
{

	unsigned char num[SIZE];
}Bigint;

typedef struct Bigint2//ʵ�ֳ˷���Ҫ
{
	unsigned char num[2 * SIZE];
}Bigint2;

Bigint p = { 0 };
Bigint q = { 0 };
Bigint N = { 0 };
Bigint PhiN = { 0 };//��(N)
Bigint e = { 0 };
Bigint d = { 0 };
Bigint m = { 0 };//����
Bigint c = { 0 };//����
Bigint m1 = { 0 };//������

void Copy(Bigint &a, Bigint b);//����
void Print(Bigint a);//��a��ӡ��ʮ�������
void PrintNum(unsigned char *out, int &len, Bigint a);//��ӡ�������������
int Length(Bigint a);//�����ֽڳ���
int Length(Bigint2 a);//
int Compare(Bigint a, Bigint b);//�Ƚϴ�С��a>b,a=b,a<b�ֱ����1,0��-1
int Compare(Bigint2 a, Bigint2 b);
Bigint ByteMoveLeft(Bigint a, int loop);//����loop���ֽ�(ÿ����һλ����256)
Bigint2 ByteMoveLeft(Bigint2 a, int loop);
void BitMoveRight(Bigint &a);//����һ������(ÿ����һλ����2)
Bigint2 Extend(Bigint a);//��������,���䲿����0
Bigint Narrow(Bigint2 a);//�ض�����
//���������
Bigint BigRand(Bigint n);  //����[0,n���ڵ������
Bigint BigRandOdd(int bytes); //�����ֽ���Ϊbytes���������
bool MillerRabinKnl(Bigint &n); //����MillerRabin���Լ��
bool MillerRabin(Bigint &n, long loop);  //���MillerRabin���Լ��
Bigint GenPrime(int bytes);  //�����ֽ���Ϊbytes�Ĵ�����

//��������
Bigint Add(Bigint a, Bigint b);//�ӷ�
Bigint Sub(Bigint a, Bigint b);//����
Bigint2 Sub(Bigint2 a, Bigint2 b);
Bigint2 Mul(Bigint a, Bigint b);//�˷�
Bigint Div(Bigint a, Bigint b);//����
Bigint Mod(Bigint a, Bigint b);//����
Bigint2 Mod(Bigint2 a, Bigint2 b);
Bigint AddMod(Bigint a, Bigint b, Bigint n);//ģ�ӣ�����a+bmodn
Bigint SubMod(Bigint a, Bigint b, Bigint n);//ģ��������a-bmodn,a>=b
Bigint Sub2Mod(Bigint a, Bigint b, Bigint n);//ģ��������a-bmodn
Bigint MulMod(Bigint a, Bigint b, Bigint n);//ģ�ˣ�����a*bmodn
Bigint PowMod(Bigint a, Bigint b, Bigint n);//ģ�ݣ�����a^bmodn
Bigint GCD(Bigint a, Bigint b);//���������

Bigint GenE(Bigint PhiN);//���ɹ�˽Կ
bool Inverse(Bigint e, Bigint N, Bigint &res);
//����
Bigint Encrypt(Bigint m, Bigint e, Bigint n);
//����
Bigint Decrypt(Bigint c, Bigint d, Bigint n);


void Copy(Bigint &a, Bigint b) {
	for (int i = 0; i < SIZE; i++)
		a.num[i] = b.num[i];
}

void Print(Bigint a)  //��a��ӡ��ʮ�������
{
	int res[2000];
	int i = 0;
	Bigint b = { 0 };
	Bigint c = { 10 };
	while (Compare(a, b) == 1) {
		res[i] = Mod(a, c).num[0];
		a = Div(a, c);
		i++;
	}

}

void PrintNum(unsigned char *out, int &len, Bigint a)  //��ӡ�������������
{
	int i = 0;

	Bigint b = { 0 };
	Bigint c = { 10 };
	while (Compare(a, b) == 1)
	{
		out[i] = Mod(a, c).num[0];
		a = Div(a, c);
		i++;
	}
	len = i;
	unsigned char temp = 0;
	for (int k = 0; k < len / 2; k++)
	{
		temp = out[k];
		out[k] = out[len - 1 - k];
		out[len - 1 - k] = temp;
	}
	for (i = 0; i < len; i++)
	{
		out[i] = out[i] + '0';
	}
}


int Length(Bigint a) {//Ĭ��lengthΪsize
	int i = SIZE - 1;
	while (a.num[i] == 0 && i >= 0) {//�Ӹ������ң�ֱ���ֽڲ�Ϊ0
		i--;
	}
	return i + 1;
}

int Length(Bigint2 a) {//Ĭ��lengthΪsize
	int i = 2 * SIZE - 1;
	while (a.num[i] == 0 && i >= 0) {//�Ӹ������ң�ֱ���ֽڲ�Ϊ0
		i--;
	}
	return i + 1;
}

int Compare(Bigint a, Bigint b) {
	int a_len = Length(a);
	int b_len = Length(b);
	int max;
	if (a_len > b_len)
		max = a_len;
	else
		max = b_len;
	if (max == 0)
		return 0;
	else
	{
		for (int i = max - 1; i >= 0; i--)
		{
			if (a.num[i] > b.num[i])
				return 1;
			if (a.num[i] < b.num[i])
				return -1;
		}
	}
	return 0;
}

int Compare(Bigint2 a, Bigint2 b) {
	int a_len = Length(a);
	int b_len = Length(b);
	int max;
	if (a_len > b_len)
		max = a_len;
	else
		max = b_len;
	if (max == 0)
		return 0;
	else
	{
		for (int i = max - 1; i >= 0; i--)
		{
			if (a.num[i] > b.num[i])
				return 1;
			if (a.num[i] < b.num[i])
				return -1;
		}
	}
	return 0;
}

Bigint ByteMoveLeft(Bigint a, int loop) {
	for (int i = Length(a) - 1; i >= 0; i--)
	{
		if (i + loop >= SIZE)//���������󳤶�
			continue;
		a.num[i + loop] = a.num[i];
	}
	for (int i = loop - 1; i >= 0; i--)//��λ��0
		a.num[i] = 0;
	return a;
}

Bigint2 ByteMoveLeft(Bigint2 a, int loop) {
	for (int i = Length(a) - 1; i >= 0; i--)
	{
		if (i + loop >= 2 * SIZE)
			continue;
		a.num[i + loop] = a.num[i];
	}
	for (int i = loop - 1; i >= 0; i--)
		a.num[i] = 0;
	return a;
}

void BitMoveRight(Bigint &a) {
	Bigint  two = { 2 };
	a = Div(a, two);
}

Bigint2 Extend(Bigint a) {
	Bigint2 res = { 0 };
	for (int i = 0; i < SIZE; i++)
		res.num[i] = a.num[i];   //��ĵط���0
	return res;
}

Bigint Narrow(Bigint2 a) {
	Bigint res = { 0 };
	for (int i = 0; i < SIZE; i++)
		res.num[i] = a.num[i];
	return res;
}

Bigint Add(Bigint a, Bigint b) { //�ӷ�
	Bigint c;//������
	unsigned short temp;//16λ��ʱ��
	unsigned char carry = 0;//��λ
	for (int i = 0; i < SIZE; i++)
	{
		temp = a.num[i] + b.num[i] + carry;
		c.num[i] = temp & 0x00ff;//�ѵͰ�λȡ������ÿ�ֽڼӷ����
		carry = (temp >> 8) & 0xff;//����8λ��û�н�λ
	}
	return c;
}

Bigint Sub(Bigint a, Bigint b) { //����
	if (Compare(a, b) == -1) {  //a<b
		cout << "subtract error";
		return a;
	}
	Bigint c;
	short temp;
	unsigned char carry = 0;//��λ
	for (int i = 0; i < SIZE; i++) {
		temp = a.num[i] - b.num[i] - carry;
		c.num[i] = temp & 0x00ff;//���
		carry = (temp >> 15) & 0x01;//���޽�λ
	}
	return c;
}

Bigint2 Sub(Bigint2 a, Bigint2 b) { //����
	if (Compare(a, b) == -1) {
		cout << "subtract error";
		return a;
	}
	Bigint2 c;
	short temp;
	unsigned char carry = 0;//��λ
	for (int i = 0; i < 2 * SIZE; i++) {
		temp = a.num[i] - b.num[i] - carry;
		c.num[i] = temp & 0x00ff;//ÿ�ֽڼ������
		carry = (temp >> 15) & 0x01;//���޽�λ
	}
	return c;
}

Bigint2 Mul(Bigint a, Bigint b)//�˷�
{
	Bigint2 c = { 0 };
	unsigned short temp;
	unsigned char carry;
	for (int i = 0; i < SIZE; i++)
	{
		carry = 0;
		for (int j = 0; j < SIZE; j++)
		{
			temp = a.num[i] * b.num[j] + c.num[i + j] + carry;//a.num[i]*b.num[i]���ӵ�c.num[i+j]�ϲ���¼���
			c.num[i + j] = temp & 0x000ff;//ȡ�Ͱ�λ��¼���
			carry = (temp >> 8) & 0xff;
		}
	}
	c.num[2 * SIZE - 1] = carry;
	return c;
}

Bigint Div(Bigint a, Bigint b) //����
{
	Bigint B = { 0 };
	Bigint c = { 0 };
	int len = Length(a) - Length(b);//�̵����鳤��
	while (len >= 0) {
		B = ByteMoveLeft(b, len);//����b����len���ֽڣ���Ϊ��ʱ����B
		while (Compare(a, B) >= 0)
		{
			a = Sub(a, B);//��a>=Bʱ�����ϼ�ȥB
			c.num[len]++;//�̲�������
		}
		len--;
	}
	return c;
}

Bigint Mod(Bigint a, Bigint b)  //��ģ
{
	if (Compare(a, b) < 0)
		return a;    //a<b,amodb=a
	else
	{
		Bigint B = { 0 };
		int len = Length(a) - Length(b);//len�����Գ�
		while (len >= 0)
		{
			B = ByteMoveLeft(b, len); //����b����len���ֽڣ���Ϊ��ʱ����B,B��b�ı���;ϣ��������B��a�ӽ�			
			while (Compare(a, B) >= 0)
				a = Sub(a, B);
			len--;
		}
		return a;//��a<bʱ�õ����	
	}
}

Bigint2 Mod(Bigint2 a, Bigint2 b)  //��ģ
{
	if (Compare(a, b) < 0)
		return a;
	else
	{
		Bigint2 B = { 0 };
		int len = Length(a) - Length(b);//len�����Գ�
		while (len >= 0)
		{
			B = ByteMoveLeft(b, len); //����b����len���ֽڣ���Ϊ��ʱ����B,B��b�ı���;ϣ��������B��a�ӽ�			
			while (Compare(a, B) >= 0)
				a = Sub(a, B);
			len--;
		}
		return a;//��a<bʱ�õ����	
	}
}

Bigint AddMod(Bigint a, Bigint b, Bigint n) //ģ�ӣ�����a+bmodn
{
	Bigint res;
	res = Add(a, b);
	return Mod(res, n);
}

Bigint SubMod(Bigint a, Bigint b, Bigint n)  //ģ�ӣ�����a-bmodn,a>=b
{
	Bigint res;
	res = Sub(a, b);
	return Mod(res, n);
}

Bigint Sub2Mod(Bigint a, Bigint b, Bigint n)  //ģ��������a-bmodn
{
	while (Compare(a, b) < 0)
		a = Add(a, n);
	return Sub(a, b);
}

Bigint MulMod(Bigint a, Bigint b, Bigint n)  //ģ�ˣ�����a*bmodn
{
	Bigint2 res;
	res = Mul(a, b);
	return Narrow(Mod(res, Extend(n)));
}

Bigint PowMod(Bigint a, Bigint b, Bigint n)  //ģ�ݣ�����a^bmodn
{
	Bigint c = { 1 };
	Bigint temp = { 1 };
	while (Length(b) > 0)
	{
		//��bΪż��ʱ��a^b mod c = ((a^2)^b/2) mod c;��bΪ����ʱ��ab mod c = ((a^2)^b/2�� a) mod c
		while (!(b.num[0] & 1))//b��ż��
		{
			BitMoveRight(b);//b����2
			a = MulMod(a, a, n);//a=a^2modn
		}                    //��ʵ����ÿ�ν�b��һ�룬ֱ��b=0
		b = Sub(b, temp);
		c = MulMod(a, c, n);
	}
	return c;
}

Bigint GCD(Bigint a, Bigint b)  //���������
{
	Bigint c = { 0 };
	while (Length(a) > 0)
	{
		Copy(c, Mod(b, a));
		Copy(b, a);
		Copy(a, c);  //շת�����
	}
	return b;
}

Bigint BigRand(Bigint n)  //����[0,n���ڵ������
{
	Bigint res = { 0 };
	for (int i = 0; i < SIZE; i++)
	{
		res.num[i] = rand() % 256;  //ÿ���ֽڶ�ȡ��[0,255]�������
	}
	res = Mod(res, n);  //������ģ����ʹ���䵽[0,n)��
	return res;
}

Bigint BigRandOdd(int bytes) //�����ֽ���Ϊbytes���������
{
	Bigint res = { 0 };
	for (int i = 0; i < bytes - 1; i++)
	{
		res.num[i] = rand() % 256;
	}
	res.num[bytes - 1] = 128 + rand() % 128;  //���λȡ1

	if (!(res.num[0] & 0x01))
		res.num[0] = res.num[0] + 1;//��Ϊż�������1Ϊ����

	return res;
}

bool MillerRabinKnl(Bigint &n) //����MillerRabin���Լ��
{
	Bigint b, m, v, temp;
	Bigint j = { 0 };
	Bigint one = { 1 };
	Bigint two = { 2 };
	Bigint three = { 3 };
	m = Sub(n, one);

	//����m,j,ʹ��n-1=2^j*m(mΪ����)
	while (!(m.num[0] & 0x01))  //��m��Ϊ���� 
	{
		j = Add(j, one);	//j=j+1;
		BitMoveRight(m);  //�൱�ڳ�2
	}
	b = Add(two, BigRand(Sub(n, three)));//���ѡȡb��[2,N-1]
	v = PowMod(b, m, n);   //����v=b^mmodn
	if (Compare(v, one) == 0)  //��v=1,ͨ������
		return 1;
	//��v!=1
	Bigint i = { 1 };
	temp = Sub(n, one);
	while (Compare(v, temp) < 0)  //��v<n-1,��ͨ��
	{
		if (Compare(i, j) == 0)  //��i=j���Ǻ�������ͨ��
			return 0;
		v = MulMod(v, v, n);  //v=v^2modn,i=i+1
		i = Add(i, one);
	}
	return 1;  //��v=n-1,ͨ�����
}

bool MillerRabin(Bigint &n, long loop)  //���MillerRabin���Լ��
{
	for (int i = 0; i < loop; i++)  //�������Լ��loop��
	{
		if (!MillerRabinKnl(n))
			return 0;
	}
	return 1;
}

Bigint GenPrime(int bytes)  //�����ֽ���Ϊbytes�Ĵ�����
{
	Bigint res = BigRandOdd(bytes);
	int loop = 20;
	while (!MillerRabin(res, loop)) //�����μ�ⲻͨ��������ȡһ������
	{
		res = BigRandOdd(bytes);
	}
	return res;
}

Bigint GenE(Bigint PhiN)  //���ɹ�Կe�����(N)����
{
	Bigint e = BigRand(PhiN); //�������[1,N]�������e
	Bigint g = GCD(PhiN, e);  //g��e���N���������
	while (Length(g) != 1 || g.num[0] != 1)//������������Ϊ1�������e���������ѭ��
	{
		e = BigRand(PhiN);
		g = GCD(PhiN, e);
	}
	return e;
}

bool Inverse(Bigint e, Bigint N, Bigint &d) //������ģ�棬��eģN���棬�������d
{
	Bigint r1 = { 0 };
	Bigint r2 = { 0 };
	Copy(r1, e);
	Copy(r2, N);      //���ʼֵr1=e,r2=N
	Bigint s1 = { 1 }; //��ϵ����ʼֵs1=1,s2=0
	Bigint s2 = { 0 };
	Bigint s = { 0 };
	Bigint r = { 0 };

	while (1)
	{
		if (Length(r1) == 0)  //��r1=0����ģ��ʧ��
			return 0;
		if (Length(r1) == 1 && r1.num[0] == 1)
		{
			Copy(d, s1);     //��r1=1,��ģ��ɹ������������d
			return 1;
		}
		q = Div(r1, r2);     //��q=r1/r2
		s = Sub2Mod(s1, MulMod(q, s2, N), N);//s=s1-q*s2,Ϊ�˽���Ǹ���ʹ��ģN����
		r = Sub(r1, Narrow(Mul(q, r2)));  //r=r1-q*r2
		Copy(r1, r2);
		Copy(s1, s2);
		Copy(s2, s);
		Copy(r2, r);
	}
}

Bigint Encrypt(Bigint m, Bigint e, Bigint n)
{
	return PowMod(m, e, n);
}

Bigint Decrypt(Bigint c, Bigint d, Bigint n)
{
	return PowMod(c, d, n);
}