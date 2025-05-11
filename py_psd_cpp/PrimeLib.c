#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "PrimeLib.h"

static PrimeError validate_positive(int n, const char* op) {
    if (n < 0) return ERR_NEGATIVE;
    if (n > MAX_VAL) return ERR_TOO_LARGE;
    return PRIME_OK;
}

static int power_mod(int a, int b, int mod) {
    int result = 1;
    a %= mod;
    while (b > 0) {
        if (b % 2 == 1)
            result = (result * a) % mod;
        a = (a * a) % mod;
        b /= 2;
    }
    return result;
}

void free_array(int* arr) {
    if (arr) free(arr);
}

IntResult is_prime(int n) {
    IntResult result = { 0, PRIME_OK };
    result.error = validate_positive(n, "primality test");
    if (result.error != PRIME_OK) return result;

    if (n <= 1) {
        result.result = 0;
        return result;
    }
    if (n <= 3) {
        result.result = 1;
        return result;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        result.result = 0;
        return result;
    }
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            result.result = 0;
            return result;
        }
    }
    result.result = 1;
    return result;
}

IntResult gcd(int a, int b) {
    IntResult result = { 0, PRIME_OK };
    result.error = validate_positive(a, "GCD");
    if (result.error == PRIME_OK) result.error = validate_positive(b, "GCD");
    if (result.error != PRIME_OK) return result;

    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    result.result = a;
    return result;
}

IntResult lcm(int a, int b) {
    IntResult result = { 0, PRIME_OK };
    IntResult gcd_result = gcd(a, b);
    if (gcd_result.error != PRIME_OK) {
        result.error = gcd_result.error;
        return result;
    }
    if (a == 0 || b == 0) {
        result.result = 0;
        return result;
    }
    result.result = (a / gcd_result.result) * b;
    return result;
}

IntArrayResult sieve_of_eratosthenes(int limit) {
    IntArrayResult result = { NULL, 0, PRIME_OK };
    result.error = validate_positive(limit, "sieve");
    if (result.error != PRIME_OK) return result;

    if (limit < 2) return result;

    bool* sieve = (bool*)calloc(limit + 1, sizeof(bool));
    for (int i = 2; i * i <= limit; i++) {
        if (!sieve[i]) {
            for (int j = i * i; j <= limit; j += i)
                sieve[j] = true;
        }
    }

    int count = 0;
    for (int i = 2; i <= limit; i++)
        if (!sieve[i]) count++;

    int* primes = (int*)malloc(count * sizeof(int));
    for (int i = 2, j = 0; i <= limit; i++) {
        if (!sieve[i]) primes[j++] = i;
    }

    free(sieve);
    result.data = primes;
    result.length = count;
    return result;
}

IntArrayResult goldbach_conjecture(int n) {
    IntArrayResult result = { NULL, 0, PRIME_OK };
    result.error = validate_positive(n, "Goldbach");
    if (n <= 2 || n % 2 != 0) result.error = ERR_INVALID_INPUT;
    if (result.error != PRIME_OK) return result;

    IntArrayResult sieve_result = sieve_of_eratosthenes(n);
    if (sieve_result.error != PRIME_OK) {
        result.error = sieve_result.error;
        return result;
    }

    for (int i = 0; i < sieve_result.length; i++) {
        int p = sieve_result.data[i];
        IntResult is_prime_result = is_prime(n - p);
        if (is_prime_result.result) {
            int* pair = (int*)malloc(2 * sizeof(int));
            pair[0] = p;
            pair[1] = n - p;
            result.data = pair;
            result.length = 2;
            free_array(sieve_result.data);
            return result;
        }
    }

    free_array(sieve_result.data);
    return result;
}

IntArrayResult prime_factors(int n) {
    IntArrayResult result = { NULL, 0, PRIME_OK };
    result.error = validate_positive(n, "prime factorization");
    if (n == 0) result.error = ERR_INVALID_INPUT;
    if (result.error != PRIME_OK) return result;

    int* factors = (int*)malloc(20 * sizeof(int));
    int count = 0;
    int divisors[] = { 2, 3 };

    for (int i = 0; i < 2; i++) {
        while (n % divisors[i] == 0) {
            factors[count++] = divisors[i];
            n /= divisors[i];
        }
    }

    int i = 5;
    int w = 2;
    while (i * i <= n) {
        while (n % i == 0) {
            if (count % 20 == 19)
                factors = (int*)realloc(factors, (count + 20) * sizeof(int));
            factors[count++] = i;
            n /= i;
        }
        i += w;
        w = 6 - w;
    }

    if (n > 1) {
        if (count % 20 == 19)
            factors = (int*)realloc(factors, (count + 1) * sizeof(int));
        factors[count++] = n;
    }

    result.data = (int*)realloc(factors, count * sizeof(int));
    result.length = count;
    return result;
}

IntResult prime_count(int n) {
    IntResult result = { 0, PRIME_OK };
    IntArrayResult sieve_result = sieve_of_eratosthenes(n);
    result.error = sieve_result.error;
    result.result = sieve_result.length;
    free_array(sieve_result.data);
    return result;
}

IntResult ferma_test(int n) {
    IntResult result = { 0, PRIME_OK };
    result.error = validate_positive(n, "ferma test");
    if (n <= 1) {
        result.result = 0;
        return result;
    }

    int bases[] = { 2, 3, 5, 7 };
    for (int i = 0; i < 4; i++) {
        if (power_mod(bases[i], n - 1, n) != 1) {
            result.result = 0;
            return result;
        }
    }
    result.result = 1;
    return result;
}