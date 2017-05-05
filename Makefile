CXX=g++
LD=g++
DIR=src/
CFLAGS=-Wall -pedantic -Wno-long-long -O0 -ggdb --std=c++11

all: compile doc

auxiliary.o: $(DIR)auxiliary.cpp $(DIR)auxiliary.h
	$(CXX) $(CFLAGS) -c -o $@ $<

main.o: $(DIR)main.cpp $(DIR)server.h $(DIR)polymorph.h $(DIR)auxiliary.h
	$(CXX) $(CFLAGS) -c -o $@ $<

polymorph.o: $(DIR)polymorph.cpp $(DIR)polymorph.h $(DIR)auxiliary.h
	$(CXX) $(CFLAGS) -c -o $@ $<

server.o: $(DIR)server.cpp $(DIR)server.h $(DIR)polymorph.h
	$(CXX) $(CFLAGS) -c -o $@ $<

compile: auxiliary.o main.o polymorph.o server.o
	$(LD) -o karasma3 auxiliary.o main.o polymorph.o server.o 

run:
	./karasma3

clean:
	rm -f karasma3 auxiliary.o main.o polymorph.o server.o
	rm -rf doc
 
doc:
	doxygen Doxyfile

