#include "BigInteger.h"

int quickPow(long long base, int pow, int mod) {
    long long result = 1;
    for (; pow; pow >>= 1, base = base * base % mod) {
        if (pow & 1) {
            result = result * base % mod;
        }
    }
    return (int) result;
}

void FFT(std::vector<Complex> &array, std::vector<int> &rev, bool flag) {
    static const double pi = acos(-1);
    for (int i = 0; i < array.size(); ++i) {
        if (i < rev[i]) {
            std::swap(array[i], array[rev[i]]);
        }
    }
    for (int len = 1; len < array.size(); len <<= 1) {
        Complex wn(cos(pi / len), flag ? sin(pi / len) : -sin(pi / len));
        for (int begin = 0; begin < array.size(); begin += len << 1) {
            Complex w(1);
            for (int i = 0; i < len; ++i, w *= wn) {
                Complex x = array[begin + i], y = w * array[begin + i + len];
                array[begin + i] = x + y, array[begin + i + len] = x - y;
            }
        }
    }
}

void NTT(std::vector<int> &array, std::vector<int> &rev, int prime, bool flag) {
    for (int i = 0; i < array.size(); ++i) {
        if (i < rev[i]) {
            std::swap(array[i], array[rev[i]]);
        }
    }
    for (int len = 1; len < array.size(); len <<= 1) {
        long long wn = flag ? BigInteger::ntt[len << 1] : BigInteger::inv[len << 1];
        for (int begin = 0; begin < array.size(); begin += len << 1) {
            long long w = 1;
            for (int i = 0; i < len; ++i, w = w * wn % prime) {
                long long x = array[begin + i], y = array[begin + i + len] * w % prime;
                array[begin + i] = (int) ((x + y) % prime);
                array[begin + i + len] = (int) ((x - y + prime) % prime);
            }
        }
    }
}

int BigInteger::mulMethod = BigInteger::autoSelect;
std::unordered_map<int, int> BigInteger::ntt /* NOLINT */;
std::unordered_map<int, int> BigInteger::inv /* NOLINT */;

void BigInteger::setMulMethod(int method) {
    if (method == BigInteger::selectBrute || method == BigInteger::selectFFT
        || method == BigInteger::selectNTT) {
        BigInteger::mulMethod = method;
    }
}

void BigInteger::initNTT() {
    int reverse = quickPow(3, BigInteger::prime - 2, BigInteger::prime);
    for (int i = 2; i <= 1 << BigInteger::maxWidth; i <<= 1) {
        BigInteger::ntt[i] = quickPow(3, (BigInteger::prime - 1) / i, BigInteger::prime);
        BigInteger::inv[i] = quickPow(reverse, (BigInteger::prime - 1) / i, BigInteger::prime);
    }
}

void BigInteger::parseValue(unsigned long long from) {
    while (from) {
        this->value.emplace_back(from % BigInteger::mask);
        from /= BigInteger::mask;
    }
    if (this->value.empty()) {
        this->value.emplace_back(0);
    }
}

void BigInteger::parseString(std::string &from) {
    if (from[0] == '+' || from[0] == '-') {
        from.erase(0, 1);
    }
    from.erase(0, from.find_first_not_of('0'));
    if (from.length() == 0) {
        this->sign = true;
        this->value.emplace_back(0);
        return;
    }
    int start = (int) from.length();
    while (start != 0) {
        if (start >= BigInteger::width) {
            start -= BigInteger::width;
            this->value.emplace_back(std::stoi(from.substr(start, BigInteger::width)));
        } else {
            this->value.emplace_back(std::stoi(from.substr(0, start)));
            start = 0;
        }
    }
    this->sign |= this->value.size() == 1 && this->value[0] == 0;
}

bool BigInteger::unsignedGreater(const BigInteger &rhs, bool equal) const {
    if (this->value.size() != rhs.value.size()) {
        return this->value.size() > rhs.value.size();
    }
    for (int i = (int) this->value.size() - 1; i >= 0; --i) {
        if (this->value[i] != rhs.value[i]) {
            return this->value[i] > rhs.value[i];
        }
    }
    return equal;
}

void BigInteger::unsignedInc() {
    int adc = 1;
    for (int &i : this->value) {
        int update = i + adc;
        i = update % BigInteger::mask, adc = update >= BigInteger::mask;
    }
    if (adc) {
        this->value.emplace_back(adc);
    }
}

