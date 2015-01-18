CC=g++
CFLAGS=-c -Wall -std=c++11 -DNDEBUG -fexceptions -O2
LDFLAGS=
SOURCES=release.cpp adjacency_list.cpp barebones_tree.cpp node.cpp sampler.cpp iofilehandler.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=run_parmugit

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *o $(EXECUTABLE)
