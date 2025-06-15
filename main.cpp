#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <cassert>

// Возведение в степень по модулю
uint64_t mod_exp(uint64_t base, uint64_t exponent, uint64_t modulus) {
    uint64_t result = 1;
    base %= modulus;
    while (exponent > 0) {
        if (exponent & 1)
            result = (__uint128_t(result) * base) % modulus; // используем __uint128_t для предотвращения переполнения
        base = (__uint128_t(base) * base) % modulus;
        exponent >>= 1;
    }
    return result;
}

// Факторизация 
void factorize(uint64_t n, uint64_t& s, uint64_t& d) {
    s = 0;
    d = n - 1;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
}

// Тест Миллера-Раббина для одной базы
bool miller_rabin_test(uint64_t n, uint64_t a) {
    uint64_t s, d;
    factorize(n, s, d);
    uint64_t x = mod_exp(a, d, n);
    if (x == 1 || x == n - 1)
        return true;
    for (uint64_t r = 1; r < s; r++) {
        x = (__uint128_t(x) * x) % n;
        if (x == n - 1)
            return true;
    }
    return false;
}

// Проверка простоты для 64-битных чисел с фиксированными базами
bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    // Для 64-битных чисел достаточно проверить эти базы (по https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test)
    static const uint64_t test_bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

    for (uint64_t a : test_bases) {
        if (a % n == 0) // если база равна n, то пропускаем
            return true;
        if (!miller_rabin_test(n, a))
            return false;
    }
    return true;
}

int main() {
    std::cout << "Введите число : ";
    uint64_t num;
    while (std::cin >> num) {
        if (is_prime(num)) {
            std::cout << num << " - простое число\n";
        } else {
            std::cout << num << " - составное число\n";
        }
        std::cout << "Введите число для новой проверки (Ctrl+D для выхода): ";
    }
    return 0;
}
