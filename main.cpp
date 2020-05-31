#include "TSquareMatrix.h"
#include <cstdio>

int main()
{
	TSquareMatrix A, B, C;
	unsigned** indexes, size = 2, cnt, precision = 1;
	double** arr, value = 1;

	//create and init array
	arr = new double*[size];
	arr[0] = new double[size];
	arr[1] = new double[size];
	arr[0][0] = 1;
	arr[0][1] = 1;
	arr[1][0] = 2;
	arr[1][1] = 3;
	//init matrix
	A.setSize(size);
	A.fill(arr, size);
	delete[] arr[0];
	delete[] arr[1];
	delete[] arr;
	
	B = -A;
	puts("\nmatrix A:");
	A.printValues(precision);
	puts("\nmatrix B = -A:");
	B.printValues(precision);
	puts("\nresult of A+B:");
	(A+B).printValues(precision);
	puts("\nresult of A-B:");
	(A-B).printValues(precision);
	puts("\nresult of A*B:");
	(A*B).printValues(precision);
	puts("\nresult of A/B:");
	(A/B).printValues(precision);
	puts("\nresult of inverting A is C:");
	C = A.getInversed();
	C.printValues(precision);
	puts("\nresult of A*C");
	(A*C).printValues(precision);
	printf("\ndeterminant A =  %.*f\n", precision, A.getDeterminant());
	puts("\nresult of 2*A:");
	(2*A).printValues(precision);
	printf("\nA(1, 1) = %.*f\n", precision, A[1][1]);
	A[1][1] = 6;
	printf("changed A(1, 1) = %.*f\n", precision, A[1][1]);
	if(A.findElementByValue(value, &cnt, &indexes) != OK)
	{
		perror("\nThe following error occurred");
		return -1;
	}
	if(cnt)
		printf("\nvalue = %.3f in A with indexes:\n", value);
	else
		printf("\nthere is no value = %.*f in A\n", precision, value);
	for(unsigned i=0; i<cnt; i++)
	{
		printf("i = %u, j = %u\n", indexes[i][0], indexes[i][1]);
		delete[] indexes[i];
	}
	delete[] indexes;
	puts("");

	return 0;
}
