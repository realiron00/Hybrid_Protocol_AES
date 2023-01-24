#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef unsigned char byte; //바이트 단위로 가져옴

//subbytes, keyexpansion에 필요한 sbox 생성
byte Sbox[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

//inverse subbytes에 필요한 Invsbox 생성
byte InvSbox[256] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

//keyexpansion에 필요한 rcon 생성
byte rcon[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

//유한체 GF(2^8)에서의 곱셈을 위한 xbox함수
//x^8 mod m(x) = x^4 + x^3 + x + 1임을 이용
byte Xtime(byte fx) {
	return (((fx >> 7) & 0x01) == 1 ? (fx << 1) ^ 0x1b : fx << 1);
	/*
	f(x)에 x^7이 존재한다면, x를 곱해 x^8을 만든 후 x^8을 x^4 + x^3 + x + 1로 바꿈
	f(x)에 x^7이 존재하지 않는다면, x를 곱하기만 함
	*/
}

//유한체 GF(2^8)에서의 곱셈
byte Mul(byte fx, byte gx) {
	byte z; // z = f(x) * g(x)
	int coef; // 계수 (0 또는 1)
	z = 0x00;
	//f(x)=a7(x^7)+a6(x^6)+...+a0
	for (int i = 7; i >= 0; i--) {
		coef = (fx >> i) & 0x01; // a7, a6, a5, ... ,a0
		/*
		f(x)*g(x)
		= g(x)*a0 + g(x)*a1*x + g(x)*a2*x^2 + g(x)*a3*x^3 + ... + g(x)*a7*x^7
		= g(x)*a0 + x*( g(x)*a1 + g(x)*a2*x + g(x)*a3*x^2 + ... +g(x)*a7*x^6 )
		= g(x)*a0 + x*( g(x)*a1 + x*( g(x)*a2 + g(x)*a3*x + ... +g(x)*a7*x^5)  )
		= g(x)*a0 + x*( ...   + x*(g(x)*a7) )  )
		*/
		z = Xtime(z);
		if (coef == 1) {
			z = z ^ gx;
		}
	}
	return z;
}

//addroundkey 단계
void AddRoundKey(byte* state, byte* roundkey)	//입력값, 라운드키를 입력 받음
{
	for (int i = 0; i < 16; i++) {
		state[i] ^= roundkey[i];	//블럭에 라운드 키를 XOR함
	}
}

//subbytes 단계
void SubByte(byte* state)
{
	for (int i = 0; i < 16; i++) {
		state[i] = Sbox[state[i]];	//블럭 안의 바이트를 sbox의 값으로 치환
	}
}

//inverse subbytes 단계
void InvSubByte(byte* state)
{
	for (int i = 0; i < 16; i++) {
		state[i] = InvSbox[state[i]];	//블럭 안의 바이트를 Invsbox의 값으로 치환
	}
}

//shiftrows 단계
void ShiftRow(byte* state)
{
	byte x[16];

	/*
	state[i]=si(0<=i<15)
	s0	s4	s8	s12	 => s0	s4	s8	s12
	s1	s5	s9	s13		s5	s9	s13	s1
	s2	s6	s10	s14		s10	s14	s2	s6
	s3	s7	s11	s15		s15	s3	s7	s11
	두번째 줄은 왼쪽으로 한 칸, 세번째 줄은 두 칸, 네번째 줄은 세 칸 만큼 밈
	*/

	x[0] = state[0];
	x[1] = state[5];
	x[2] = state[10];
	x[3] = state[15];

	x[4] = state[4];
	x[5] = state[9];
	x[6] = state[14];
	x[7] = state[3];

	x[8] = state[8];
	x[9] = state[13];
	x[10] = state[2];
	x[11] = state[7];

	x[12] = state[12];
	x[13] = state[1];
	x[14] = state[6];
	x[15] = state[11];
	//x에 shiftrow한 배열을 받음

	for (int i = 0; i < 16; i++) {
		state[i] = x[i];	//입력값에 x를 넣음
	}

}

//inverse shiftrows 단계
void InvShiftRow(byte* state)
{
	byte x[16];

	/*
	state[i]=si(0<=i<15)
	s0	s4	s8	s12	 => s0	s4	s8	s12
	s1	s5	s9	s13		s13	s1	s5	s9
	s2	s6	s10	s14		s10	s14	s2	s6
	s3	s7	s11	s15		s7	s11	s15	s3
	shiftrow 단계와 반대로
	두 번째 줄은 왼쪽으로 3번, 세 번째 줄은 2번, 네 번째 줄은 1번 밈
	*/

	x[0] = state[0];
	x[1] = state[13];
	x[2] = state[10];
	x[3] = state[7];

	x[4] = state[4];
	x[5] = state[1];
	x[6] = state[14];
	x[7] = state[11];

	x[8] = state[8];
	x[9] = state[5];
	x[10] = state[2];
	x[11] = state[15];

	x[12] = state[12];
	x[13] = state[9];
	x[14] = state[6];
	x[15] = state[3];

	for (int i = 0; i < 16; i++) {
		state[i] = x[i];
	}
}

//mixcolumns 단계
void MixColumn(byte* state)
{
	byte MixCol[4][4] = { {0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01},
					  {0x01, 0x01, 0x02, 0x03} , {0x03, 0x01, 0x01, 0x02}
	};
	byte x[4][4];
	byte temp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			x[i][j] = state[i * 4 + j];
		}
	} //입력받은 state를 4x4 행렬로 바꿈
	/*
	state[i]=si(0<=i<15)
	s0	s4	s8	s12
	s1	s5	s9	s13	
	s2	s6	s10	s14
	s3	s7	s11	s15
	*/

	for (int i = 0; i < 4; i++) {
		/* state[i]=s[i], MixCol[i][j]=m[i][j]
		s[i * 4 + j] += m[j][k] * x[i][k](k=0~k=3)
		*/
		for (int j = 0; j < 4; j++) {
			temp = 0x00; // temp=s[i * 4 + j]
			for (int k = 0; k < 4; k++) {
				temp ^= Mul(x[i][k], MixCol[j][k]); //temp에 m[j][k]과 x[i][k]를 곱한값들을 합함(XOR)
			}
			state[i * 4 + j] = temp;
		}
	}
}

