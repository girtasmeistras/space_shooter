CXXFLAGS = -Isdl2/32bit/include/SDL2 -std=c++0x

LXXFLAGS = -Lsdl2/32bit/lib -lmingw32 -lSDL2main -lSDL2



main: main.o bullet.o player.o helpers.o application.o application.h player.h bullet.h helpers.h
	g++ main.o helpers.o bullet.o player.o application.o -o main $(LXXFLAGS)



main.o: main.cpp application.h 

	g++ main.cpp -c $(CXXFLAGS)



application.o: application.cpp application.h bullet.h player.h helpers.h

	g++ application.cpp -c $(CXXFLAGS)



player.o: player.cpp player.h bullet.h helpers.h

	g++ player.cpp -c $(CXXFLAGS)



bullet.o: bullet.cpp bullet.h

	g++ bullet.cpp -c $(CXXFLAGS)



helpers.o: helpers.cpp helpers.h

	g++ helpers.cpp -c $(CXXFLAGS)