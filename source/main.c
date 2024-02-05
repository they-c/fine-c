#include "lexer.h"
#include <stdio.h>

int main(void)
{
	char *a;
	while ((a = next_token())) {
		puts(a);
	}
}