//inverse mixcolumns 단계
void InvMixColumn(byte* state)
{
	byte InvMixCol[4][4] = { {0x0e, 0x0b, 0x0d, 0x09}, {0x09, 0x0e, 0x0b, 0x0d},
					  {0x0d, 0x09, 0x0e, 0x0b} , {0x0b, 0x0d, 0x09, 0x0e}
	};

	byte x[4][4];
	byte temp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			x[i][j] = state[i * 4 + j];
		}
	}

	for (int i = 0; i < 4; i++) {
		/* state[i]=s[i], InvMixCol[i][j]=im[i][j]
		s[i * 4 + j] += im[j][k] * x[i][k](k=0~k=3)
		*/
		for (int j = 0; j < 4; j++) {
			temp = 0x00; // temp=s[i * 4 + j]
			for (int k = 0; k < 4; k++) {
				temp ^= Mul(x[i][k], InvMixCol[j][k]); //temp에 im[j][k]과 x[i][k]를 곱한값들을 합함(XOR)
			}
			state[i * 4 + j] = temp;
		}
	}
}

//keyexpansion 단계
void KeySchedule(byte* Key, int round) //주 암호화 키와 몇 라운드인지 알려주는 값을 받음
{
	/*Key를 4x4행렬로 바꿈
	Key[i] = ki(0 <= i < 15)
	k0	k4	k8	k12 
	k1	k5	k9	k13	
	k2	k6	k10	k14	
	k3	k7	k11	k15
	*/

	byte x[4];
	x[0] = Key[13];
	x[1] = Key[14];
	x[2] = Key[15];
	x[3] = Key[12];
	//Key 행렬의 맨 오른쪽 열 부분을 가져와 위쪽으로 1번 밈
	for (int j = 0; j < 4; j++) {
		x[j] = Sbox[x[j]];
	} //밀고 난 열의 각 원소를 sbox의 값으로 치환

	Key[0] ^= x[0] ^ rcon[round-1]; //라운드키[0]는 rcon값과 xor연산도 추가
	Key[1] ^= x[1];
	Key[2] ^= x[2];
	Key[3] ^= x[3];
	//맨 왼쪽 열과 sbox값으로 치환한 x열을 xor한 값이 라운드 키의 첫번째 열

	for (int j = 4; j < 16; j++)
	{
		Key[j] ^= Key[j - 4];
	} //나머지 2, 3, 4 열은 각각 1, 2, 3열과 xor한 값이 라운드 키의 2, 3, 4열 
}

