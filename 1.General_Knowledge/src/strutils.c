#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "strutils.h"

void str_reverse(char *str) {
    if (!str) return;
    int i = 0, j = strlen(str) - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

void str_trim(char *str) {
    if (!str) return;
    int start = 0, end = strlen(str) - 1;

    while (isspace((unsigned char)str[start])) start++;

    while (end >= start && isspace((unsigned char)str[end])) end--;

    int len = end - start + 1;
    memmove(str, str + start, len);
    str[len] = '\0';
}

int str_to_int(const char *str, int *out) {
    if (!str || !out) return 0;

    char *endptr;
    long val = strtol(str, &endptr, 10);

    // kiểm tra lỗi
    if (*endptr != '\0') return 0;

    *out = (int)val;
    return 1;
}
