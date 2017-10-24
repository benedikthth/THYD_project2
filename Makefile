CC = g++
CPPFLAGS = -std=c++11 -Wall -Wno-conversion
FILES = main.cpp hparser.cpp

OUT = compilers
FLEXER = decaf.l
FLEXOP = FlexLexer.cpp

all:
		flex --outfile=$(FLEXOP) $(FLEXER)
		$(CC) $(CPPFLAGS) $(FILES) $(FLEXOP) -o $(OUT)

clean:
		rm -f *.o

distclean:
		rm -f $(OUT)
