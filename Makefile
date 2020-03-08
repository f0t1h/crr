
OPT ?=
all:
	@echo '#include ".crr.h"' | cat - example.crr > .crr.cpp
	@g++ ${OPT} .crr.cpp -o plot_crr -std=c++17
	@echo g++ ${OPT} crr.cpp -o plot_crr -std=c++17
	@rm .crr.cpp
