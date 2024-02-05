#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static char currc = ' ';
static char nextc = ' ';

static char popc(void)
{
	int next_char = getchar();
	currc = nextc;
	nextc = next_char == EOF? '\0' : next_char;
	return currc;
}


static void crash(char *message)
{
	fputs("ERROR inside Lexer:\n", stderr);
	fputs(message, stderr);
	fputs("\n", stderr);
	exit(1);
}

static void skip_ws(void)
{
	while (nextc && isspace(nextc))
		popc();
}

static void conc(char **s, size_t *size, char c)
{
	(*size)++;
	*s = realloc(*s, *size);
	(*s)[*size - 1] = c;
}

static char *scan_id(void)
{
	size_t size = 1;
	char *token = realloc(NULL, size);
	token[0] = currc;
	while (isalnum(nextc) || nextc == '_') {
		popc();
	        conc(&token, &size, currc);
	}
	conc(&token, &size, '\0');
	return token;
}

static char *scan_number_end(char **token, size_t *size)
{
	if (isdigit(currc))
		conc(token, size, currc);
	while (nextc && isdigit(nextc)) {
		popc();
		conc(token, size, currc);
	}
	conc(token, size, '\0');
	return *token;
}

static char *scan_number(void)
{
	size_t size = 1;
	char *token = realloc(NULL, size);
	token[0] = currc;
	if (currc == '0') {
		popc();
		if (currc == 'b' || currc == 'x' || currc == '.') {
			conc(&token, &size, currc);
			popc();
		} else if (!isdigit(currc)) {
			crash("Invalid character inside number.");
		}
		return scan_number_end(&token, &size);
	}
	return NULL;
}

static char *scan_string(void)
{
	size_t size = 1;
	char *token = realloc(NULL, size);
	token[0] = currc;
	popc();
	while (currc != '"') {
		conc(&token, &size, currc);
		popc();
	}
	conc(&token, &size, '\0');
	return token;
}

static char *scan_char(void)
{
	size_t size = 3;
	char *token = realloc(NULL, size);
	token[0] = currc;
	popc();
	token[1] = currc;
	popc();
	token[2] = '\0';
	return token;
}

static char *scan_op(void)
{
	size_t size = 1;
	char *token = realloc(NULL, size);
	token[0] = currc;
        if (currc == nextc) {
		while (nextc && currc == nextc) {
			if (currc == '{' || currc == '}' || currc == '(' || currc == ')')
				break;
			conc(&token, &size, popc());
		}
	} else if (nextc == '=') {
		conc(&token, &size, popc());
	}
	conc(&token, &size, '\0');
	return token;
}

static char *after_ws(void)
{
	popc();
	if (isalpha(currc) || currc == '_')
		return scan_id();
	if (isdigit(currc))
		return scan_number();
	if (currc == '"')
		return scan_string();
	if (currc == '\'')
		return scan_char();
	return scan_op();
}

char *next_token(void)
{
        skip_ws();
	if (nextc)
		return after_ws();
	return NULL;
}
