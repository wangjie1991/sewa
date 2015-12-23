#VAR = debug
VAR = release

GXX = g++
TARGET = sewa
OBJ = main.o desc.o eval.o syns.o util.o wave.o 

ifeq ($(VAR), debug)
CXXFLAGS = -g -O0 -c -Wall
else
CXXFLAGS = -O3 -c -Wall
endif

all : $(TARGET)
$(TARGET) : $(OBJ)
	$(GXX) $^ -o $@

%.o	: %.cc
	$(GXX) $(CXXFLAGS) $< -o $@

clean :	
	rm -f *.o
.PHONY : clean


