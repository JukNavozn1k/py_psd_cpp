#ifndef PRIME_LIB_H
#define PRIME_LIB_H

#define MAX_VAL 1000000

typedef enum {
    PRIME_OK = 0,
    ERR_NEGATIVE,
    ERR_INVALID_INPUT,
    ERR_TOO_LARGE,
    ERR_UNKNOWN
} PrimeError;

typedef struct {
    int* data;
    int length;
    PrimeError error;
} IntArrayResult;

typedef struct {
    int result;
    PrimeError error;
} IntResult;

#ifdef __cplusplus
extern "C" {
#endif

    __declspec(dllexport) void free_array(int* arr);
    __declspec(dllexport) IntResult is_prime(int n);
    __declspec(dllexport) IntResult gcd(int a, int b);
    __declspec(dllexport) IntResult lcm(int a, int b);
    __declspec(dllexport) IntArrayResult sieve_of_eratosthenes(int limit);
    __declspec(dllexport) IntArrayResult goldbach_conjecture(int n);
    __declspec(dllexport) IntArrayResult prime_factors(int n);
    __declspec(dllexport) IntResult prime_count(int n);
    __declspec(dllexport) IntResult ferma_test(int n);

#ifdef __cplusplus
}
#endif

#endif