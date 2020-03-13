
OPT ?=
all:
	@g++ ${OPT} example.cpp -o plot_crr -std=c++17
