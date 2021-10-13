#include <iostream>
#include <cstdio>
#include <fstream>


using namespace std;


class Chip8 {

private :
    const uint16_t initialAddress = 0x200 ;

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
			memory[initialAddress + i] = buffer[i];
		}
            delete[] buffer ;
            
        }
        else cout << "Can't open file";

    }

};











