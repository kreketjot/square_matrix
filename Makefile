file: main
	g++ *.o -o runMe -g

main: matrix
	g++ -c main.cpp -o main.o -g

matrix: 
	g++ -c TSquareMatrix.cpp -o TSquareMatrix.o -g
