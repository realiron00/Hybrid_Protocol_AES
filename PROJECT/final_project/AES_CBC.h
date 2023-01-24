#pragma once
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char byte;

int FileSize(const char* fname);

void File_Padding(const char* input, const char* output);

void erase_padding(const char* pad_name, const char* origin_name);

void file_AES_CBC(const char *pt_name, byte IV[16], byte Key[16], const char *ct_name);

void Inv_file_AES_CBC(const char *ct_name, byte IV[16], byte Key[16], const char *pt_name);
