CC=g++ -std=c++23 -O2
CONFIG_FILE=config.json

CPPFILES=$(wildcard src/*.cpp)

run: src/main
	./src/main < $(CONFIG_FILE)

src/main: $(CPPFILES)
	$(CC) -o $@ $(CPPFILES)
