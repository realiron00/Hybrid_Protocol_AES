#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "aes_protocol.h"
#include "AES_CBC.h"

//파일을 암호화하는 함수
void se_encrypt(const char* pt_name, const char* Key_name, const char* IV_name, const char* ct_name)
{
	FILE* keyfile;
	FILE* IVfile;
	byte CBCkey[16];
	byte IV[16];

	keyfile = fopen(Key_name, "rb");
	IVfile = fopen(IV_name, "rb");

	fread((char*)CBCkey, sizeof(byte), 16, keyfile);
	fread((char*)IV, sizeof(byte), 16, IVfile);
	file_AES_CBC(pt_name, IV, CBCkey, ct_name);

}

//파일을 복호화하는 함수
void se_decrypt(const char* ct_name, const char* Key_name, const char* IV_name, const char* pt_name)
{
	FILE* keyfile;
	FILE* IVfile;
	byte CBCkey[16];
	byte IV[16];

	keyfile = fopen(Key_name, "rb");
	IVfile = fopen(IV_name, "rb");

	fread((char*)CBCkey, sizeof(byte), 16, keyfile);
	fread((char*)IV, sizeof(byte), 16, IVfile);
	Inv_file_AES_CBC(ct_name, IV, CBCkey, pt_name);
}

int main()
{
	int w_check; //원하는 작업을 입력받음
	char pt_filename[80]; //평문 파일
	char ct_filename[80]; //암호된 파일
	char Key_filename[80]; //키값이 저장되어있는 파일
	char IV_filename[80]; //초기벡터가 저장되어있는 파일
	
	//암호화된 파일에 올바를 확장자명을 입력받기 위해 쓰는 변수
	char drive[11];
	char dir[256];
	char fname[256];
	char ext[256];
	//변수에 작업을 수행할 파일의 정보를 입력받음

	printf("\n원하는 작업을 입력하시오.(1: 파일 암호화, 2: 파일 복호화):");
	scanf("%d", &w_check);
	if (w_check == 1) { //파일 암호화
		printf("암호화할 평문파일 이름 입력:");
		scanf("%s", pt_filename);
		printf("암호화 키 입력:");
		scanf("%s", Key_filename);
		printf("CBC모드를 수행하기 위한 초기벡터 입력:");
		scanf("%s", IV_filename);
		char ct_name[80];
		printf("암호화파일 이름 입력:");
		scanf("%s", ct_name);
		_splitpath(pt_filename, drive, dir, fname, ext);
		sprintf(ct_filename, "%s%s", &ct_name, &ext); //암호화파일이름+평문의확장자명
		printf("[암호화 수행]\n");
		se_encrypt(pt_filename, Key_filename, IV_filename, ct_filename);
		printf("%s로 파일 암호화 수행 완료", &ct_filename);
	}
	else { //파일 복호화
		printf("복호화할 암호문파일 이름 입력:");
		scanf("%s", ct_filename);
		printf("암호화 키 입력:");
		scanf("%s", Key_filename);
		printf("CBC모드를 수행하기 위한 초기벡터 입력:");
		scanf("%s", IV_filename);
		char pt_name[80];
		printf("복호화파일 이름 입력:");
		scanf("%s", pt_name);
		_splitpath(ct_filename, drive, dir, fname, ext); //복호화파일이름+암호문의확장자명
		sprintf(pt_filename, "%s%s", &pt_name, &ext);
		printf("[복호화 수행]\n");
		se_decrypt(ct_filename, Key_filename, IV_filename, pt_filename);
		printf("%s로 파일 복호화 수행 완료", &pt_filename);
	}
}