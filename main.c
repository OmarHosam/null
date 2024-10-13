#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "lexer.h"
#include "parser.h"

void print_usage(char* loc) { printf("Usage: %s <filename.nl>\n", loc); }

void print_token(Token token) {
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

void print_tokens(Token* tokens, int* length) {
  for (int i = 0; i < *length; i++) {
    print_token(tokens[i]);
  }
}

void free_tokens(Token* tokens, int length) {
  for (int i = 0; i < length; i++) {
    if (tokens[i].type == INT_LITERAL) {
      free(tokens[i].value);
    }
  }

  free(tokens);
}

int main(int argc, char **argv) {
  if (!argv[1]) {
    printf("Wrong usage!\n");
    print_usage(argv[0]);
    exit(1);
  }

  FILE *p_file;
  int length;

  p_file = fopen(argv[1], "r");
  if (!p_file) {
    printf("'%s' is not found!\n", argv[1]);
    print_usage(argv[0]);
    exit(1);
  }

  Token* tokens = lexer(p_file, &length);
  tokens = parse(tokens, &length);

  print_tokens(tokens, &length);

  // Cleanup.
  fclose(p_file);
  free_tokens(tokens, length);

  return 0;
}
