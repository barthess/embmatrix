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
CPPFLAGS = $(CFLAGS) $(CPPWARN)

DEPS = *.hpp *.h 

all: testsuite

testsuite: $(DEPS)

.PHONY: clean, test

clean:
	rm -f testsuite.o testsuite

test: testsuite
	./testsuite
