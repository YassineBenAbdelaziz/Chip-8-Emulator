#pragma once

#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#define INITIAL_ADDRESS 0x200 
#define FONTSET_SIZE  80
#define FONTSET_ADDRESS 0X50

class Chip8 {

public :


    uint16_t opcode;
    uint8_t reg[16] ;
    uint8_t memory[4096] ;
    uint16_t indexReg ;
    uint16_t PC ;
    uint8_t SP ;
    uint8_t stack[16] ;
    uint8_t delayTimer ;
	uint8_t soundTimer ;
    uint8_t keys[16] ;
	uint32_t video[2048] ;

    uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


    Chip8();
    void load_ROM(char* filename);
    void inst_COPY();

    void inst_ADD();
    void inst_SUBR() ;


    void inst_AND();
    void inst_XOR();

    void inst_SHR();
    void inst_SHL();

    
    void inst_SUB() ;
    void inst_RET() ;


    void inst_JUMP();
    void inst_CALL() ;


    void inst_SEI();
    void inst_SNEI();
    void inst_SE();

    void inst_SNE();
    void inst_BR();


    void inst_SKP();
    void inst_SKPN();


    void inst_ADDI();
    void inst_STRI();


    void inst_STR();
    void inst_OR();

    void inst_STRD();
    void inst_SETD();


    void inst_SETS();
    void inst_WAIT();


    void inst_OFFS();
    void inst_LDSprite();


    void inst_BCD();
    void inst_STRM();


    void inst_LDM();
    void inst_CLR();
    
    uint8_t randGen();
    void inst_RND();


    void inst_DRAW();
    void inst_null();

    void decode();
    void cycle();
};








