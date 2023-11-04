all:
	g++ src/*.cpp src/*.c src/imgui/*.cpp -o g -Iinclude -Iinclude/imgui -limgui -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
