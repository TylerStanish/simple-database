#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct InputBuffer {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer() {
  InputBuffer* in_buf = malloc(sizeof(InputBuffer));
  in_buf->buffer = NULL;
  in_buf->buffer_length = 0;
  /* the input length does not include the '\0' byte */
  in_buf->input_length = 0;
  return in_buf;
}

void print_prompt() {
  printf(">");
}

void read_input(InputBuffer* buf) {
  ssize_t bytes_read = getline(&buf->buffer, &buf->buffer_length, stdin);
  if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }
  // trash newline character
  buf->input_length = bytes_read - 1;
  buf->buffer[bytes_read - 1] = '\0';
}

void close_input_buffer(InputBuffer* buf) {
  free(buf->buffer);
  buf->buffer = NULL;
  free(buf);
  buf = NULL;
}

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED,
} MetaCommandResult;

MetaCommandResult do_meta_command(InputBuffer* buf) {
  if (strcmp(buf->buffer, ".exit") == 0) {
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED;
  }
}

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_STATEMENT_UNRECOGNIZED,
} PrepareResult;

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT,
} StatementType;

typedef struct {
  StatementType type;
} Statement;

PrepareResult prepare_statement(InputBuffer* buf, Statement* statement) {
  if (strncmp(buf->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strncmp(buf->buffer, "select", 6) == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  return PREPARE_STATEMENT_UNRECOGNIZED;
}

void execute_statement(Statement* statement) {
  switch (statement->type) {
    case STATEMENT_SELECT:
      printf("doing a select...\n");
      break;
    case STATEMENT_INSERT:
      printf("doing an insert...\n");
      break;
  }
}

int main(int argc, char** argv) {
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);
    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
        case META_COMMAND_SUCCESS:
          continue;
        case META_COMMAND_UNRECOGNIZED:
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    }
    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case PREPARE_SUCCESS:
        break;
      case PREPARE_STATEMENT_UNRECOGNIZED:
        printf("Unrecognized keyword at the start of '%s'\n", input_buffer->buffer);
        continue;
    }
    execute_statement(&statement);
  }
}
