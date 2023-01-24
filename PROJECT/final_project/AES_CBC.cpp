#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "aes_protocol.h"

//파일의 길이를 알려주는 함수
int FileSize(const char* fname)
{
	FILE* myfile = fopen(fname, "rb");
	int file_length; //file_length에 파일의 길이를 저장

	fseek(myfile, 0, SEEK_END);
	file_length = ftell(myfile);
	fclose(myfile);

	return file_length; //file_length 반환
}

//파일을 패딩하는 함수
void File_Padding(const char* input, const char* output)
{
	FILE* infile = fopen(input, "rb");
	FILE* outfile = fopen(output, "wb");

	int file_len;
	file_len = FileSize(input);

	int num_blocks = file_len / 16 + 1;
	int remainder = file_len - (num_blocks - 1) * 16;

	byte nblock[16]; //패딩을 안해도 되는 블록(마지막 블록 전까지)

	for (int i = 0; i < num_blocks - 1; i++) {
		fread((char*)nblock, sizeof(byte), 16, infile);
		fwrite((char*)nblock, sizeof(byte), 16, outfile); 
	}//마지막 블록 전까지는 output파일에 옮겨줌

	byte last_block[16]; //마지막 블록
	byte ch;

	if (remainder == 0) {
		fclose(infile);
		fclose(outfile);
	}//마지막 블록이 없다면 그대로 끝냄
	else { //패딩할 블록이 있다면
		for (int i = 0; i < remainder; i++) {
			fread((char*)&ch, sizeof(byte), 1, infile);
			last_block[i] = ch;
		} //남은 데이터는 옮겨줌

		byte padding_number = 0x00 + (16 - remainder);
		for (int i = remainder; i < 16; i++) {
			last_block[i] = padding_number; //빈공간은 남은 공간의 길이 숫자를 넣어서 패딩함
		}
		fwrite((char*)last_block, sizeof(byte), 16, outfile); //패딩까지 한 마지막 블록을 output파일에 옮겨줌

		fclose(infile);
		fclose(outfile);
	}
}

//패딩을 지우는 함수
void erase_padding(const char* pad_name, const char* origin_name)
{
	FILE* infile = fopen(pad_name, "rb");
	FILE* outfile = fopen(origin_name, "wb");

	int file_len;
	file_len = FileSize(pad_name);
	int num_blocks = file_len / 16 + 1;

	byte buffer[16];
	int x;

	for (int i = 0; i < num_blocks - 2; i++) {
		fread((char*)buffer, sizeof(byte), 16, infile);
		fwrite((char*)buffer, sizeof(byte), 16, outfile); 
	}//패딩된 부분의 전 블록까지 origin파일에 저장

	byte last_block[16];
	fread((char*)last_block, sizeof(byte), 16, infile); //마지막 블록을 last_block에 저장
	for (int i = 0; i < 16; i++) {
		if (last_block[16 - i] == i) {
			for (int j = 16 - i; j < 16; j++) {
				if (last_block[j] == i) x = i; //마지막 블록에서 숫자 i가 마지막에서 i개 만큼 존재한다면 x=i
				else x = 0;
			}
			if (x == i) break;
		}
		else x = 0;
	}
	for (int i = 0; i < 16 - x; i++) { //0부터 16-x까지의 부분을 origin파일에 저장
		fwrite((char*)&last_block[i], sizeof(byte), 1, outfile);
	}

	fclose(infile);
	fclose(outfile);
}

