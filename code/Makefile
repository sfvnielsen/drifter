CC=g++
CFLAGS=-c -Wall -std=c++11 -DNDEBUG -fexceptions -O3
LDFLAGS=-lm -std=c++11 -O3
SOURCES=release.cpp adjacency_list.cpp tree.cpp node.cpp sampler.cpp iofilehandler.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=run_parmugit

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *o $(EXECUTABLE)
