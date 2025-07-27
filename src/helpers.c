#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>

void leave(const char *const message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
