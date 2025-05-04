#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_VAL 18446744073709551615ULL

	// Basic arithmetic functions
	extern __declspec(dllexport) bool is_prime(uint64_t n, bool* result, const char** error);
	extern __declspec(dllexport) bool gcd(uint64_t a, uint64_t b, uint64_t* result, const char** error);
	extern __declspec(dllexport) bool lcm(uint64_t a, uint64_t b, uint64_t* result, const char** error);

	// Sieve and number theory
	extern __declspec(dllexport) bool sieve_of_eratosthenes(uint64_t limit, uint64_t** primes, size_t* count, const char** error);
	extern __declspec(dllexport) bool goldbach_conjecture(uint64_t n, uint64_t** pair, size_t* count, const char** error);
	extern __declspec(dllexport) bool prime_factors(uint64_t n, uint64_t** factors, size_t* count, const char** error);
	extern __declspec(dllexport) bool prime_count(uint64_t n, uint64_t* count, const char** error);
	extern __declspec(dllexport) bool ferma_test(uint64_t n, bool* result, const char** error);

	// Memory management
	extern __declspec(dllexport) void free_array(uint64_t* array);

#ifdef __cplusplus
}
#endif