//AES 암호화
void AES_Encrypt(byte* PT, byte* K, byte* CT) //평문, 키, 결과값을 저장할 암호문을 입력받음
{
	printf("[AES 암호화 실행]\n");
	byte state[16];

	printf("ROUND[0]\n"); //0라운드
	for (int i = 0; i < 16; i++) {
		state[i] = PT[i]; //state에 평문을 입력받음
	}
	printf("input=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", state[j]);
	}
	printf("\n");

	byte RK[16]; //라운드키
	
	for (int i = 0; i < 16; i++) {
		RK[i] = K[i]; //0라운드에서는 키값이 그대로
	}
	printf("keyshcedule=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", RK[j]);
	}
	printf("\n");

	AddRoundKey(state, RK);
	printf("addroundkey=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", state[j]);
	}
	printf("\n");
	//0라운드: 라운드 키 중 첫번째 키를 사용, addroundkey를 한번 실행

	for (int i = 1; i <= 9; i++) { //1~9라운드
		printf("ROUND[%d]\n", i);

		SubByte(state);
		printf("subbyte=");
		for (int j = 0; j < 16; j++) {
			printf("%02x", state[j]);
		}
		printf("\n");

		ShiftRow(state);
		printf("shiftrow=");
		for (int j = 0; j < 16; j++) {
			printf("%02x", state[j]);
		}
		printf("\n");

		MixColumn(state);
		printf("mixcolumn=");
		for (int j = 0; j < 16; j++) {
			printf("%02x", state[j]);
		}
		printf("\n");

		KeySchedule(RK, i);
		printf("newroundkey="); //라운드에 필요한 라운드키를 입력받음
		for (int j = 0; j < 16; j++) {
			printf("%02x", RK[j]);
		}
		printf("\n");

		AddRoundKey(state, RK);
		printf("addroundkey=");
		for (int j = 0; j < 16; j++) {
			printf("%02x", state[j]);
		}
		printf("\n");
		//1~9라운드: subbytes, shiftrows, mixcolumns, addroundkey를 순서대로 실행
	}

	printf("round[10]\n"); //10라운드

	SubByte(state);
	printf("subbyte=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", state[j]);
	}
	printf("\n");

	ShiftRow(state);
	printf("shiftrow=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", state[j]);
	}
	printf("\n");

	KeySchedule(RK, 10);
	printf("newroundkey=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", RK[j]);
	}
	printf("\n");

	AddRoundKey(state, RK);
	//subbytes, shiftrows, addroundkey를 순서대로 실행

	for (int i = 0; i < 16; i++) {
		CT[i] = state[i]; //결과 값인 암호문 출력
	}
}

//AES 복호화
void AES_Decrypt(byte* CT, byte* K, byte* PT) //암호문, 키, 결과값을 저장할 평문을 입력받음
{
	printf("[AES 복호화 실행]\n");
	byte state[16];

	printf("ROUND[0]\n"); //0라운드
	for (int i = 0; i < 16; i++) {
		state[i] = CT[i]; //state에 암호문을 입력받음
	}

	byte RK[16]; //라운드키

	for (int i = 0; i < 16; i++) {
		RK[i] = K[i];
	}
	for (int j = 1; j <= 10; j++) {
		KeySchedule(RK, j);
	} //암호화의 10라운드에서 쓰인 라운드키를 받음
	printf("newroundkey=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", RK[j]);
	}
	printf("\n");

	AddRoundKey(state, RK);
	printf("addroundkey=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", state[j]);
	}
	printf("\n");

	for (int i = 10; i > 1; i--) { //1~9라운드
		printf("ROUND[%d]\n", 11 - i);

		InvShiftRow(state);
		printf("Invshiftrow=");
		for (int j = 0; j < 16; j++) {
			printf("%02x", state[j]);
		}
		printf("\n");

		InvSubByte(state);
		printf("Invsubbyte=");
		for (int j = 0; j < 16; j++) {
			printf("%02x", state[j]);
		}
		printf("\n");

		for (int j = 0; j < 16; j++) {
			RK[j] = K[j];
		}
		for (int j = 1; j <= i - 1; j++) {
			KeySchedule(RK, j); //암호화의 i-1라운드에서 쓰인 라운드키를 받음
		}
		printf("newroundkey=");
		for (int j = 0; j < 16; j++) {
			printf("%02x", RK[j]);
		}
		printf("\n");

		AddRoundKey(state, RK);
		printf("addroundkey=");
		for (int j = 0; j < 16; j++) {
			printf("%02x", state[j]);
		}
		printf("\n");

		InvMixColumn(state);
		printf("invmixcolumn=");
		for (int j = 0; j < 16; j++) {
			printf("%02x", state[j]);
		}
		printf("\n");
		//inverse shiftrows, inverse subbytes, addroundkey, inverse mixcolumns를 순서대로 실행
	}

	printf("ROUND[10]\n"); //10라운드

	InvShiftRow(state);
	printf("Invshiftrow=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", state[j]);
	}
	printf("\n");

	InvSubByte(state);
	printf("Invsubbyte=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", state[j]);
	}
	printf("\n");

	printf("newroundkey=");
	for (int j = 0; j < 16; j++) {
		printf("%02x", K[j]);
	} //암호화의 0라운드에서 쓰인 라운드키는 원래 키값이므로 키값 그대로를 받음
	printf("\n");

	AddRoundKey(state, K);
	//inverse shiftrows, inverse subbytes, addroundkey를 순서대로 실행

	for (int i = 0; i < 16; i++) {
		PT[i] = state[i]; //결과값인 평문 출력
	}

}

int main()
{
	//테스트벡터
	byte pt[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
					0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
	//테스트 평문
	byte key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
					0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	//테스트 암호 키
	byte ct[16];
	//테스트 암호문
	AES_Encrypt(pt, key, ct); //AES 암호화 실행

	printf("CT = ");
	for (int i = 0; i < 16; i++) {
		printf("%02x ", ct[i]);
	}
	printf("\n"); //암호문 출력

	printf("==============================================================\n");

	byte npt[16];
	AES_Decrypt(ct, key, npt); //AES 복호화 실행

	printf("PT = ");
	for (int i = 0; i < 16; i++) {
		printf("%02x ", npt[i]);
	}
	printf("\n"); //평문 출력
}