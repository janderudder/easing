#pragma once
#include <iostream>


/**
 *  Print expression, then evaluate it and print result.
 */
#define ECHO(X)                             \
    ([&] {                                  \
        std::cout                           \
            << (#X) << " : "                \
            << (X);                         \
    })()


/**
 *  Print expression, evaluate it and print result, then print a new line.
 */
#define ECHO_LN(X)                          \
    ([&] {                                  \
        std::cout                           \
            << (#X) << " : "                \
            << (X) << "\n";                 \
    })()


/**
 *  Print a new line
 */
#define NL(Stream)                          \
    ([](std::ostream& os = std::cout) {     \
        os << "\n";                         \
    }(Stream))
