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

int main(int argc, char** argv) {
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    } else {
      printf("Unrecognized command '%s'.\n", input_buffer->buffer);
    }
  }
}
