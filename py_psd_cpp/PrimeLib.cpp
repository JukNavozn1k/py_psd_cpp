#include "PrimeLib.h"
#include <vector>
#include <stdexcept>
#include <string>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <functional>  
#include <memory>    

constexpr PrimeInt MAX_VAL = 2147483647;

class PrimeException : public std::exception {
public:
    virtual PrimeErrorCode GetErrorCode() const = 0;
};

class NegativeNumberError : public PrimeException {
public:
    PrimeErrorCode GetErrorCode() const override { return PRIME_NEGATIVE_NUMBER_ERROR; }
};

class InvalidInputError : public PrimeException {
public:
    PrimeErrorCode GetErrorCode() const override { return PRIME_INVALID_INPUT_ERROR; }
};

class NumberTooLargeError : public PrimeException {
public:
    PrimeErrorCode GetErrorCode() const override { return PRIME_NUMBER_TOO_LARGE_ERROR; }
};

class UndefinedError : public PrimeException {
public:
    PrimeErrorCode GetErrorCode() const override { return PRIME_UNDEFINED_ERROR; }
};

void ValidatePositive(PrimeInt n, const std::string& operation) {
    if (n < 0) {
        throw NegativeNumberError();
    }
    if (n > MAX_VAL) {
        throw NumberTooLargeError();
    }
}

bool IsPrimeImpl(PrimeInt n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    PrimeInt i = 5;
    PrimeInt w = 2;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i += w;
        w = 6 - w;
    }
    return true;
}

std::vector<PrimeInt> PrimeFactorsImpl(PrimeInt n) {
    ValidatePositive(n, "prime factorization");
    if (n == 0) {
        throw InvalidInputError();
    }
    std::vector<PrimeInt> factors;
    for (PrimeInt divisor : {2, 3}) {
        while (n % divisor == 0) {
            factors.push_back(divisor);
            n /= divisor;
        }
    }
    PrimeInt i = 5;
    PrimeInt w = 2;
    while (i * i <= n) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
        i += w;
        w = 6 - w;
    }
    if (n > 1) {
        factors.push_back(n);
    }
    return factors;
}

PrimeInt GCDImpl(PrimeInt a, PrimeInt b) {
    ValidatePositive(a, "GCD");
    ValidatePositive(b, "GCD");
    if (a == 0 && b == 0) {
        throw InvalidInputError();
    }
    while (b != 0) {
        PrimeInt temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

PrimeInt LCMImpl(PrimeInt a, PrimeInt b) {
    ValidatePositive(a, "LCM");
    ValidatePositive(b, "LCM");
    if (a == 0 || b == 0) {
        throw InvalidInputError();
    }
    return (a / GCDImpl(a, b)) * b;
}

std::vector<PrimeInt> SieveOfEratosthenesImpl(PrimeInt limit) {
    ValidatePositive(limit, "sieve of Eratosthenes");
    if (limit < 2) {
        return {};
    }
    std::vector<bool> sieve(limit + 1, true);
    sieve[0] = sieve[1] = false;
    for (PrimeInt num = 2; num * num <= limit; ++num) {
        if (sieve[num]) {
            for (PrimeInt multiple = num * num; multiple <= limit; multiple += num) {
                sieve[multiple] = false;
            }
        }
    }
    std::vector<PrimeInt> primes;
    for (PrimeInt num = 2; num <= limit; ++num) {
        if (sieve[num]) {
            primes.push_back(num);
        }
    }
    return primes;
}

std::vector<PrimeInt> GoldbachConjectureImpl(PrimeInt n) {
    ValidatePositive(n, "Goldbach conjecture");
    if (n <= 2) {
        throw InvalidInputError();
    }
    if (n % 2 != 0) {
        throw InvalidInputError();
    }
    auto primes = SieveOfEratosthenesImpl(n);
    std::unordered_set<PrimeInt> primeSet(primes.begin(), primes.end());
    for (PrimeInt p : primes) {
        if (p > n / 2) {
            break;
        }
        if (primeSet.find(n - p) != primeSet.end()) {
            return { p, n - p };
        }
    }
    return {};
}

PrimeErrorCode HandleExceptions(std::function<void()> func) {
    try {
        func();
        return PRIME_NO_ERROR;
    }
    catch (const NegativeNumberError&) {
        return PRIME_NEGATIVE_NUMBER_ERROR;
    }
    catch (const InvalidInputError&) {
        return PRIME_INVALID_INPUT_ERROR;
    }
    catch (const NumberTooLargeError&) {
        return PRIME_NUMBER_TOO_LARGE_ERROR;
    }
    catch (const PrimeException&) {
        return PRIME_UNDEFINED_ERROR;
    }
    catch (...) {
        return PRIME_UNDEFINED_ERROR;
    }
}

PRIMELIB_API PrimeErrorCode IsPrime(PrimeInt n, bool* result) {
    return HandleExceptions([&]() {
        ValidatePositive(n, "primality test");
        *result = IsPrimeImpl(n);
        });
}

PRIMELIB_API PrimeErrorCode PrimeFactors(PrimeInt n, PrimeIntArray* result) {
    return HandleExceptions([&]() {
        auto factors = PrimeFactorsImpl(n);
        result->data = new PrimeInt[factors.size()];
        result->length = static_cast<int>(factors.size());
        std::copy(factors.begin(), factors.end(), result->data);
        });
}

PRIMELIB_API PrimeErrorCode GCD(PrimeInt a, PrimeInt b, PrimeInt* result) {
    return HandleExceptions([&]() {
        *result = GCDImpl(a, b);
        });
}

PRIMELIB_API PrimeErrorCode LCM(PrimeInt a, PrimeInt b, PrimeInt* result) {
    return HandleExceptions([&]() {
        *result = LCMImpl(a, b);
        });
}

PRIMELIB_API PrimeErrorCode SieveOfEratosthenes(PrimeInt limit, PrimeIntArray* result) {
    return HandleExceptions([&]() {
        auto primes = SieveOfEratosthenesImpl(limit);
        result->data = new PrimeInt[primes.size()];
        result->length = static_cast<int>(primes.size());
        std::copy(primes.begin(), primes.end(), result->data);
        });
}

PRIMELIB_API PrimeErrorCode GoldbachConjecture(PrimeInt n, PrimeIntArray* result) {
    return HandleExceptions([&]() {
        auto pair = GoldbachConjectureImpl(n);
        if (pair.size() != 2) {
            throw InvalidInputError();
        }
        result->data = new PrimeInt[2];
        result->length = 2;
        result->data[0] = pair[0];
        result->data[1] = pair[1];
        });
}

PRIMELIB_API void FreePrimeIntArray(PrimeIntArray* array) {
    if (array != nullptr) {
        delete[] array->data;
        array->data = nullptr;
        array->length = 0;
    }
}
