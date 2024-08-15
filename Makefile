### __PROJECT_MAKEFILE__ ###

CC = g++
# compiling in 64 bits
CFLAGS = -maes -msse4 -m64 -O3 -std=c++11
LDFLAGS = -maes -msse4 -m64 -O3 -std=c++11
EXEC = bin/sm-aes.exe
BENCHMARK = bin/benchmark.exe

all : $(EXEC)

$(EXEC): main.o FastAES.o
		$(CC) -o $(EXEC) $^ $(LDFLAGS)

benchmark: FastAES.o benchmark.o
	$(CC) -o $(BENCHMARK) $^ $(LDFLAGS)

main.o:	src/main.cpp
		$(CC) -c $< $(CFLAGS)

FastAES.o: src/FastAES.cpp
		$(CC) -c $< $(CFLAGS)

benchmark.o: src/benchmark.cpp
	$(CC) -c $< $(CFLAGS)

clean:
		del *.o

mrproper: clean 
		del -f $(EXEC)