game: main.o animated_sprite.o entity.o graphics.o player.o sprite.o tilemap.o
	g++ *.o -lSDL2 -lSDL2main -o game

main.o: main.cpp
	g++ -I../extern -c main.cpp -o main.o

animated_sprite.o: animated_sprite.cpp
	g++ -I../extern -c animated_sprite.cpp -o animated_sprite.o

entity.o: entity.cpp
	g++ -I../extern -c entity.cpp -o entity.o

graphics.o: graphics.cpp
	g++ -I../extern -c graphics.cpp -o graphics.o

player.o: player.cpp
	g++ -I../extern -c player.cpp -o player.o

sprite.o: sprite.cpp
	g++ -I../extern -c sprite.cpp -o sprite.o

tilemap.o: tilemap.cpp
	g++ -I../extern -c tilemap.cpp -o tilemap.o
