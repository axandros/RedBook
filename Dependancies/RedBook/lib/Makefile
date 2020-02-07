CXXFLAGS  = -I ../include

all: libLoadShaders.a

libLoadShaders.a: LoadShaders.o
	ar rsv libLoadShaders.a LoadShaders.o

LoadShaders.o: ../include/LoadShaders.h

clean:
	rm *.o *.a
