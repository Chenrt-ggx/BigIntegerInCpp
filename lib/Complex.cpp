#include "Complex.h"

Complex::Complex(double real, double imag) {
    this->real = real, this->imag = imag;
}

void Complex::update(double newReal, double newImag) {
    this->real = newReal, this->imag = newImag;
}

double Complex::getReal() const {
    return this->real;
}

double Complex::getImag() const {
    return this->imag;
}

double Complex::length() const {
    return sqrt(this->real * this->real + this->imag * this->imag);
}

Complex Complex::operator+(const Complex &rhs) const {
    return Complex(this->real + rhs.real, this->imag + rhs.imag);
}

Complex Complex::operator-(const Complex &rhs) const {
    return Complex(this->real - rhs.real, this->imag - rhs.imag);
}

Complex Complex::operator*(const Complex &rhs) const {
    double newReal = this->real * rhs.real - this->imag * rhs.imag;
    double newImag = this->imag * rhs.real + this->real * rhs.imag;
    return Complex(newReal, newImag);
}

Complex Complex::operator/(const Complex &rhs) const {
    double newReal = this->real * rhs.real + this->imag * rhs.imag;
    double newImag = this->imag * rhs.real - this->real * rhs.imag;
    double div = rhs.real * rhs.real + rhs.imag * rhs.imag;
    if (div == 0) {
        throw ComplexException("Divide Zero");
    }
    return Complex(newReal / div, newImag / div);
}

Complex &Complex::operator+=(const Complex &rhs) {
    this->real += rhs.real, this->imag += rhs.imag;
    return *this;
}

Complex &Complex::operator-=(const Complex &rhs) {
    this->real -= rhs.real, this->imag -= rhs.imag;
    return *this;
}

Complex &Complex::operator*=(const Complex &rhs) {
    double newReal = this->real * rhs.real - this->imag * rhs.imag;
    double newImag = this->imag * rhs.real + this->real * rhs.imag;
    this->real = newReal, this->imag = newImag;
    return *this;
}

Complex &Complex::operator/=(const Complex &rhs) {
    double newReal = this->real * rhs.real + this->imag * rhs.imag;
    double newImag = this->imag * rhs.real - this->real * rhs.imag;
    double div = rhs.real * rhs.real + rhs.imag * rhs.imag;
    if (div == 0) {
        throw ComplexException("Divide Zero");
    }
    this->real = newReal / div, this->imag = newImag / div;
    return *this;
}

bool Complex::operator==(const Complex &rhs) const {
    return fabs(this->real - rhs.real) < eps && fabs(this->imag - rhs.imag) < eps;
}

bool Complex::operator!=(const Complex &rhs) const {
    return fabs(this->real - rhs.real) > eps && fabs(this->imag - rhs.imag) > eps;
}

std::string doubleToString(double value) {
    if (floor(value) == ceil(value)) {
        return std::to_string((int) value);
    }
    std::string str = std::to_string(value);
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    return str;
}

std::string Complex::toString() const {
    if (this->real == 0 && this->imag == 0) {
        return "0";
    } else if (this->real == 0) {
        return this->imag == 1 ? "i" : this->imag == -1 ? "-i" : doubleToString(this->imag) + "i";
    } else if (this->imag == 0) {
        return doubleToString(this->real);
    }
    return doubleToString(this->real) + (this->imag > 0 ? " + " : " - ") +
           (fabs(this->imag) == 1 ? "i" : doubleToString(fabs(this->imag)) + "i");
}