void BigInteger::unsignedAdd(const BigInteger &rhs) {
    while (this->value.size() < rhs.value.size()) {
        this->value.emplace_back(0);
    }
    int adc = 0;
    for (int i = 0; i < rhs.value.size(); ++i) {
        int update = this->value[i] + adc + rhs.value[i];
        this->value[i] = update % BigInteger::mask, adc = update >= BigInteger::mask;
    }
    for (int i = (int) rhs.value.size(); adc && i < this->value.size(); ++i) {
        int update = this->value[i] + adc;
        this->value[i] = update % BigInteger::mask, adc = update >= BigInteger::mask;
    }
    if (adc) {
        this->value.emplace_back(adc);
    }
}

void BigInteger::unsignedDec() {
    int adc = 1;
    for (int &i : this->value) {
        int update = i - adc;
        if (update < 0) {
            i = (update + BigInteger::mask) % BigInteger::mask, adc = 1;
        } else {
            i = update % BigInteger::mask, adc = 0;
        }
    }
    while (this->value.size() > 1 && this->value.back() == 0) {
        this->value.pop_back();
    }
}

void BigInteger::unsignedSub(const BigInteger &rhs) {
    while (this->value.size() < rhs.value.size()) {
        this->value.emplace_back(0);
    }
    int adc = 0;
    for (int i = 0; i < rhs.value.size(); ++i) {
        int update = this->value[i] - adc - rhs.value[i];
        if (update < 0) {
            this->value[i] = (update + BigInteger::mask) % BigInteger::mask, adc = 1;
        } else {
            this->value[i] = update % BigInteger::mask, adc = 0;
        }
    }
    for (int i = (int) rhs.value.size(); adc && i < this->value.size(); ++i) {
        int update = this->value[i] - adc;
        if (update < 0) {
            this->value[i] = (update + BigInteger::mask) % BigInteger::mask, adc = 1;
        } else {
            this->value[i] = update % BigInteger::mask, adc = 0;
        }
    }
    while (this->value.size() > 1 && this->value.back() == 0) {
        this->value.pop_back();
    }
}

void BigInteger::overflowFix() {
    int adc = 0;
    for (int &i : this->value) {
        int update = i + adc;
        i = update % BigInteger::mask, adc = update / BigInteger::mask;
    }
    while (adc) {
        this->value.emplace_back(adc % BigInteger::mask);
        adc /= BigInteger::mask;
    }
}

void BigInteger::unsignedBrute(const BigInteger &rhs) {
    std::vector<int> result(this->value.size() + rhs.value.size());
    for (int i = 0; i < this->value.size(); ++i) {
        int adc = 0;
        for (int j = 0; j < rhs.value.size(); ++j) {
            int update = result[i + j] + this->value[i] * rhs.value[j] + adc;
            result[i + j] = update % BigInteger::mask;
            adc = update / BigInteger::mask;
        }
        result[i + rhs.value.size()] = adc;
    }
    while (result.back() == 0) {
        result.pop_back();
    }
    this->value = result;
}

void BigInteger::unsignedFFT(const BigInteger &rhs) {
    int bit = 0, limit = 1;
    int length = (int) std::max(this->value.size(), rhs.value.size());
    for (; limit <= (length - 1) << 1; limit <<= 1, ++bit);
    std::vector<int> rev(limit);
    std::vector<Complex> array(limit);
    for (int i = 0; i < limit; ++i) {
        array[i] = Complex(i < this->value.size() ? this->value[i] : 0,
                           i < rhs.value.size() ? rhs.value[i] : 0);
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bit - 1));
    }
    FFT(array, rev, true);
    for (int i = 0; i < array.size(); array[i] *= array[i], ++i);
    FFT(array, rev, false);
    int index = limit - 1;
    for (; lround(array[index].getImag() / limit / 2) == 0; --index);
    this->value = std::vector<int>(++index);
    for (int i = 0; i < index; ++i) {
        this->value[i] = lround(array[i].getImag() / limit / 2);
    }
    overflowFix();
}

