#include <iostream>
#include <cstdio>
#include <fstream>
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




    void inst_ADD_R (){

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


    void inst_SUB_R() {
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

    void inst_LD_R() {

        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        reg[Vx] = reg[Vy];

    }

    void inst_OR_R() {

        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        reg[Vx] |= reg[Vy];        



    }


    void inst_AND_R(){
        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        reg[Vx] &= reg[Vy];  


    }

    void inst_XOR_R(){
        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        uint8_t Vy = ( opcode & 0x00F0 ) >> 4;

        reg[Vx] ^= reg[Vy];  


    }

    void inst_SHR_R(){
        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        

        reg[Vx] >>= 1 ;  
        reg[15] = reg[Vx] & 0x1;
    }

    void inst_SHL_R(){
        uint8_t Vx = ( opcode & 0x0F00 ) >> 8;
        

        reg[Vx] <<= 1 ;  
        reg[15] = ( reg[Vx] & 0x80 ) >> 7 ;
    }



    
    void inst_SUBN_R() {
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














};




















