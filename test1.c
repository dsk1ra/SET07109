// Author: Simon Powers
// Date of last modification: 13/02/2023
// Test 1 for Coursework 1.

#include <stdio.h>

// Adds two integers.
// Parameters:
// x : int to add
// y : int to add
int add ( int a , int b )
{
	return a + b ;
}

// Subtract two integers.
// Parameters:
// x : int to subtract
// y : int to subtract
int sub ( int c , int d )
{
	return c - d ;
}

int main ( )
{
	int e = 1 ;
	int f = 2 ;
	int g = add ( e , f ) ;
	int h = sub ( e , f ) ;
	double z = 0.5 ;
	printf ( "%d\n" , g ) ;
	printf ( "%d\n" , h ) ;
	return 0 ;
}