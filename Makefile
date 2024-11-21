CPPC=gcc
CODEDIRS=. src
INCDIRS=. include /usr/include

FLAGS=-Wall $(foreach D, $(INCDIRS), -I$D) -g
LIBFLAGS=-lm -lbsd
FILES=$(foreach D, $(CODEDIRS), $(wildcard $D/*.c))
BINARY=bin
OBJ=$(patsubst %.c,%.o,$(FILES))

all: $(BINARY)

$(BINARY): $(OBJ)
	$(CPPC) $^ -o $@ $(LIBFLAGS)
%.o: %.c
	$(CPPC) -c $(FLAGS) $(OP) -o $@ $^
clean:
	rm -r $(OBJ) $(BINARY)
run: all
	./bin
