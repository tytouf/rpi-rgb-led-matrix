# Select board between bbb and rpi
#BOARD=rpi
BOARD=bbb

CFLAGS=-Wall -O3 -g
CXXFLAGS=-Wall -O3 -g -D$(BOARD)
OBJECTS=main.o $(BOARD)_gpio.o led-matrix.o thread.o
BINARIES=led-matrix
LDFLAGS=-lrt -lm -lpthread

all : $(BINARIES)

led-matrix.o: led-matrix.cc led-matrix.h
main.o: led-matrix.h

led-matrix : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(BINARIES)
