
main : main.o chip8.o graphics.o
	g++ -o Chip8 main.o chip8.o graphics.o -lmingw32 -lSDL2main -lSDL2  

main.o : main.cpp
	g++ -c main.cpp

graphics.o : graphics.cpp
	g++ -c graphics.cpp

chip8.o: chip8.cpp
	g++ -c chip8.cpp

clean :
	rm -f *.o
