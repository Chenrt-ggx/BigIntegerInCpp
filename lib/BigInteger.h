#ifndef BIGINTEGER_BIGINTEGER_H
#define BIGINTEGER_BIGINTEGER_H

#include <vector>
#include <unordered_map>

#include "./Complex.h"

class BigInteger {
private:
    bool sign;
    std::vector<int> value;

    static int mulMethod;
    static constexpr int maxWidth = 23;
    static constexpr int prime = 998244353;

    void parseValue(unsigned long long from);

    void parseString(std::string &from);

    bool unsignedGreater(const BigInteger &rhs, bool equal) const;

    void unsignedInc();

    void unsignedAdd(const BigInteger &rhs);

    void unsignedDec();

    void unsignedSub(const BigInteger &rhs);

    void overflowFix();

    void unsignedBrute(const BigInteger &rhs);

    void unsignedFFT(const BigInteger &rhs);

    void unsignedNTT(const BigInteger &rhs);

    void shiftLeft();

    void shiftRight();

    void binaryLifting(const BigInteger &rhs, bool upper, bool divFlag);

public:
    class BigIntegerException : public std::exception {
    private:
        const char *message;
    public:
        explicit BigIntegerException(const char *message) : message(message) {}

        const char *what() const noexcept override {
            return this->message;
        }
    };

    static constexpr int width = 4;
    static constexpr int mask = 10000;
    static constexpr int autoSelect = 0;
    static constexpr int selectBrute = 1;
    static constexpr int selectFFT = 2;
    static constexpr int selectNTT = 3;
    static std::unordered_map<int, int> ntt;
    static std::unordered_map<int, int> inv;

    static void setMulMethod(int method);

    static void initNTT();

    explicit BigInteger();

    explicit BigInteger(int from);

    explicit BigInteger(unsigned int from);

    explicit BigInteger(long long from);

    explicit BigInteger(unsigned long long from);

    explicit BigInteger(const char *from);

    explicit BigInteger(std::string &from);

    BigInteger abs() const;

    BigInteger operator+() const;

    BigInteger operator++();

    const BigInteger operator++(int);

    BigInteger operator-() const;

    BigInteger operator--();

    const BigInteger operator--(int);

    bool operator==(const BigInteger &rhs) const;

    bool operator!=(const BigInteger &rhs) const;

    bool operator>(const BigInteger &rhs) const;

    bool operator>=(const BigInteger &rhs) const;

    bool operator<(const BigInteger &rhs) const;

    bool operator<=(const BigInteger &rhs) const;

    BigInteger operator+(const BigInteger &rhs) const;

    BigInteger operator-(const BigInteger &rhs) const;

    BigInteger operator*(const BigInteger &rhs) const;

    BigInteger operator/(const BigInteger &rhs) const;

    BigInteger operator%(const BigInteger &rhs) const;

    BigInteger &operator+=(const BigInteger &rhs);

    BigInteger &operator-=(const BigInteger &rhs);

    BigInteger &operator*=(const BigInteger &rhs);

    BigInteger &operator/=(const BigInteger &rhs);

    BigInteger &operator%=(const BigInteger &rhs);

    std::string toString() const;
};

#endif //BIGINTEGER_BIGINTEGER_H
