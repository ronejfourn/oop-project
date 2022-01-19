./bin/game.exe: ./build/main.o ./build/animated_sprite.o ./build/entity.o ./build/graphics.o ./build/player.o ./build/sprite.o ./build/tilemap.o ./build/input.o
	g++ ./build/*.o -lSDL2 -lSDL2main -o ./build/game.exe -O2

./build/main.o: main.cpp
	g++ -I../extern -c main.cpp -o ./build/main.o -O2

./build/animated_sprite.o: animated_sprite.cpp animated_sprite.h sprite.cpp sprite.h
	g++ -I../extern -c animated_sprite.cpp -o ./build/animated_sprite.o -O2

./build/entity.o: entity.cpp entity.h animated_sprite.cpp animated_sprite.h
	g++ -I../extern -c entity.cpp -o ./build/entity.o -O2

./build/graphics.o: graphics.cpp graphics.h
	g++ -I../extern -c graphics.cpp -o ./build/graphics.o -O2

./build/player.o: player.cpp player.h entity.cpp entity.h
	g++ -I../extern -c player.cpp -o ./build/player.o -O2

./build/sprite.o: sprite.cpp sprite.h graphics.cpp graphics.h
	g++ -I../extern -c sprite.cpp -o ./build/sprite.o -O2

./build/tilemap.o: tilemap.cpp tilemap.h graphics.cpp graphics.h
	g++ -I../extern -c tilemap.cpp -o ./build/tilemap.o -O2

./build/input.o: input.cpp input.h
	g++ -I../extern -c input.cpp -o ./build/input.o -O2
