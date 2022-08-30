#ifndef BIGINTEGER_COMPLEX_H
#define BIGINTEGER_COMPLEX_H

#include <cmath>
#include <string>

class Complex {
private:
    double real;
    double imag;
    static constexpr double eps = 1e-8;

public:
    class ComplexException : public std::exception {
    private:
        const char *message;
    public:
        explicit ComplexException(const char *message) : message(message) {}

        const char *what() const noexcept override {
            return this->message;
        }
    };

    explicit Complex(double real = 0, double imag = 0);

    void update(double newReal, double newImag);

    double getReal() const;

    double getImag() const;

    double length() const;

    Complex operator+(const Complex &rhs) const;

    Complex operator-(const Complex &rhs) const;

    Complex operator*(const Complex &rhs) const;

    Complex operator/(const Complex &rhs) const;

    Complex &operator+=(const Complex &rhs);

    Complex &operator-=(const Complex &rhs);

    Complex &operator*=(const Complex &rhs);

    Complex &operator/=(const Complex &rhs);

    bool operator==(const Complex &rhs) const;

    bool operator!=(const Complex &rhs) const;

    std::string toString() const;
};

#endif //BIGINTEGER_COMPLEX_H
