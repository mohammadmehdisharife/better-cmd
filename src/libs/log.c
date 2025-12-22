#include <stdio.h>
#include <stdlib.h>
#include "./ansi_colors.h"

void error_log(char *msg) {
  fprintf(stderr, ANSI_BOLD_RED "%s\n" ANSI_RESET, msg);
  exit(1);
}
