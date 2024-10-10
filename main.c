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
  char buffer[16];
  int i = 0;
  Token token = {0};

  while (current != EOF && isalpha(current)) {
    buffer[i++] = current;
    current = fgetc(p_file);
  }
  buffer[i] = '\0';

  if (strcmp(buffer, "exit") == 0) {
    token.type = KEYWORD;
    token.value = "exit";
  }
  ungetc(current, p_file); // Move back one step
  return token;
}

Token check_int_lit(char current, FILE *p_file) {
  char buffer[8];
  int i = 0;
  Token token = {INT_LITERAL, NULL};

  while (current != EOF && isdigit(current)) {
    buffer[i++] = current;
    current = fgetc(p_file);
  }
  buffer[i] = '\0';

  // Because the buffer is getting freed after the function exits,
  // it's better to duplicate the string. That way, we can avoid undefined behavior.
  token.value = strdup(buffer);
  ungetc(current, p_file); // Move back one step
  return token;
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

  int capacity = 8;
  Token *tokens = malloc(sizeof(Token) * capacity);

  p_file = fopen(argv[1], "r");
  if (!p_file) {
    printf("This file is not found!\n");
    print_usage();
    exit(1);
  }

  int token_index = 0;
  current = fgetc(p_file);

  while (current != EOF) {
    Token curr_token;

    // Thanks ChatGPT for the refactoring!
    if (isalpha(current)) {
      curr_token = check_string(current, p_file);
    } else if (isdigit(current)) {
      curr_token = check_int_lit(current, p_file);
    } else if (current == ')' || current == '(') {
      curr_token = tokenize_brackets(current);
    } else if (current == ';') {
      curr_token = tokenize_semicolon(current);
    } else {
      current = fgetc(p_file);
      continue;
    }

    if (token_index >= capacity) {
      capacity *= 2;
      tokens = realloc(tokens, sizeof(Token) * capacity);
      if (tokens == NULL) {
        printf("Lexer Error: failed to reallocate more memory for the tokens.\n");
      }
    }

    tokens[token_index++] = curr_token;
    current = fgetc(p_file);
  }

  for (int i = 0; i < token_index; i++) {
    print_token(tokens[i]);
  }

  // Cleanup.
  fclose(p_file);
  free(tokens);

  return 0;
}
