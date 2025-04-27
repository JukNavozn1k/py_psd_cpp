#ifdef PRIMELIB_EXPORTS
#define PRIMELIB_API __declspec(dllexport)
#else
#define PRIMELIB_API __declspec(dllimport)
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef int64_t PrimeInt;

    enum PrimeErrorCode {
        PRIME_NO_ERROR = 0,
        PRIME_NEGATIVE_NUMBER_ERROR,
        PRIME_INVALID_INPUT_ERROR,
        PRIME_NUMBER_TOO_LARGE_ERROR,
        PRIME_UNDEFINED_ERROR,
        PRIME_INVALID_INPUT_ZERO_ERROR,
        PRIME_GOLDBACH_INVALID_INPUT_ERROR
    };

    typedef struct {
        PrimeInt* data;
        int length;
    } PrimeIntArray;

    PRIMELIB_API PrimeErrorCode IsPrime(PrimeInt n, bool* result);
    PRIMELIB_API PrimeErrorCode PrimeFactors(PrimeInt n, PrimeIntArray* result);
    PRIMELIB_API PrimeErrorCode GCD(PrimeInt a, PrimeInt b, PrimeInt* result);
    PRIMELIB_API PrimeErrorCode LCM(PrimeInt a, PrimeInt b, PrimeInt* result);
    PRIMELIB_API PrimeErrorCode SieveOfEratosthenes(PrimeInt limit, PrimeIntArray* result);
    PRIMELIB_API PrimeErrorCode GoldbachConjecture(PrimeInt n, PrimeIntArray* result);
    PRIMELIB_API void FreePrimeIntArray(PrimeIntArray* array);

#ifdef __cplusplus
}
#endif