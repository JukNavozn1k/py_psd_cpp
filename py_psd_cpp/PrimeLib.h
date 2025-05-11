#ifndef PRIME_H
#define PRIME_H

#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#define MAX_VAL 18446744073709551615ULL

typedef enum {
    PRIME_OK,
    ERROR_NEGATIVE_NUMBER,
    ERROR_INVALID_INPUT,
    ERROR_NUMBER_TOO_LARGE,
    ERROR_ZERO_INVALID
} PrimeError;

EXPORT PrimeError is_prime(uint64_t n, bool* result);
EXPORT PrimeError gcd(uint64_t a, uint64_t b, uint64_t* result);
EXPORT PrimeError lcm(uint64_t a, uint64_t b, uint64_t* result);
EXPORT PrimeError sieve_of_eratosthenes(uint64_t limit, uint64_t** primes, uint64_t* count);
EXPORT PrimeError goldbach_conjecture(uint64_t n, uint64_t** result, uint64_t* size);
EXPORT PrimeError prime_factors(uint64_t n, uint64_t** factors, uint64_t* count);
EXPORT PrimeError prime_count(uint64_t n, uint64_t* count);
EXPORT PrimeError ferma_test(uint64_t n, bool* result);
EXPORT void free_array(uint64_t* array);

#endif