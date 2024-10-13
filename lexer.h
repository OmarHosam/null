#pragma once
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

Token check_string(char current, FILE* p_file);
Token check_int_lit(char current, FILE* p_file);
Token tokenize_brackets(char current);
Token tokenize_semicolon(char current);
Token* lexer(FILE* p_file, int* length);
