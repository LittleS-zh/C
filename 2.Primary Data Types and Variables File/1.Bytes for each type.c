/*This program is for checking how big of each type of variable*/

#include <stdio.h>
int main()
{
	int a = sizeof(char); 
	int b = sizeof(short);
	int c = sizeof(int);
	int d = sizeof(long);
	int e = sizeof(float);
	int f = sizeof(double);
	printf("%d\n%d\n%d\n%d\n%d\n%d\n",a,b,c,d,e,f);
	/*Number of bytes used to store a char type variable is 1
	Number of bytes used to store a short int type variable is 2
	Number of bytes used to store an int type variable is 4
	Number of bytes used to store a long int type variable is 4 
	Number of bytes used to store a float type variable is 4
	Number of bytes used to store a double type variable is 8 */
	return 0;
}