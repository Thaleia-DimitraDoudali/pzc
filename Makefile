CC=gcc
CXX=g++

COMCFLAGS = -Wall -MP -MD
COMCFLAGS += -O0 -g3

CFLAGS = $(COMCFLAGS) #-std=c99

CXXFLAGS = $(COMCFLAGS) -std=c++11

CFILES = \
	lexer.c \
	parser.c \
	#parser_interface.c \

CXXFILES = \
	new_parser_interface.cpp \
	scope.cpp \
	types.cpp \
	error.cpp
	#parser_constants.cpp \
	#parser_statements.cpp \
	#hashtable.cpp \

YFILES = parser.y

SRCS=$(CFILES) $(CXXFILES) $(YFILES)

OFILES=$(CFILES:.c=.o) $(CXXFILES:.cpp=.o)
DEPS=$(CFILES:.c=.d) $(CXXFILES:.cpp=.d)

all: pazcal

TAGS: $(SRCS)
	etags $(SRCS)

%.o : %.c parser.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cpp parser.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

lexer.c: lexer.l
	flex -s -o $@ $<

parser.c parser.h: parser.y
	bison -v -d -oparser.c $<

pazcal: $(OFILES)
	$(CXX)  $(CXXFLAGS) $^ -o $@ -lfl

.PHONY: clean distclean

clean:
	$(RM) lexer.c parser.c parser.h *.o *.output *~ $(DEPS) $(OFILES) TAGS

distclean: clean
	$(RM) lexer

-include $(DEPS)
