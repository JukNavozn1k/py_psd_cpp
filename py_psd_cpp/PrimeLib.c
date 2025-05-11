#include "PrimeLib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

EXPORT void free_array(uint64_t* array) {
    free(array);
}

PrimeError is_prime(uint64_t n, bool* result) {
    if (n <= 1) {
        *result = false;
        return PRIME_OK;
    }
    if (n <= 3) {
        *result = true;
        return PRIME_OK;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        *result = false;
        return PRIME_OK;
    }
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            *result = false;
            return PRIME_OK;
        }
    }
    *result = true;
    return PRIME_OK;
}

PrimeError gcd(uint64_t a, uint64_t b, uint64_t* result) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    *result = a;
    return PRIME_OK;
}

PrimeError lcm(uint64_t a, uint64_t b, uint64_t* result) {
    uint64_t gcd_val;
    PrimeError err = gcd(a, b, &gcd_val);
    if (err != PRIME_OK) return err;

    if (gcd_val == 0) {
        *result = 0;
        return PRIME_OK;
    }

    if (a > UINT64_MAX / b) {
        return ERROR_NUMBER_TOO_LARGE;
    }
    uint64_t product = a * b;
    *result = product / gcd_val;
    return PRIME_OK;
}

PrimeError sieve_of_eratosthenes(uint64_t limit, uint64_t** primes, uint64_t* count) {
    if (limit < 2) {
        *primes = NULL;
        *count = 0;
        return PRIME_OK;
    }

    bool* is_prime_array = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime_array) return ERROR_INVALID_INPUT;

    memset(is_prime_array, true, (limit + 1) * sizeof(bool));
    is_prime_array[0] = is_prime_array[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime_array[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_prime_array[j] = false;
            }
        }
    }

    uint64_t prime_count = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime_array[i]) prime_count++;
    }

    uint64_t* prime_list = (uint64_t*)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list) {
        free(is_prime_array);
        return ERROR_INVALID_INPUT;
    }

    uint64_t index = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime_array[i]) prime_list[index++] = i;
    }

    free(is_prime_array);
    *primes = prime_list;
    *count = prime_count;
    return PRIME_OK;
}

PrimeError goldbach_conjecture(uint64_t n, uint64_t** result, uint64_t* size) {
    if (n <= 2 || n % 2 != 0) {
        *size = 0;
        return ERROR_INVALID_INPUT;
    }

    uint64_t* sieve = NULL;
    uint64_t sieve_count;
    PrimeError err = sieve_of_eratosthenes(n, &sieve, &sieve_count);
    if (err != PRIME_OK) return err;

    *result = (uint64_t*)malloc(2 * sizeof(uint64_t));
    if (!*result) {
        free(sieve);
        return ERROR_INVALID_INPUT;
    }

    for (uint64_t i = 0; i < sieve_count; ++i) {
        uint64_t p = sieve[i];
        bool is_p_prime;
        err = is_prime(n - p, &is_p_prime);
        if (err != PRIME_OK) {
            free(sieve);
            free(*result);
            return err;
        }
        if (is_p_prime) {
            (*result)[0] = p;
            (*result)[1] = n - p;
            *size = 2;
            free(sieve);
            return PRIME_OK;
        }
    }

    free(sieve);
    free(*result);
    *size = 0;
    return ERROR_INVALID_INPUT;
}

PrimeError prime_factors(uint64_t n, uint64_t** factors, uint64_t* count) {
    if (n == 0) return ERROR_ZERO_INVALID;

    uint64_t* temp = NULL;
    uint64_t size = 0;

    for (uint64_t divisor = 2; divisor <= 3; ++divisor) {
        while (n % divisor == 0) {
            temp = (uint64_t*)realloc(temp, (size + 1) * sizeof(uint64_t));
            temp[size++] = divisor;
            n /= divisor;
        }
    }

    for (uint64_t i = 5; i * i <= n; i += 6) {
        for (uint64_t j = 0; j < 2; ++j) {
            uint64_t divisor = i + (j == 0 ? 0 : 2);
            while (n % divisor == 0) {
                temp = (uint64_t*)realloc(temp, (size + 1) * sizeof(uint64_t));
                temp[size++] = divisor;
                n /= divisor;
            }
        }
    }

    if (n > 1) {
        temp = (uint64_t*)realloc(temp, (size + 1) * sizeof(uint64_t));
        temp[size++] = n;
    }

    *factors = temp;
    *count = size;
    return PRIME_OK;
}

PrimeError prime_count(uint64_t n, uint64_t* count) {
    uint64_t* sieve;
    uint64_t sieve_count;
    PrimeError err = sieve_of_eratosthenes(n, &sieve, &sieve_count);
    if (err != PRIME_OK) return err;
    *count = sieve_count;
    free(sieve);
    return PRIME_OK;
}

static uint64_t pow_mod(uint64_t base, uint64_t exponent, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % mod;
        exponent >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

PrimeError ferma_test(uint64_t n, bool* result) {
    if (n <= 1) {
        *result = false;
        return PRIME_OK;
    }
    const uint64_t a_list[] = { 2, 3, 5, 7 };
    for (int i = 0; i < 4; ++i) {
        uint64_t a = a_list[i];
        if (a >= n) continue;
        if (pow_mod(a, n - 1, n) != 1) {
            *result = false;
            return PRIME_OK;
        }
    }
    *result = true;
    return PRIME_OK;
}