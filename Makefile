CC = g++
CPPFLAGS = -std=c++11 -Wall -Wno-conversion
FILES = main.cpp hparser.cpp

OUT = parser
FLEXER = decaf.l
FLEXOP = FlexLexer.cpp
BISIN = decaf.yy
BISOP = parser_decaf.cpp\

all:
		flex --outfile=$(FLEXOP) $(FLEXER)
		bison -o $(BISOP) $(BISIN)
		$(CC) $(CPPFLAGS) $(FILES) $(FLEXOP) $(BISOP) -o $(OUT)

clean:
		rm -f *.hh
		rm -f *.o

distclean:
		rm -f $(OUT)
