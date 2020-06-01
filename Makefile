CXX = g++
CXX_FLAGS = -g

all:
	$(CXX) $(CXX_FLAGS) src/*.cpp src/**/*.cpp -Iinclude -o renderer

clean:
	rm renderer
