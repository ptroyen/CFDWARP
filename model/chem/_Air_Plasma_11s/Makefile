include ../../../.makefile-header

LFLAGSLOCAL =  
CCFLAGSLOCAL = $(CFLAGSCFD) -I../../../lib/include -I../../../.

####### Files

HEADERS =   chem.h dunnkang1973.h park1993.h boyd2007.h lenard1964.h farbar2013.h parent2021.h
SOURCES =   chem.c dunnkang1973.c park1993.c boyd2007.c lenard1964.c farbar2013.c parent2021.c
OBJECTS =   chem.o dunnkang1973.o park1993.o boyd2007.o lenard1964.o farbar2013.o parent2021.o
TARGETS  = ../_chem.a


####### Build rules

all: src 

src: $(OBJECTS) 

arc: 
	( rm -f $(TARGETS) ; ar cqs $(TARGETS) *.o )                        

cleanall:
	( cd doc ; make clean )
	( make clean )

	

# DO NOT DELETE THIS LINE -- make depend depends on it.
