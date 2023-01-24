#pragma once
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef unsigned char byte;

byte Xtime(byte fx);

byte Mul(byte fx, byte gx);

void AddRoundKey(byte* state, byte* roundkey);

void SubByte(byte* state);

void InvSubByte(byte* state);

void ShiftRow(byte* state);

void InvShiftRow(byte* state);

void MixColumn(byte* state);

void InvMixColumn(byte* state);

void KeySchedule(byte Key[16], byte rk[11][16]);

void AES_Encrypt(byte* PT, byte RK[11][16], byte* CT);

void AES_Decrypt(byte* CT, byte RK[11][16], byte* PT);