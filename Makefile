CC=gcc
CFLAGS=-c -Wall -Werror
SOURCES= strings/stringext.c filereader/reader.c exception/exception.c lexer/lexer.c token/token.c parser/parser.c main.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=geometry.out

build: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf geometry.out *.o */*.o

run:
	./geometry.out $(path)