
#include "utils.h"

void perro(const char* str_error) {
    fprintf(stderr, "%s", str_error);
}

void perro_exit(const char* str_error, int error_no) {
    perro(str_error);
    exit(error_no);
}