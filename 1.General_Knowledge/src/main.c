#include <stdio.h>
#include "strutils.h"

int main() {
    char s1[] = "Hello World";
    char s2[] = "   trim me   ";
    char s3[] = "12345";
    char s4[] = "12a34";

    printf("=== Test str_reverse ===\n");
    printf("Original: %s\n", s1);
    str_reverse(s1);
    printf("Reversed: %s\n\n", s1);

    printf("=== Test str_trim ===\n");
    printf("Original: '%s'\n", s2);
    str_trim(s2);
    printf("Trimmed: '%s'\n\n", s2);

    printf("=== Test str_to_int ===\n");
    int num;
    if (str_to_int(s3, &num))
        printf("String '%s' -> %d\n", s3, num);
    else
        printf("Failed to convert '%s'\n", s3);

    if (str_to_int(s4, &num))
        printf("String '%s' -> %d\n", s4, num);
    else
        printf("Failed to convert '%s'\n", s4);

    return 0;
}
