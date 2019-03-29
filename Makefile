CXXFLAGS = -lSDL2 -std=c++0x

LXXFLAGS = -lSDL2 -lSDL2_image



main: main.o bullet.o player.o helpers.o application.o enemy.o entity.o application.h player.h bullet.h enemy.h entity.h helpers.h

	g++ main.o helpers.o bullet.o player.o enemy.o entity.o application.o -o main $(LXXFLAGS)



main.o: main.cpp application.h 

	g++ main.cpp -c $(CXXFLAGS)



application.o: application.cpp application.h bullet.h player.h helpers.h

	g++ application.cpp -c $(CXXFLAGS)



player.o: player.cpp player.h bullet.h helpers.h

	g++ player.cpp -c $(CXXFLAGS)


enemy.o: enemy.cpp enemy.h bullet.h

	g++ enemy.cpp -c $(CXXFLAGS)

bullet.o: bullet.cpp bullet.h

	g++ bullet.cpp -c $(CXXFLAGS)

entity.o: entity.cpp entity.h

	g++ entity.cpp -c $(CXXFLAGS)

helpers.o: helpers.cpp helpers.h

	g++ helpers.cpp -c $(CXXFLAGS)

