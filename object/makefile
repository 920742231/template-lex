SOURCE = ../source
INCLUDE = ../include

vpath %.cpp $(SOURCE)
vpath %.hpp $(INCLUDE)

CPPFLAGS = -I$(INCLUDE)

all : test.o functions.o transtabl.o

test.o : transtabl.hpp lexanalyzer.hpp \
	transmachine.hpp srcfhandler.hpp functions.hpp

functions.o : 

transtabl.o : functions.hpp transtabl.hpp 

clean : 
	rm *.o