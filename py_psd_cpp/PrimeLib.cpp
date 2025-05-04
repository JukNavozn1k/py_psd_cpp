#include "PrimeLib.h"
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <sstream>

using namespace std;

class PrimeException : public exception {
    string msg_;
public:
    PrimeException(const string& msg) : msg_(msg) {}
    const char* what() const noexcept override { return msg_.c_str(); }
};

class InvalidInputError : public PrimeException {
public:
    InvalidInputError(const string& msg) : PrimeException(msg) {}
};
class NumberTooLargeError : public PrimeException {
public:
    NumberTooLargeError(const string& msg) : PrimeException(msg) {}
};

// Validation helper
void validate(uint64_t n, const string& op) {
    if (op == "factorization" && n == 0)
        throw InvalidInputError("Cannot factorize zero");
    if (n > MAX_VAL)
        throw NumberTooLargeError("Number exceeds maximum allowed value");
}

// Prime checking implementation
bool is_prime_impl(uint64_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

// GCD implementation
uint64_t gcd_impl(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Sieve implementation
vector<uint64_t> sieve_impl(uint64_t limit) {
    vector<bool> sieve(limit + 1, true);
    sieve[0] = sieve[1] = false;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = false;

    vector<uint64_t> primes;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i]) primes.push_back(i);
    return primes;
}

// Goldbach conjecture implementation
vector<uint64_t> goldbach_impl(uint64_t n) {
    if (n <= 2 || n % 2 != 0)
        throw InvalidInputError("Number must be even and > 2");

    auto primes = sieve_impl(n);
    for (auto p : primes)
        if (is_prime_impl(n - p))
            return { p, n - p };
    return {};
}

// Prime factors implementation
vector<uint64_t> prime_factors_impl(uint64_t n) {
    vector<uint64_t> factors;
    for (uint64_t d : {2, 3}) {
        while (n % d == 0) {
            factors.push_back(d);
            n /= d;
        }
    }
    for (uint64_t i = 5; i * i <= n; i += 6) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
        uint64_t next = i + 2;
        while (n % next == 0) {
            factors.push_back(next);
            n /= next;
        }
    }
    if (n > 1) factors.push_back(n);
    return factors;
}

// Fermat test implementation
uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

bool ferma_test_impl(uint64_t n) {
    if (n <= 1) return false;
    for (uint64_t a : {2, 3, 5, 7}) {
        if (a >= n) continue;
        if (mod_pow(a, n - 1, n) != 1)
            return false;
    }
    return true;
}

// Wrapper functions
#define WRAP(f) try { validate(n, #f); *result = f##_impl(n); return true; } \
catch (const PrimeException& e) { *error = e.what(); return false; } \
catch (...) { *error = "Unknown error"; return false; }

extern "C" {
    bool is_prime(uint64_t n, bool* result, const char** error) WRAP(is_prime)

        bool gcd(uint64_t a, uint64_t b, uint64_t* result, const char** error) {
        try {
            validate(a, "gcd"); validate(b, "gcd");
            *result = gcd_impl(a, b);
            return true;
        }
        catch (...) { *error = "GCD error"; return false; }
    }

    bool lcm(uint64_t a, uint64_t b, uint64_t* result, const char** error) {
        try {
            validate(a, "lcm"); validate(b, "lcm");
            *result = a / gcd_impl(a, b) * b;
            return true;
        }
        catch (...) { *error = "LCM error"; return false; }
    }

    bool sieve_of_eratosthenes(uint64_t limit, uint64_t** primes, size_t* count, const char** error) {
        try {
            validate(limit, "sieve");
            auto vec = sieve_impl(limit);
            *primes = new uint64_t[vec.size()];
            copy(vec.begin(), vec.end(), *primes);
            *count = vec.size();
            return true;
        }
        catch (const PrimeException& e) { *error = e.what(); return false; }
        catch (...) { *error = "Sieve error"; return false; }
    }

    bool goldbach_conjecture(uint64_t n, uint64_t** pair, size_t* count, const char** error) {
        try {
            validate(n, "goldbach");
            auto vec = goldbach_impl(n);
            if (vec.empty()) throw PrimeException("No pair found");
            *pair = new uint64_t[vec.size()];
            copy(vec.begin(), vec.end(), *pair);
            *count = vec.size();
            return true;
        }
        catch (const PrimeException& e) { *error = e.what(); return false; }
        catch (...) { *error = "Goldbach error"; return false; }
    }

    bool prime_factors(uint64_t n, uint64_t** factors, size_t* count, const char** error) {
        try {
            validate(n, "factorization");
            auto vec = prime_factors_impl(n);
            *factors = new uint64_t[vec.size()];
            copy(vec.begin(), vec.end(), *factors);
            *count = vec.size();
            return true;
        }
        catch (const PrimeException& e) { *error = e.what(); return false; }
        catch (...) { *error = "Factorization error"; return false; }
    }

    bool prime_count(uint64_t n, uint64_t* count, const char** error) {
        try {
            validate(n, "prime_count");
            *count = sieve_impl(n).size();
            return true;
        }
        catch (const PrimeException& e) { *error = e.what(); return false; }
        catch (...) { *error = "Count error"; return false; }
    }

    bool ferma_test(uint64_t n, bool* result, const char** error) WRAP(ferma_test)

        void free_array(uint64_t* array) { delete[] array; }
}