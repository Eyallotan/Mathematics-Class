#include <iostream>
#include "Complex.h"

using namespace MtmMath;

Complex::Complex(double re, double im) : re(re), im(im) {}

Complex& Complex::operator+=(const Complex &c) {
    re += c.re;
    im += c.im;
    return *this;
}

Complex& Complex::operator-=(const Complex &c) {
    return *this += -c;
}

Complex& Complex::operator*=(const Complex& c){
    Complex tmp(re,im);
    re = (tmp.re*c.re-tmp.im*c.im);
    im = (tmp.re*c.im+tmp.im*c.re);
    return *this;
}

Complex Complex::operator-() const {
    return Complex(-re, -im);
}

bool MtmMath::operator==(const Complex &a, const Complex &b) {
    return a.re == b.re && a.im == b.im;
}

bool MtmMath::operator!=(const Complex &a, const Complex &b) {
    return a.re != b.re || a.im != b.im;
}

ostream& MtmMath::operator<<(ostream& os, const Complex& c) {
    const char* sign = c.im < 0 ? "-" : "+";
    return os << c.re << sign << c.im << "i";
}

istream& MtmMath::operator>>(istream& is, Complex& c) {
    return is >> c.re >> c.im;
}

Complex MtmMath::operator+(const Complex& a, const Complex& b) {
    return Complex(a) += b;
}

Complex MtmMath::operator-(const Complex& a, const Complex& b) {
    return Complex(a) -= b;
}

Complex MtmMath::operator*(const Complex& a, const Complex& b){
    return Complex(a) *= b;
}




