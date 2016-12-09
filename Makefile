CC=g++ -std=c++11
RM=rm -f

all: commsnode #test

commsnode:
	$(CC) -o bin/commsnode src/main.cpp  -Iinclude

#test:
#	$(CC) -o bin/test src/test/test_main.c -Iinclude

clean:
	$(RM) bin/commsnode #bin/test
