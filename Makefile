OPER_SYSTEM = linux

all: $(OPER_SYSTEM)

linux:
	g++ --std=c++17 main.cpp -lncursesw -o main