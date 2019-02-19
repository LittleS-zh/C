/*The relation between "char" and "int"*/

#include <stdio.h>
int main()
{	
	char letter;
	int letter_in_ascii,a,b;//a,b means the interger "letter_in_ascii" plus one and minus one
	printf("please input an English letter:\n");
	scanf("%c",&letter);
	letter_in_ascii = int(letter);
	a = letter_in_ascii - 1;
	b = letter_in_ascii + 1;
	printf("%c and its neigbors in ASCII:\n",letter);
	printf("%c	%c	%c\n%d	%d	%d\n",a,letter_in_ascii,b,a,letter_in_ascii,b);
	return 0;
}