#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  KEYWORD,
  INT_LITERAL,
  SEMICOLON,
  OPEN_BRACKET,
  CLOSE_BRACKET
} TokenType;

typedef struct {
  TokenType type;
  char *value;
} Token;

void print_usage() { printf("Usage: null <filename.nl>\n"); }

Token check_string(char current, FILE *p_file) {
  char* buffer = malloc(sizeof(char) * 16);
  int i = 0;
  Token l_token;

  while (current != EOF && isalpha(current)) {
    buffer[i] = current;
    current = fgetc(p_file);
    i++;
  }

  if (strcmp(buffer, "exit") == 0) {
    l_token.type = KEYWORD;
    l_token.value = "exit";
  }

  ungetc(current, p_file); // One step back.

  return l_token;
}

Token check_int_lit(char current, FILE *p_file) {
  char *value = malloc(sizeof(char) * 8);
  int i = 0;
  Token l_token;

  l_token.type = INT_LITERAL;

  while (current != EOF && isdigit(current)) {
    if (!isdigit(current)) break;
    value[i] = current;
    current = fgetc(p_file);
    i++;
  }

  l_token.value = value;

  ungetc(current, p_file); // One step back.

  return l_token;
}

Token tokenize_brackets(char current) {
  Token l_token;
  if (current == '(') {
    l_token.type = OPEN_BRACKET;
    l_token.value = "(";
  }

  if (current == ')') {
    l_token.type = CLOSE_BRACKET;
    l_token.value = ")";
  }

  return l_token;
}

Token tokenize_semicolon(char current) {
  Token l_token;

  l_token.type = SEMICOLON;
  l_token.value = ";";

  return l_token;
}

void print_token(Token token){
  printf("Token value: '%s'\t", token.value);
  switch (token.type) {
    case (INT_LITERAL):
      printf("Token type: INT\n");
      break;
    case (KEYWORD):
      printf("Token type: KEYWORD\n");
      break;
    case (SEMICOLON):
      printf("Token type: SEMICOLON\n");
      break;
    case (OPEN_BRACKET):
      printf("Token type: OPEN_BRACKET\n");
      break;
    case (CLOSE_BRACKET):
      printf("Token type: CLOSE_BRACKET\n");
      break;
  }
}

int main(int argc, char **argv) {
  if (!argv[1]) {
    printf("Wrong usage!\n");
    print_usage();
    exit(1);
  }

  FILE *p_file;
  char current;
  Token *tokens = malloc(sizeof(Token) * 8);

  p_file = fopen(argv[1], "r");
  if (!p_file) {
    printf("This file is not found!\n");
    print_usage();
    exit(1);
  }

  int token_index = 0;
  current = fgetc(p_file);

  while (current != EOF) {
    // printf("%c\n", current);
    Token curr_token;

    if (isalpha(current)) {
      curr_token = check_string(current, p_file);
      tokens[token_index] = curr_token;
      token_index++;
    }

    if (isdigit(current)) {
      curr_token = check_int_lit(current, p_file);
      tokens[token_index] = curr_token;
      token_index++;
    }

    if (current == ')' || current == '(') {
      curr_token = tokenize_brackets(current);
      tokens[token_index] = curr_token;
      token_index++;
    }

    if (current == ';') {
      curr_token = tokenize_semicolon(current);
      tokens[token_index] = curr_token;
      token_index++;
    }

    current = fgetc(p_file);
  }

  for (int i = 0; i < 5; i++) {
    print_token(tokens[i]);
  }

  // Cleanup.
  fclose(p_file);
  free(tokens);

  return 0;
}
