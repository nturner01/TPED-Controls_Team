all: demo.cpp classes.h inputs.h
	@g++ demo.cpp
	@./a.out
	@rm a.out