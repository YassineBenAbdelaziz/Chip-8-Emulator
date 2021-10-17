#include <iostream>
#include <cstdio>
#include <fstream>
#include <stdlib.h>
#define INITIAL_ADDRESS 0x200 
#define FONTSET_SIZE  80
#define FONTSET_ADDRESS 0X50



using namespace std;


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


    Chip8(){
        
        PC = INITIAL_ADDRESS ;

	for ( int i = 0; i < FONTSET_SIZE; i++)
	{
		memory[FONTSET_ADDRESS + i] = fontset[i];
	}

    }


    void load_ROM(char* filename){
        streampos size;
        char * buffer;
        ifstream myfile ("example.txt", ios::binary | ios::ate);
        if (myfile.is_open())
        {
            size = myfile.tellg();
            buffer = new char [size];
            myfile.seekg (0, ios::beg);
            myfile.read (buffer, size);
            myfile.close();

        for (int i = 0; i < size; ++i)
		{
			memory[INITIAL_ADDRESS + i] = buffer[i];
		}
            delete[] buffer ;
            
        }
        else cout << "Can't open file";

    }




    void inst_ADD(){

        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        if (reg[Vx] + reg[Vy] > 255) {

            reg[15]  = 1 ; // Set overflow flag

        }

        else {

            reg[15]  = 0 ;

        }

        reg[Vx] = (reg[Vx] + reg[Vy]) & 0xFFu;

    }


    void inst_SUBR() {
        // Vx - Vy
        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        if (reg[Vx] > reg[Vy] ){

            reg[15] = 1 ;

        }
        else {

            reg[15] = 0 ;
        }

        reg[Vx] -= reg[Vy] ;



    }

    void inst_COPY() {

        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        reg[Vx] = reg[Vy];

    }

    void inst_OR() {

        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        reg[Vx] |= reg[Vy];        



    }


    void inst_AND(){
        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        reg[Vx] &= reg[Vy];  


    }

    void inst_XOR(){
        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        reg[Vx] ^= reg[Vy];  


    }

    void inst_SHR(){
        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        

        reg[Vx] >>= 1 ;  
        reg[15] = reg[Vx] & 0x1;
    }

    void inst_SHL(){
        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        

        reg[Vx] <<= 1 ;  
        reg[15] = ( reg[Vx] & 0x80 ) >> 7 ;
    }



    
    void inst_SUB() {
        // Vy - Vx
        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        if (reg[Vy] > reg[Vx] ){

            reg[15] = 1 ;

        }
        else {

            reg[15] = 0 ;
        }

        reg[Vx] += - reg[Vy] ;



    }

    void inst_RET() {

        SP-- ;
        PC = stack[SP] ;


    }



    void inst_JUMP(){

        PC = opcode & 0x0FFF ;

    }


    void inst_CALL() {

        stack[SP] = PC ;
        SP++ ;
        PC = opcode & 0x0FFF ;

    }


    void inst_SEI(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;
        uint8_t immediate = (opcode & 0x00FF) ;

        if ( reg[Vx] == immediate) {

            PC += 2 ;
        }

    }


    void inst_SNEI(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;
        uint8_t immediate = (opcode & 0x00FF) ;

        if ( reg[Vx] != immediate) {

            PC += 2 ;
        }

    }


    void inst_SE(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;
        uint8_t Vy = (opcode & 0x00F0) >> 4;

        if ( reg[Vx] == reg[Vy]) {

            PC += 2 ;
        }

    }

    void inst_SNE(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;
        uint8_t Vy = (opcode & 0x00F0) >> 4;

        if ( reg[Vx] != reg[Vy]) {

            PC += 2 ;
        }

    }

    void inst_BR(){

        PC = reg[0] + ( opcode & 0x0FFF ) ;


    }


    void inst_SKP(){

        uint8_t Vx = (opcode & 0x0F00) >> 8 ;

        if (keys[reg[Vx]]) {
            PC += 2 ;

        }

    }


    void inst_SKPN(){

        uint8_t Vx = (opcode & 0x0F00) >> 8 ;

        if ( !keys[reg[Vx]]) {
            PC += 2 ;

        }

    }    




    void inst_ADDI(){

    uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
    uint16_t immediate = opcode & 0x00FF ;
    reg[Vx] += immediate ;

}

    void inst_STRI(){

        uint8_t Vx = (opcode & 0x0F00) >> 8 ;
        uint8_t immediate = (opcode & 0x00FF) ;

        reg[Vx] = immediate ;

    }



    void inst_STR(){

        uint16_t immediate = (opcode & 0x0FFF) ;

        indexReg = immediate ;
    
    }


    void inst_STRD(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;

        reg[Vx] = delayTimer ;

    }

    void inst_SETD(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;

        delayTimer = reg[Vx] ;

    }

    void inst_SETS(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;

        soundTimer = reg[Vx] ;

    }


    void inst_WAIT(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;

        if (keys[0])
        {
            reg[Vx] = 0;
        }
        else if (keys[1])
        {
            reg[Vx] = 1;
        }
        else if (keys[2])
        {
            reg[Vx] = 2;
        }
        else if (keys[3])
        {
            reg[Vx] = 3;
        }
        else if (keys[4])
        {
            reg[Vx] = 4;
        }
        else if (keys[5])
        {
            reg[Vx] = 5;
        }
        else if (keys[6])
        {
            reg[Vx] = 6;
        }
        else if (keys[7])
        {
            reg[Vx] = 7;
        }
        else if (keys[8])
        {
            reg[Vx] = 8;
        }
        else if (keys[9])
        {
            reg[Vx] = 9;
        }
        else if (keys[10])
        {
            reg[Vx] = 10;
        }
        else if (keys[11])
        {
            reg[Vx] = 11;
        }
        else if (keys[12])
        {
            reg[Vx] = 12;
        }
        else if (keys[13])
        {
            reg[Vx] = 13;
        }
        else if (keys[14])
        {
            reg[Vx] = 14;
        }
        else if (keys[15])
        {
            reg[Vx] = 15;
        }
        else
        {
            PC -= 2;
        }


    }


    void inst_OFFS(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;

        indexReg += reg[Vx] ;

    }

    void inst_LDSprite(){
        uint8_t Vx = (opcode & 0x0F00) >> 8;

        indexReg = FONTSET_ADDRESS + (5 * reg[Vx]) ;

    }


    void inst_BCD(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;
        uint8_t value = reg[Vx] ;

        memory[indexReg + 2] = value % 10;
        value /= 10 ;
        memory[indexReg + 1] = value % 10;
        value /= 10 ;
        memory[indexReg ] = value % 10;
    }



    void inst_STRM(){
        uint8_t Vx = (opcode & 0x0F00) >> 8;

        for (uint8_t i = 0; i <= Vx; i++)
        {
            memory[indexReg + i] = reg[i];
        }

    }


    void inst_LDM(){
        uint8_t Vx = (opcode & 0x0F00) >> 8;

        for (uint8_t i = 0; i <= Vx; i++)
        {
            
            reg[i] = memory[indexReg + i ] ;
        }
    }


    void inst_CLR(){

        memset(video, 0, sizeof(video));

    }
    
    uint8_t randGen(){

        return (uint8_t)(rand() % 100);


    }


    void inst_RND(){

        uint8_t Vx = (opcode & 0x0F00) >> 8;

        reg[Vx] = randGen() & ( opcode & 0x00FF);


    }



    void inst_DRAW(){

        uint8_t Vx = (opcode & 0x0F00 ) >> 8 ;
        uint8_t Vy = (opcode & 0x00F0 ) >> 4 ;
        uint8_t Height = opcode & 0x000F ;
        uint8_t x = reg[Vx] % 64 ;
        uint8_t y = reg[Vy] % 32 ;
        reg[15] = 0;

        for(uint8_t row = 0 ; row < Height ; row++){

            uint8_t rowbyte = memory[indexReg + row ] ;
            for( uint8_t col = 0 ; col < 8 ; col ++ ){
                uint8_t pixel = rowbyte & (0x80 >> col ) ;
                uint32_t* screenPixel = &video[(y + row) * 64 + x + col] ;

                if (pixel){
                    if (*screenPixel == 0xFFFFFFFF)
                        reg[15] = 1 ;
                }

                *screenPixel = *screenPixel ^ 0xFFFFFFFF ;
            }
        }
    }

    void inst_null(){

    }


    void decode(){
        switch (opcode & 0xF000 )
        {
        case 0x0000:
            switch (opcode & 0x000F)
            {
            case 0x0000:
                inst_CLR();
                break;
            case 0x000E:
                inst_RET;
                break;           
            default:
            inst_null();
                break;
            }
            break;
        case 0x8000 : 
            switch (opcode & 0x000F)
            {
            case 0x0000:
                inst_COPY();
                break;
            case 0x0001:
                inst_OR();
                break;
            case 0x0002:
                inst_AND();
                break;
            case 0x0003:
                inst_XOR();
                break;
            case 0x0004:
                inst_ADD();
                break;
            case 0x0005:
                inst_SUB();
                break;
            case 0x0006:
                inst_SHR();
                break;
            case 0x0007:
                inst_SUBR();
                break;
            case 0x000E:
                inst_SHL();
                break;
            default:
                inst_null();
                break;
            }
        case  0xE000 : case  0xF000:
            switch (opcode & 0x00FF)
            {
            case 0x00A1:
                inst_SKPN();
                break;
            case 0x009E:
                inst_SKP();
                break;   
            case 0x0007:
                inst_STRD();
                break;
            case 0x000A:
                inst_WAIT();
                break;
            case 0x0015:
                inst_SETD();
                break;
            case 0x0018:
                inst_SETS();
                break;
            case 0x001E:
                inst_OFFS();
                break;
            case 0x0029:
                inst_LDSprite();
                break;
            case 0x0033:
                inst_BCD();
                break;
            case 0x0055:
                inst_STRM();
                break;
            case 0x0065:
                inst_LDM();
                break;
            default:
                inst_null();
                break;
            }
        case 0x1000:
            inst_JUMP();
            break;
        case 0x2000:
            inst_CALL();
            break;
        case 0x3000:
            inst_SEI();
            break;
        case 0x4000:
            inst_SNEI();
            break;
        case 0x5000:
            inst_SE();
            break;
        case 0x6000:
            inst_STRI();
            break;
        case 0x7000:
            inst_ADDI();
            break;
        case 0x9000:
            inst_SNE();
            break;
        case 0xA000:
            inst_STR();
            break;
        case 0xB000:
            inst_BR();
            break;
        case 0xC000:
            inst_RND();
            break;
        case 0xD000:
            inst_DRAW();
            break;
        default:
            inst_null();
            break;
        }



    }



    void cycle(){
        
        opcode = (memory[PC] << 8) | memory[PC + 1];

        PC += 2;

        decode();

        if (delayTimer > 0)
        {
            --delayTimer;
        }

        if (soundTimer > 0)
        {
            --soundTimer;
        }
    }










};




















