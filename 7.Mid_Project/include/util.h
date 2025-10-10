/**
 * @file util.h
 * @brief Utility macros and helper definitions for the P2P chat application.
 *
 * This header provides general-purpose macros for error handling
 * and system-level utility operations.
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#endif /* UTIL_H */