void BigInteger::unsignedNTT(const BigInteger &rhs) {
    int bit = 0, limit = 1;
    int length = (int) std::max(this->value.size(), rhs.value.size());
    for (; limit <= (length - 1) << 1; limit <<= 1, ++bit);
    std::vector<int> rev(limit);
    std::vector<int> left(limit);
    std::vector<int> right(limit);
    for (int i = 0; i < limit; ++i) {
        left[i] = i < this->value.size() ? this->value[i] : 0;
        right[i] = i < rhs.value.size() ? rhs.value[i] : 0;
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bit - 1));
    }
    if (BigInteger::ntt.empty()) {
        BigInteger::initNTT();
    }
    NTT(left, rev, BigInteger::prime, true), NTT(right, rev, BigInteger::prime, true);
    for (int i = 0; i < limit; ++i) {
        left[i] = (int) ((long long) left[i] * right[i] % BigInteger::prime);
    }
    NTT(left, rev, BigInteger::prime, false);
    int index = limit - 1;
    int reverse = quickPow(limit, BigInteger::prime - 2, BigInteger::prime);
    for (; left[index] * reverse % BigInteger::prime == 0; --index);
    this->value = std::vector<int>(++index);
    for (int i = 0; i < index; ++i) {
        this->value[i] = (int) ((long long) left[i] * reverse % BigInteger::prime);
    }
    overflowFix();
}

void BigInteger::shiftLeft() {
    int adc = 0;
    for (int &i : this->value) {
        i = i << 1 | adc;
        adc = i >= BigInteger::mask ? i -= BigInteger::mask, 1 : 0;
    }
    if (adc) {
        this->value.emplace_back(adc);
    }
}

void BigInteger::shiftRight() {
    for (int i = (int) this->value.size() - 1, adc = 0; i >= 0; --i) {
        int next = this->value[i] & 1 ? BigInteger::mask : 0;
        this->value[i] = (this->value[i] + adc) >> 1;
        adc = next;
    }
    while (this->value.size() > 1 && this->value.back() == 0) {
        this->value.pop_back();
    }
}

void BigInteger::binaryLifting(const BigInteger &rhs, bool upper, bool divFlag) {
    BigInteger sub = rhs, binary(1), result(0);
    while (unsignedGreater(sub, true)) {
        sub.shiftLeft(), binary.shiftLeft();
    }
    for (bool notFirst = false; binary.value.size() != 1 || binary.value[0] != 0;) {
        notFirst ? sub.shiftRight(), 0 : notFirst = true;
        if (unsignedGreater(sub, true)) {
            unsignedSub(sub);
            result.unsignedAdd(binary);
        }
        binary.shiftRight();
    }
    if (divFlag) {
        if (upper && (this->value.size() != 1 || this->value[0] != 0)) {
            this->value = result.value;
            unsignedInc();
        } else {
            this->value = result.value;
        }
    } else {
        if (upper && (this->value.size() != 1 || this->value[0] != 0)) {
            sub.unsignedSub(*this);
            this->value = sub.value;
        }
    }
}

BigInteger::BigInteger() {
    this->sign = true;
    this->value.emplace_back(0);
}

BigInteger::BigInteger(int from) {
    this->sign = from >= 0;
    if (from == INT_MIN) {
        parseValue((unsigned long long) INT_MAX + 1);
    } else {
        parseValue(from < 0 ? -from : from);
    }
}

BigInteger::BigInteger(unsigned int from) {
    this->sign = true;
    parseValue(from);
}

BigInteger::BigInteger(long long from) {
    this->sign = from >= 0;
    if (from == LONG_LONG_MIN) {
        parseValue((unsigned long long) LONG_LONG_MAX + 1);
    } else {
        parseValue(from < 0 ? -from : from);
    }
}

BigInteger::BigInteger(unsigned long long from) {
    this->sign = true;
    parseValue(from);
}

BigInteger::BigInteger(const char *from) {
    std::string str(from);
    if (str.length() == 0) {
        this->sign = true;
        this->value.emplace_back(0);
    } else {
        this->sign = from[0] != '-';
        parseString(str);
    }
}

BigInteger::BigInteger(std::string &from) {
    std::string str(from);
    if (str.length() == 0) {
        this->sign = true;
        this->value.emplace_back(0);
    } else {
        this->sign = from[0] != '-';
        parseString(str);
    }
}

BigInteger BigInteger::abs() const {
    BigInteger result(*this);
    result.sign = true;
    return result;
}

BigInteger BigInteger::operator+() const {
    return *this;
}

BigInteger BigInteger::operator++() {
    if (this->sign) {
        unsignedInc();
    } else {
        unsignedDec();
        this->sign |= this->value.size() == 1 && this->value[0] == 0;
    }
    return *this;
}

