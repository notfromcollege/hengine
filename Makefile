all:
	g++ src/*.cpp src/*.c src/gameObjects/*.cpp src/imgui/*.cpp -o g -Iinclude -Iinclude/gameObjects -Iinclude/imgui -limgui -lglfw -lassimp -lGL -lX11 -lpthread -lXrandr -lXi -ldl
