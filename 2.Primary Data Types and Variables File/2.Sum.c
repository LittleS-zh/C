/*
 *This code is for summing different type of variable.
 *Feel free to change "%0.xf" to "%d".
 */

#include <stdio.h>
int main() 
{ 
	float value1, value2, sum; 
	value1 = 50; 
	value2 = 25; 
	sum = value1 + value2;
	printf("The sum of %0.1f and %0.2f is %0.3f\n", value1, value2, sum); 
	return 0;
}  