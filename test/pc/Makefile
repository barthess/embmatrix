INC = ../../include
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

CFLAGS = -O0 -ggdb
CPPFLAGS = $(CFLAGS) $(CPPWARN) -I$(INC)

DEPS = $(INCDIR)*.hpp $(INCDIR)*.h 

all: testsuite

testsuite: $(INCDIR)

.PHONY: clean, test

clean:
	rm -f testsuite.o testsuite

test: testsuite
	./testsuite