//CBC모드 AES 파일 암호화
void file_AES_CBC(const char *pt_name, byte IV[16], byte Key[16], const char *ct_name)
{
	byte rk[11][16];
	FILE *pt_file;
	FILE *ct_file;

	byte pt_block[16];
	byte ct_block[16];

	KeySchedule(Key, rk); //라운드키 획득

	const char* pad_file = "AES_CBC-Padded.bin";
	File_Padding(pt_name, pad_file); //파일을 패딩하여 저장

	int file_len;
	file_len = FileSize(pad_file);
	int num_blocks = file_len / 16 + 1;

	pt_file = fopen(pad_file, "rb");
	ct_file = fopen(ct_name, "wb");

	for (int i = 0; i < num_blocks - 1; i++) {
		if (i == 0) { //CBC 첫블록
			for (int j = 0; j < 16; j++) {
				fread((char*)&pt_block[j], sizeof(byte), 1, pt_file); //CBC의 첫블록을 pt_block에 저장
				pt_block[j] = pt_block[j] ^ IV[j]; //pt_block과 초기벡터를 XOR연산 수행
			}
			AES_Encrypt(pt_block, rk, ct_block); //pt_block를 암호화하여 ct_block에 1블록 입력
			fwrite((char*)ct_block, sizeof(byte), 16, ct_file); //암호화파일에 ct_block을 1블록 입력
		}
		else { //첫블록 이후
			for (int j = 0; j < 16; j++) {
				fread((char*)&pt_block[j], sizeof(byte), 1, pt_file); //pt_block에 평문파일을 1블록씩 입력
				pt_block[j] = pt_block[j] ^ ct_block[j]; //pt_block과 직전의 암호문 블록을 XOR연산 수행
			}
			AES_Encrypt(pt_block, rk, ct_block); //pt_block를 암호화하여 ct_block에 1블록 입력
			fwrite((char*)ct_block, sizeof(byte), 16, ct_file); //암호화파일에 ct_block을 1블록씩 입력
		}
	}

	fclose(pt_file);
	remove(pad_file); //패딩되었던 파일은 삭제
	fclose(ct_file);
}

//CBC모드 AES 파일 복호화
void Inv_file_AES_CBC(const char *ct_name, byte IV[16], byte Key[16], const char *pt_name)
{
	byte rk[11][16];
	FILE *pt_file;
	FILE *ct_file;

	byte pt_block[16];
	byte ct1_block[16];
	byte ct2_block[16];

	KeySchedule(Key, rk); //라운드키 획득

	const char* pad_file = "Inv_AES_CBC-Padded.bin"; //패딩을 지우기 전인 파일

	int file_len;
	file_len = FileSize(ct_name);
	int num_blocks = file_len / 16 + 1;

	ct_file = fopen(ct_name, "rb");
	pt_file = fopen(pad_file, "wb");

	for (int i = 0; i < num_blocks - 1; i++) {
		if (i == 0) { //CBC 첫블록
			fread((char*)ct1_block, sizeof(byte), 16, ct_file); //암호문파일의 첫블록을 ct1_block에 저장
			AES_Decrypt(ct1_block, rk, pt_block); //ct1_block을 복호화하여 pt_block에 평문 1블록 입력
			for (int j = 0; j < 16; j++) {
				pt_block[j] = pt_block[j] ^ IV[j]; //pt_block과 초기벡터를 XOR연산 수행
				fwrite((char*)&pt_block[j], sizeof(byte), 1, pt_file); //pad_file에 pt_block 1블록 저장
			}
		}
		else { //첫블록 이후
			fread((char*)ct2_block, sizeof(byte), 16, ct_file); //ct2_block에 암호문파일을 1블록씩 입력
			AES_Decrypt(ct2_block, rk, pt_block); //ct2_block을 복호화하여 pt_block에 1블록 입력
			for (int j = 0; j < 16; j++) {
				pt_block[j] = pt_block[j] ^ ct1_block[j]; //pt_block과 직전의 암호문 1블록을 XOR연산 수행
				ct1_block[j] = ct2_block[j]; //ct1_block에 암호문 1블록을 저장
				fwrite((char*)&pt_block[j], sizeof(byte), 1, pt_file); //pad_file에 패딩이 포함된 평문을 입력
			}
		}
	}

	fseek(pt_file, 0, SEEK_SET);
	erase_padding(pad_file, pt_name); //pad_file의 패딩부븐을 제거하여 pt파일에 넣음

	fclose(pt_file);
	remove(pad_file);
	fclose(ct_file); // 파일 닫기
}