CC=gcc
CFLAGS=-std=c11 -lpthread -g

readers_and_writers: ps1_priority_readers_and_writers.c
	$(CC) ps1_priority_readers_and_writers.c -o ../bin/$@ $(CFLAGS)

clean: 
	rm -rf ../bin/*