const BigInteger /* NOLINT */ BigInteger::operator++(int) {
    BigInteger result = *this;
    if (this->sign) {
        unsignedInc();
    } else {
        unsignedDec();
        this->sign |= this->value.size() == 1 && this->value[0] == 0;
    }
    return result;
}

BigInteger BigInteger::operator-() const {
    BigInteger result(*this);
    result.sign = (result.value.size() == 1 && result.value[0] == 0) || !result.sign;
    return result;
}

BigInteger BigInteger::operator--() {
    if (this->sign) {
        if (this->value.size() == 1 && this->value[0] == 0) {
            this->sign = false;
            this->value[0] = 1;
        } else {
            unsignedDec();
        }
    } else {
        unsignedInc();
    }
    return *this;
}

const BigInteger /* NOLINT */ BigInteger::operator--(int) {
    BigInteger result = *this;
    if (this->sign) {
        if (this->value.size() == 1 && this->value[0] == 0) {
            this->sign = false;
            this->value[0] = 1;
        } else {
            unsignedDec();
        }
    } else {
        unsignedInc();
    }
    return result;
}

bool BigInteger::operator==(const BigInteger &rhs) const {
    return this->sign == rhs.sign && this->value == rhs.value;
}

bool BigInteger::operator!=(const BigInteger &rhs) const {
    return this->sign != rhs.sign || this->value != rhs.value;
}

bool BigInteger::operator>(const BigInteger &rhs) const {
    if (this->sign != rhs.sign) {
        return this->sign > rhs.sign;
    }
    return unsignedGreater(rhs, !this->sign) ^ !this->sign;
}

bool BigInteger::operator>=(const BigInteger &rhs) const {
    if (this->sign != rhs.sign) {
        return this->sign > rhs.sign;
    }
    return unsignedGreater(rhs, this->sign) ^ !this->sign;
}

bool BigInteger::operator<(const BigInteger &rhs) const {
    if (this->sign != rhs.sign) {
        return this->sign < rhs.sign;
    }
    return rhs.unsignedGreater(*this, !this->sign) ^ !this->sign;
}

bool BigInteger::operator<=(const BigInteger &rhs) const {
    if (this->sign != rhs.sign) {
        return this->sign < rhs.sign;
    }
    return rhs.unsignedGreater(*this, this->sign) ^ !this->sign;
}

BigInteger BigInteger::operator+(const BigInteger &rhs) const {
    if (this->sign == rhs.sign) {
        BigInteger result = *this;
        result.unsignedAdd(rhs);
        return result;
    } else {
        if (unsignedGreater(rhs, true)) {
            BigInteger result = *this;
            result.unsignedSub(rhs);
            return result;
        } else {
            BigInteger result = rhs;
            result.unsignedSub(*this);
            result.sign = result.value.size() == 1 && result.value[0] == 0 || result.sign;
            return result;
        }
    }
}

BigInteger BigInteger::operator-(const BigInteger &rhs) const {
    if (this->sign == rhs.sign) {
        if (unsignedGreater(rhs, true)) {
            BigInteger result = *this;
            result.unsignedSub(rhs);
            return result;
        } else {
            BigInteger result = rhs;
            result.unsignedSub(*this);
            result.sign = result.value.size() == 1 && result.value[0] == 0 || !rhs.sign;
            return result;
        }
    } else {
        BigInteger result = *this;
        result.unsignedAdd(rhs);
        return result;
    }
}

BigInteger BigInteger::operator*(const BigInteger &rhs) const {
    if (this->value.size() == 1 && this->value[0] == 0) {
        return BigInteger(0);
    }
    if (rhs.value.size() == 1 && rhs.value[0] == 0) {
        return BigInteger(0);
    }
    BigInteger result = *this;
    result.sign = this->sign == rhs.sign;
    int len = (int) std::min(this->value.size(), rhs.value.size());
    if (BigInteger::mulMethod == BigInteger::selectBrute ||
        BigInteger::mulMethod == BigInteger::autoSelect && len <= 1 << 6) {
        result.unsignedBrute(rhs);
    } else if (BigInteger::mulMethod == BigInteger::selectNTT ||
               BigInteger::mulMethod == BigInteger::autoSelect && len <= 1 << (BigInteger::maxWidth - 1)) {
        result.unsignedNTT(rhs);
    } else if (BigInteger::mulMethod == BigInteger::selectFFT) {
        result.unsignedFFT(rhs);
    }
    return result;
}

