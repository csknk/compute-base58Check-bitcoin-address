SOURCES:= base58.cpp base58.h
OBJECTS:= base58.cpp
OUT:= bin

all: main p2sh

main: $(SOURCES) main.cpp
	g++ -W -Wall -std=c++17  -g -o $(OUT)/$@ $(OBJECTS) main.cpp -lssl -lcrypto

p2sh: $(SOURCES) p2sh.cpp
	g++ -W -Wall -std=c++17  -g -o $(OUT)/$@ $(OBJECTS) p2sh.cpp -lssl -lcrypto

debug: $(SOURCES)
	g++ -W -Wall -std=c++17 -g -o $(OUT) $(OBJECTS)
