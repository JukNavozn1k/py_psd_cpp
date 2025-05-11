#include "PrimeLib.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_VAL UINT64_MAX

// Быстрая проверка на простоту
__declspec(dllexport) bool is_prime(uint64_t n, PrimeError* err) {
    if (n > MAX_VAL) { *err = ERR_TOO_LARGE; return false; }
    if (n <= 1) { *err = PRIME_OK; return false; }
    if (n <= 3) { *err = PRIME_OK; return true; }
    if (n % 2 == 0 || n % 3 == 0) { *err = PRIME_OK; return false; }

    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            *err = PRIME_OK;
            return false;
        }
    }
    *err = PRIME_OK;
    return true;
}

// Бинарный алгоритм GCD
__declspec(dllexport) uint64_t gcd(uint64_t a, uint64_t b, PrimeError* err) {
    if (a == 0 || b == 0) { *err = ERR_INVALID_INPUT; return 0; }

    int shift = 0;
    while (((a | b) & 1) == 0) {
        a >>= 1;
        b >>= 1;
        shift++;
    }

    while ((a & 1) == 0) a >>= 1;

    do {
        while ((b & 1) == 0) b >>= 1;
        if (a > b) { uint64_t t = b; b = a; a = t; }
        b -= a;
    } while (b != 0);

    *err = PRIME_OK;
    return a << shift;
}

// LCM через GCD
__declspec(dllexport) uint64_t lcm(uint64_t a, uint64_t b, PrimeError* err) {
    PrimeError tmp_err;
    uint64_t g = gcd(a, b, &tmp_err);
    if (tmp_err != PRIME_OK) { *err = tmp_err; return 0; }
    if (a == 0 || b == 0) { *err = ERR_INVALID_INPUT; return 0; }
    *err = PRIME_OK;
    return (a / g) * b;
}

// Оптимизированное решето Эратосфена
__declspec(dllexport) uint64_t* sieve_of_eratosthenes(uint64_t limit, uint64_t* count, PrimeError* err) {
    if (limit > MAX_VAL) { *err = ERR_TOO_LARGE; return NULL; }
    if (limit < 2) { *count = 0; *err = PRIME_OK; return NULL; }

    bool* sieve = calloc(limit + 1, sizeof(bool));
    if (!sieve) { *err = ERR_ALLOCATION_FAILED; return NULL; }

    for (uint64_t i = 2; i * i <= limit; i++) {
        if (!sieve[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = true;
        }
    }

    uint64_t cnt = 0;
    for (uint64_t i = 2; i <= limit; i++)
        if (!sieve[i]) cnt++;

    uint64_t* primes = malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(sieve); *err = ERR_ALLOCATION_FAILED; return NULL; }

    for (uint64_t i = 2, j = 0; i <= limit; i++) {
        if (!sieve[i]) primes[j++] = i;
    }

    free(sieve);
    *count = cnt;
    *err = PRIME_OK;
    return primes;
}

// Гипотеза Гольдбаха
__declspec(dllexport) uint64_t* goldbach_conjecture(uint64_t n, uint64_t* count, PrimeError* err) {
    if (n % 2 != 0 || n <= 2) { *err = ERR_INVALID_INPUT; return NULL; }

    PrimeError tmp_err;
    uint64_t sieve_cnt;
    uint64_t* primes = sieve_of_eratosthenes(n, &sieve_cnt, &tmp_err);
    if (tmp_err != PRIME_OK) { *err = tmp_err; return NULL; }

    for (uint64_t i = 0; i < sieve_cnt; i++) {
        bool is_p = is_prime(n - primes[i], &tmp_err);
        if (tmp_err != PRIME_OK) { free(primes); *err = tmp_err; return NULL; }
        if (is_p) {
            uint64_t* pair = malloc(2 * sizeof(uint64_t));
            if (!pair) {
                free(primes);
                *err = ERR_ALLOCATION_FAILED;
                return NULL;
            }
            pair[0] = primes[i];
            pair[1] = n - primes[i];
            free(primes);
            *count = 2;
            *err = PRIME_OK;
            return pair;
        }
    }

    free(primes);
    *err = ERR_NO_SOLUTION;
    return NULL;
}

// Факторизация
__declspec(dllexport) uint64_t* prime_factors(uint64_t n, uint64_t* count, PrimeError* err) {
    if (n == 0) { *err = ERR_INVALID_INPUT; return NULL; }

    uint64_t* factors = malloc(64 * sizeof(uint64_t));
    if (!factors) { *err = ERR_ALLOCATION_FAILED; return NULL; }

    uint64_t idx = 0;
    uint64_t divisors[] = { 2, 3 };

    for (int i = 0; i < 2; i++) {
        while (n % divisors[i] == 0) {
            factors[idx++] = divisors[i];
            n /= divisors[i];
        }
    }

    uint64_t i = 5;
    uint64_t w = 2;
    while (i * i <= n) {
        while (n % i == 0) {
            if (idx >= 64) {
                uint64_t* new_factors = realloc(factors, (idx + 64) * sizeof(uint64_t));
                if (!new_factors) {
                    free(factors);
                    *err = ERR_ALLOCATION_FAILED;
                    return NULL;
                }
                factors = new_factors;
            }
            factors[idx++] = i;
            n /= i;
        }
        i += w;
        w = 6 - w;
    }

    if (n > 1) {
        if (idx >= 64) {
            uint64_t* new_factors = realloc(factors, (idx + 1) * sizeof(uint64_t));
            if (!new_factors) {
                free(factors);
                *err = ERR_ALLOCATION_FAILED;
                return NULL;
            }
            factors = new_factors;
        }
        factors[idx++] = n;
    }

    *count = idx;
    *err = PRIME_OK;
    return factors;
}

// Подсчет простых чисел
__declspec(dllexport) uint64_t prime_count(uint64_t n, PrimeError* err) {
    uint64_t count;
    uint64_t* primes = sieve_of_eratosthenes(n, &count, err);
    if (*err != PRIME_OK) return 0;
    free(primes);
    return count;
}

// Тест Ферма
__declspec(dllexport) bool ferma_test(uint64_t n, PrimeError* err) {
    if (n <= 1) { *err = ERR_INVALID_INPUT; return false; }
    const uint64_t bases[] = { 2, 3, 5, 7 };

    for (int i = 0; i < 4; i++) {
        uint64_t a = bases[i];
        uint64_t result = 1;
        uint64_t exp = n - 1;
        a %= n;

        while (exp > 0) {
            if (exp % 2 == 1)
                result = (result * a) % n;
            a = (a * a) % n;
            exp >>= 1;
        }

        if (result != 1) {
            *err = PRIME_OK;
            return false;
        }
    }

    *err = PRIME_OK;
    return true;
}

// Освобождение памяти
__declspec(dllexport) void free_array(uint64_t* arr) {
    free(arr);
}