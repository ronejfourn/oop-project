game.exe: ./build/main.o ./build/animated_sprite.o ./build/entity.o ./build/graphics.o ./build/player.o ./build/sprite.o ./build/tilemap.o
	g++ ./build/*.o -lSDL2 -lSDL2main -o game.exe

./build/main.o: main.cpp
	g++ -I../extern -c main.cpp -o ./build/main.o

./build/animated_sprite.o: animated_sprite.cpp animated_sprite.h sprite.cpp sprite.h
	g++ -I../extern -c animated_sprite.cpp -o ./build/animated_sprite.o

./build/entity.o: entity.cpp entity.h animated_sprite.cpp animated_sprite.h
	g++ -I../extern -c entity.cpp -o ./build/entity.o

./build/graphics.o: graphics.cpp graphics.h
	g++ -I../extern -c graphics.cpp -o ./build/graphics.o

./build/player.o: player.cpp player.h entity.cpp entity.h
	g++ -I../extern -c player.cpp -o ./build/player.o

./build/sprite.o: sprite.cpp sprite.h graphics.cpp graphics.h
	g++ -I../extern -c sprite.cpp -o ./build/sprite.o

./build/tilemap.o: tilemap.cpp tilemap.h graphics.cpp graphics.h
	g++ -I../extern -c tilemap.cpp -o ./build/tilemap.o
