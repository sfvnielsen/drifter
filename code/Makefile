CC=g++
CFLAGS=-c -Wall -std=c++11 -fexceptions -O3
LDFLAGS=-lm -std=c++11 -O3
SOURCES=release.cpp adjacency_list.cpp tree.cpp node.cpp sampler.cpp iofilehandler.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=drifter

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) -DNDEBUG $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *o $(EXECUTABLE)
