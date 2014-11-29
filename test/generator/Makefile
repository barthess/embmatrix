INC = -I../../src -I../../osal
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

CFLAGS = -O0 -ggdb -std=gnu++11
CPPFLAGS = $(CFLAGS) $(CPPWARN) $(INC)

DEPS = ../../src/* ../../osal/*

all: matrix_test Makefile

matrix_test.cpp: gen.py $(DEPS)
	python gen.py

matrix_test: matrix_test.cpp
	$(CC) $(CPPFLAGS) matrix_test.cpp -o matrix_test

.PHONY: clean, test

clean:
	rm -f *.o matrix_test.cpp matrix_test