BigInteger BigInteger::operator/(const BigInteger &rhs) const {
    if (rhs.value.size() == 1 && rhs.value[0] == 0) {
        throw BigIntegerException("Divide Zero");
    }
    if (this->value.size() == 1 && this->value[0] == 0) {
        return *this;
    }
    BigInteger result = *this;
    result.binaryLifting(rhs, this->sign != rhs.sign, true);
    result.sign = this->sign == rhs.sign || (result.value.size() == 1 && result.value[0] == 0);
    return result;
}

BigInteger BigInteger::operator%(const BigInteger &rhs) const {
    if (rhs.value.size() == 1 && rhs.value[0] == 0) {
        throw BigIntegerException("Divide Zero");
    }
    if (this->value.size() == 1 && this->value[0] == 0) {
        return *this;
    }
    BigInteger result = *this;
    result.binaryLifting(rhs, this->sign != rhs.sign, false);
    result.sign = rhs.sign || (result.value.size() == 1 && result.value[0] == 0);
    return result;
}

BigInteger &BigInteger::operator+=(const BigInteger &rhs) {
    if (this->sign == rhs.sign) {
        unsignedAdd(rhs);
    } else {
        if (unsignedGreater(rhs, true)) {
            unsignedSub(rhs);
        } else {
            BigInteger result = rhs;
            result.unsignedSub(*this);
            this->value = result.value;
            this->sign = result.value.size() == 1 && result.value[0] == 0 || result.sign;
        }
    }
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &rhs) {
    if (this->sign == rhs.sign) {
        if (unsignedGreater(rhs, true)) {
            unsignedSub(rhs);
        } else {
            BigInteger result = rhs;
            result.unsignedSub(*this);
            this->value = result.value;
            this->sign = result.value.size() == 1 && result.value[0] == 0 || !result.sign;
        }
    } else {
        unsignedAdd(rhs);
    }
    return *this;
}

BigInteger &BigInteger::operator*=(const BigInteger &rhs) {
    if (this->value.size() == 1 && this->value[0] == 0) {
        return *this;
    }
    if (rhs.value.size() == 1 && rhs.value[0] == 0) {
        this->value = rhs.value;
        this->sign = true;
        return *this;
    }
    this->sign = this->sign == rhs.sign;
    bool isSmall = std::min(this->value.size(), rhs.value.size()) <= 1 << 6;
    bool notLarge = std::max(this->value.size(), rhs.value.size()) <= 1 << (BigInteger::maxWidth - 1);
    if (BigInteger::mulMethod == BigInteger::selectBrute ||
        BigInteger::mulMethod == BigInteger::autoSelect && isSmall) {
        this->unsignedBrute(rhs);
    } else if (BigInteger::mulMethod == BigInteger::selectNTT ||
               BigInteger::mulMethod == BigInteger::autoSelect && notLarge) {
        this->unsignedNTT(rhs);
    } else if (BigInteger::mulMethod == BigInteger::selectFFT) {
        this->unsignedFFT(rhs);
    }
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &rhs) {
    if (rhs.value.size() == 1 && rhs.value[0] == 0) {
        throw BigIntegerException("Divide Zero");
    }
    if (this->value.size() == 1 && this->value[0] == 0) {
        return *this;
    }
    binaryLifting(rhs, this->sign != rhs.sign, true);
    this->sign = this->sign == rhs.sign || (this->value.size() == 1 && this->value[0] == 0);
    return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &rhs) {
    if (rhs.value.size() == 1 && rhs.value[0] == 0) {
        throw BigIntegerException("Divide Zero");
    }
    if (this->value.size() == 1 && this->value[0] == 0) {
        return *this;
    }
    binaryLifting(rhs, this->sign != rhs.sign, false);
    this->sign = rhs.sign || (this->value.size() == 1 && this->value[0] == 0);
    return *this;
}

std::string BigInteger::toString() const {
    int begin = this->value.back();
    std::string result = std::to_string(this->sign ? begin : -begin);
    for (int i = (int) this->value.size() - 2; i >= 0; --i) {
        std::string str = std::to_string(this->value[i]);
        result += std::string(BigInteger::width - str.length(), '0') + str;
    }
    return result;
}
