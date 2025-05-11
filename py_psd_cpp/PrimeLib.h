#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    PRIME_OK = 0,
    ERR_NEGATIVE,
    ERR_INVALID_INPUT,
    ERR_TOO_LARGE,
    ERR_ALLOCATION_FAILED,
    ERR_NO_SOLUTION
} PrimeError;

__declspec(dllexport) bool is_prime(uint64_t n, PrimeError* err);
__declspec(dllexport) uint64_t gcd(uint64_t a, uint64_t b, PrimeError* err);
__declspec(dllexport) uint64_t lcm(uint64_t a, uint64_t b, PrimeError* err);
__declspec(dllexport) uint64_t* sieve_of_eratosthenes(uint64_t limit, uint64_t* count, PrimeError* err);
__declspec(dllexport) uint64_t* goldbach_conjecture(uint64_t n, uint64_t* count, PrimeError* err);
__declspec(dllexport) uint64_t* prime_factors(uint64_t n, uint64_t* count, PrimeError* err);
__declspec(dllexport) uint64_t prime_count(uint64_t n, PrimeError* err);
__declspec(dllexport) bool ferma_test(uint64_t n, PrimeError* err);
__declspec(dllexport) void free_array(uint64_t* arr);