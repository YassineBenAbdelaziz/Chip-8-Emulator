#include "chip8.hpp"
#include "graphics.hpp"
#include <iostream>



int main(int argc, char* argv[]){

    if ( argc != 4 ) {

        std::cout << "Missing arguments";
        exit(-1);
    }


    int scaleFactor = std::stoi(argv[1]);
    int delayTime = std::stoi(argv[2]);


    graphics Graphics("Chip8 Emulator",VIDEO_WIDTH * scaleFactor, VIDEO_HEIGHT * scaleFactor,VIDEO_WIDTH,VIDEO_HEIGHT);
    Chip8 chip8;

    chip8.load_ROM(argv[3]);
    

    int pitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

    bool quit = false ;

    while(!quit){

        quit = Graphics.ProcessInput(chip8.keys);
        Graphics.delay(delayTime);

        chip8.cycle();

        Graphics.Update(chip8.video,pitch);


    }
    

    return 0;
}