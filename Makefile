all:
	make build
build:
	g++ -g all.cpp --std=c++11 -o runme
