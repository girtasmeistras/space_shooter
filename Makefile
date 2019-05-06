CXXFLAGS = -lSDL2 -std=c++0x

LXXFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf



main: main.o bullet.o player.o helpers.o application.o enemy.o entity.o asteroid.o application.h player.h bullet.h enemy.h entity.h helpers.h

	g++ main.o helpers.o bullet.o player.o enemy.o entity.o asteroid.o application.o -o main $(LXXFLAGS)


main.o: main.cpp application.h 

	g++ main.cpp -c $(CXXFLAGS)



application.o: application.cpp application.h bullet.h player.h entity.h asteroid.h helpers.h

	g++ application.cpp -c $(CXXFLAGS)

player.o: player.cpp player.h bullet.h helpers.h entity.h

	g++ player.cpp -c $(CXXFLAGS)

enemy.o: enemy.cpp enemy.h bullet.h entity.h

	g++ enemy.cpp -c $(CXXFLAGS)

bullet.o: bullet.cpp bullet.h entity.h

	g++ bullet.cpp -c $(CXXFLAGS)

asteroid.o: asteroid.cpp asteroid.h entity.h

	g++ asteroid.cpp -c $(CXXFLAGS)

entity.o: entity.cpp entity.h

	g++ entity.cpp -c $(CXXFLAGS)

helpers.o: helpers.cpp helpers.h

	g++ helpers.cpp -c $(CXXFLAGS)

