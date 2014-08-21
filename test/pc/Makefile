INC = -I../../include -I../mcu/common
CC = g++
CPPWARN = -Wall \
		  -Wextra \
		  -Wformat \
		  -Woverloaded-virtual \
		  -Wctor-dtor-privacy  \
		  -Winit-self \
		  -Wunreachable-code \
		  -Wold-style-cast \
		  -Wnon-virtual-dtor 

CFLAGS = -O2 -ggdb -std=gnu++11
CPPFLAGS = $(CFLAGS) $(CPPWARN) $(INC)

DEPS = $(INCDIR)*.hpp $(INCDIR)*.h 

all: testsuite Makefile

testsuite: $(INCDIR)

.PHONY: clean, test

clean:
	rm -f testsuite.o testsuite

test: testsuite
	./testsuite
