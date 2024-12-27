COMPILER = g++
FLAGS = -O2 -lpthread
DEBUG = -D DEBUG

# Obiettivo di compilazione di tutti i file
all: hd

# Creazione dell'eseguibile hd
hd: hd.o main.o
	$(COMPILER) $(FLAGS) -o hd hd.o main.o

# Compilazione del file main.o
main.o: main.cpp hd.hpp
	$(COMPILER) $(FLAGS) -c main.cpp

# Compilazione del file hd.o
hd.o: hd.cpp hd.hpp
	$(COMPILER) $(FLAGS) -c hd.cpp

debug: hd.o main.o 
	$(COMPILER) $(FLAGS) $(DEBUG) -o debug hd.o main.o
	./debug || rm ./debug
	rm ./debug

# Rimozione dei file oggetto
remove:
	rm -f *.o

# Obiettivo principale
run: hd
	./hd