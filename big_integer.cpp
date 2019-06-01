#include <bits/stdc++.h>

#include "big_integer.h"

typedef uint32_t ul;
typedef uint64_t ull;
typedef long long ll;

const ul BASE = 32;
const ul MAX_UL = (ul(0)) - 1;
const ull M = (ull(1) << ull(32));


std::string bin_to_dec(std::string s) {
    size_t index = 0;
    while (index < s.size() && s[index] == '0') {
        index++;
    }
    if (index == s.size()) {
        return "0";
    }
    std::string res = "1";
    index++;
    for (; index < s.size(); ++index) {
        std::string res2;
        size_t carry = 0;
        for (size_t i = res.size() - 1;; --i) {
            int x = res[i] - '0';
            size_t cur = x * 2 + carry;
            res2 += char(cur % 10 + '0');
            carry = cur / 10;
            if (i == 0) {
                break;
            }
        }
        if (carry) {
            res2 += '1';
        }
        reverse(res2.begin(), res2.end());
        res = res2;
        if (s[index] == '0') {
            continue;
        }
        size_t i = res.size() - 1;
        while (i > 0 && res[i] == '9') {
            res[i] = '0';
            i--;
        }
        if (i == 0) {
            if (res[i] != '9') {
                res[i]++;
            } else {
                res[i] = '0';
                res = '1' + res;
            }
        } else {
            res[i]++;
        }
    }
    return res;
}

std::string to_string(big_integer const &a) {
    bool sign = 0;
    big_integer res = a;
    if (a.get_sign()) {
        sign = 1;
        res = -res;
    }
    res.fixe();
    std::string bin = "";
    for (size_t i = 0; i < res.data.size(); ++i) {
        for (size_t j = 0; j < BASE; ++j) {
            bin += char(((res.data[i] >> j) & 1) + '0');
        }
    }
    reverse(bin.begin(), bin.end());
    return (sign ? "-" : "") + bin_to_dec(bin);
}

big_integer::big_integer() {
    data.resize(1);
    data[0] = 0;
}

big_integer::~big_integer() {
    data.resize(0);
}

big_integer::big_integer(int a) {
    data.resize(1);
    data[0] = ul(a);
}

big_integer::big_integer(ul a) {
    data.resize(1);
    data[0] = a;
    if (get_sign()) {
        data.push_back(0);
    }
}

//big_integer::big_integer(big_integer const &other) {
//    data.resize(other.data.size());
//    for (size_t i = 0; i < data.size(); ++i) {
//        data[i] = other.data[i];
//    }
//}
big_integer::big_integer(big_integer const &other) : data(other.data) {}
big_integer big_integer::operator+() const {
    return *this;
}

big_integer::big_integer(std::string const &s) {
//    this->data.resize(0);
    bool sign = false;
    std::string tmp;
    if (s[0] == '-') {
        sign = true;
        tmp = s.substr(1);
    } else {
        tmp = s;
    }
    while (!tmp.empty()) {
        ul ul_number = 0;
        for (size_t i = 0; i < BASE; ++i) {
            if (!tmp.empty()) {
                ul_number = ul_number | (ul((tmp[tmp.size() - 1] - '0') % 2) << ul(i));
                std::string tmp_div2;
                ul cur = 0;
                for (char c : tmp) {
                    cur = cur * 10 + (c - '0');
                    if (cur > 1) {
                        tmp_div2 += char((cur / 2) + '0');
                        cur %= 2;
                    } else if (!tmp_div2.empty()) {
                        tmp_div2 += '0';
                    }
                }
                tmp = tmp_div2;
            } else {
                break;
            }
        }
        this->data.push_back(ul_number);
    }
    if (get_sign()) {
        data.push_back(0);
    }
    if (sign) {
        *this = -*this;
    }
}


big_integer operator+(big_integer a, big_integer const &b) {
    big_integer a1 = a;
    big_integer b1 = b;
    a1.add(b1, 0);
    return a1;
}


big_integer operator-(big_integer a, big_integer const &b) {
    return a + (-b);
}

//big_integer &big_integer::operator=(big_integer const &other) {
//    if (this != &other) {
//        data.resize(other.data.size());
//        for (size_t i = 0; i != data.size(); ++i)
//            data[i] = other.data[i];
//    }
//    return *this;
//}

big_integer &big_integer::operator=(big_integer const &other) {
//    sign = other.sign;
    data = other.data;
    return *this;
}
big_integer big_integer::operator~() const {
    my_vector<ul> tmp(data);
    for (size_t i = 0; i < data.size(); ++i) {
        tmp[i] = ~data[i];
    }
    big_integer res;
    res.data = tmp;
    return res;
}


bool big_integer::get_sign() const {
    if (data.size() != 0) {
        ul x = data[data.size() - 1];
        return (x >> ul(31)) > 0;
    } else {
        return false;
    }
}

ul big_integer::get_digit(size_t ind) const {
    if (ind < this->data.size()) {
        return data[ind];
    } else if (get_sign()) {
        return MAX_UL;
    } else {
        return 0;
    }
}

void big_integer::fixe() {
    while (data.size() > 1 && (((data.back() == MAX_UL && (data[data.size() - 2] >> 31)) || (data.back() == 0 && !(data[data.size() - 2] >> 31))))) {
        data.pop_back();
    }
}

big_integer big_integer::operator-() const {
    if (!this->data.size()) {
        return *this;
    }
    big_integer res =  ~(*this) + ul(1);
    return res;
}

big_integer operator*(big_integer a, big_integer const &b) {
    int sign = a.get_sign() ^b.get_sign();
    big_integer b1 = b;
    if (a.get_sign()) {
        a = -a;
    }
    if (b1.get_sign()) {
        b1 = -b1;
    }
    big_integer res;
    res.data.resize(a.data.size() + b1.data.size() + 1);
    for (size_t i = 0; i < a.data.size(); ++i) {
        ull c = 0;
        ull mul = 0;
        ull tmp = 0;
        for (size_t j = 0; j < b1.data.size(); ++j) {
            size_t k = i + j;
            mul = (ull) a.data[i] * b1.data[j];
            tmp = (mul & MAX_UL) + res.data[k] + c;
            res.data[k] = ul(tmp);
            c = (mul >> BASE) + (tmp >> BASE);
        }
        res.data[i + b1.data.size()] += ul(c);
    }

    if (res.get_sign()) {
        res.data.push_back(0);
    }
    if (sign) {
        res = -res;
    }
    res.fixe();
    return res;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    big_integer a = *this;
    a = a - rhs;
    *this = a;
    return *this;
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    *this = *this * rhs;
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    big_integer a = *this;
    a = a + rhs;
    *this = a;
    return *this;
}

big_integer &big_integer::operator++() {
    return *this += 1;
}

big_integer big_integer::operator++(int) {
    big_integer r(*this);
    *this += 1;
    return r;
}

big_integer &big_integer::operator--() {
    return *this -= 1;
}


big_integer big_integer::operator--(int) {
    big_integer r = *this;
    *this -= 1;
    return r;
}

bool operator==(big_integer const &a, big_integer const &b) {
    return (a.data == b.data || (a.is_zero() && b.is_zero()));
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.get_sign() && !b.get_sign()) {
        return 1;
    }
    if (!a.get_sign() && b.get_sign()) {
        return 0;
    }
    big_integer a1 = a, b1 = b;
    bool sign = a.get_sign() | b.get_sign();
    if (a.get_sign()) {
        a1 = -a1;
    }
    if (b.get_sign()) {
        b1 = -b1;
    }
    if (a.data.size() < b.data.size()) {
        return !sign;
    }
    if (b.data.size() < a.data.size()) {
        return sign;
    }
    for (int i = (int) a.data.size() - 1; i >= 0; i--) {
        if (a.data[i] < b.data[i]) {
            return !sign;
        }
        if (a.data[i] > b.data[i]) {
            return sign;
        }
    }
    return 0;
}


bool operator>(big_integer const &a, big_integer const &b) {
    return (b < a);
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return !(a > b);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

big_integer operator%(big_integer a, big_integer const &b) {
    auto res = a.div_mod(b).second;
    if (a < 0) {
        res = -res;
    }
    return res;
}

std::pair<big_integer, ul> div_long_short(big_integer const &a, ul b) {
    big_integer a1 = a;
    bool sign = a1.get_sign();
    if (sign) {
        a1 = -a1;
    }
    big_integer res = big_integer();
    //

    ul carry = 0;
    for (size_t i = a1.data.size() - 1;; --i) {
        ull cur = ull(a1.data[i]) + ull(carry) * (ull(1) << BASE);
        carry = cur % (ull) b;
        res.data.push_back(cur / b);
        if (i == 0) {
            break;
        }
    }
    for (size_t i = 0; i < res.data.size() / 2; ++i) {
        auto kek = res.data[i];
        res.data[i] = res.data[res.data.size() - i - 1];
        res.data[res.data.size() - i - 1] = kek;
        // std::swap(res.data[i], res.data[res.data.size() - i - 1]);
    }
    while (res.data.size() > 1 && res.data.back() == 0) {
        res.data.pop_back();
    }
    if (res.get_sign()) {
        res.data.push_back(0);
    }
    if (sign) {
        res = -res;
    }
    return {res, carry};
}

big_integer operator/(big_integer const &a, ul b) {
    return a / b;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return a.div_mod(b).first;
}

big_integer operator&(big_integer a, big_integer const &b) {
    size_t n = std::max(a.data.size(), b.data.size()), m = std::min(a.data.size(), b.data.size());
    big_integer res;
    res.data.resize(n);
    for (size_t i = 0; i < m; i++) {
        res.data[i] = a.get_digit(i) & b.get_digit(i);
    }
    for (size_t i = m; i < n; i++) {
        res.data[i] = a.get_digit(i) & b.get_digit(i);
    }
    bool sign = (a.get_sign() & b.get_sign());
    if (res.get_sign() && !sign) {
        res.data.push_back(0);
    } else if (!res.get_sign() && sign) {
        res.data[res.data.size() - 1] |= (ul(1) << ul(BASE - 1));
    }
    res.fixe();
    return res;
}

big_integer operator|(big_integer a, big_integer const &b) {
    size_t n = std::max(a.data.size(), b.data.size()), m = std::min(a.data.size(), b.data.size());
    big_integer res;
    res.data.resize(n);
    for (size_t i = 0; i < m; i++) {
        res.data[i] = a.get_digit(i) | b.get_digit(i);
    }
    for (size_t i = m; i < n; i++) {
        res.data[i] = a.get_digit(i) | b.get_digit(i);
    }
    bool sign = (a.get_sign() | b.get_sign());
    if (res.get_sign() && !sign) {
        res.data.push_back(0);
    } else if (!res.get_sign() && sign) {
        res.data[res.data.size() - 1] |= (ul(1) << ul(BASE - 1));
    }
    res.fixe();
    return res;
}

big_integer operator^(big_integer a, big_integer const &b) {
    size_t n = std::max(a.data.size(), b.data.size()), m = std::min(a.data.size(), b.data.size());
    big_integer res;
    res.data.resize(n);
    for (size_t i = 0; i < m; i++) {
        res.data[i] = a.get_digit(i) ^ b.get_digit(i);
    }
    for (size_t i = m; i < n; i++) {
        res.data[i] = a.get_digit(i) ^ b.get_digit(i);
    }
    bool sign = (a.get_sign() ^ b.get_sign());
    if (res.get_sign() && !sign) {
        res.data.push_back(0);
    } else if (!res.get_sign() && sign) {
        res.data[res.data.size() - 1] |= (ul(1) << ul(BASE - 1));
    }
    res.fixe();
    return res;
}

big_integer operator<<(big_integer a, int b) {
    if (b == 0) {
        return a;
    }
    size_t div = b / BASE;
    size_t mod = b % BASE;
    size_t new_size = a.data.size() + div + 1;
    big_integer res = big_integer();
    res.data.resize(new_size);
    res.data[div] = ul((ull) a.data[0] << mod);
    for (size_t i = div + 1; i < res.data.size(); ++i) {
        ull x = 0, y = 0;
        x = (ull) a.get_digit(i - div) << mod;

        y = (ull) a.get_digit(i - div - 1) >> (BASE - mod);
        res.data[i] = ul(x + y);
    }
    if (res.get_sign() && !a.get_sign()) {
        res.data.push_back(0);
    } else if (!res.get_sign() && a.get_sign()) {
        res.data[res.data.size() - 1] |= (ul(1) << ul(BASE - 1));
    }
    res.fixe();
    return res;
}

big_integer operator>>(big_integer a, int b) {
    if (b == 0) {
        return a;
    }
    size_t div = b / BASE;
    size_t mod = b % BASE;
    size_t new_size = 0;
    if (div < a.data.size()) {
        new_size = a.data.size() - div;
    }
    big_integer res;
    res.data.resize(new_size);
    for (size_t i = 0; i < res.data.size(); ++i) {
        ull x = 0, y = 0;
        x = (ull) a.get_digit(i + div) >> mod;

        y = (ull) a.get_digit(i + div + 1) << (BASE - mod);
        res.data[i] = ul(x + y);
    }
    if (res.get_sign() && !a.get_sign()) {
        res.data.push_back(0);
    } else if (!res.get_sign() && a.get_sign()) {
        res.data[res.data.size() - 1] |= (ul(1) << ul(BASE - 1));
    }
    res.fixe();
    return res;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    *this = *this / rhs;
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    *this = *this % rhs;
    return *this;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    *this = *this & rhs;
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    *this = *this | rhs;
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    *this = *this ^ rhs;
    return *this;
}

big_integer &big_integer::operator<<=(int rhs) {
    *this = *this << rhs;
    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    *this = *this >> rhs;
    return *this;
}

bool big_integer::is_zero() const {
    return (this->data.size() == 0 || (this->data.size() == 1 && this->data[0] == 0));
}

void big_integer::add(const big_integer &a, size_t pos) {
    ul carry = 0;
    size_t length = std::max(this->data.size(), a.data.size() + pos) + 1;
    for (size_t i = data.size(); i < length; ++i) {
        data.push_back(get_sign() ? MAX_UL : 0);
    }
    big_integer a1 = a;
    for (size_t i = a.data.size(); i < length; ++i) {
        a1.data.push_back(a1.get_sign() ? MAX_UL : 0);
    }
    for (size_t i = 0; i < a1.data.size(); ++i) {
        ull sum = (ull) get_digit(i + pos) + a1.get_digit(i) + carry;
        data[i + pos] = (ul) sum;
        carry = (ul) (sum >> BASE);
    }
    for (size_t i = a1.data.size() + pos; i < length; ++i) {
        ull sum = (ull) get_digit(i) + carry;
        data[i] = (ul) sum;
        carry = (ul) (sum >> BASE);
    }
    fixe();
}

std::pair<big_integer, big_integer> big_integer::div_mod(const big_integer &a) const {
    big_integer v(a);
    big_integer u(*this);
    bool sign = v.get_sign() ^u.get_sign();
    if (v.is_zero()) {
        return {v, u};
    }
    if (v.get_sign()) {
        v = -v;
    }
    if (u.get_sign()) {
        u = -u;
    }
    if (u < v) {
        return std::make_pair(0, *this);
    }
    size_t n = (v.data.back() == 0 ? v.data.size() - 1 : v.data.size());
    size_t m = u.data.size() - n;
    int j = (int) m;
    if (n == 1) {

        auto res = div_long_short(u, v.data[0]);
        if (res.first.get_sign()) {
            res.first.data.push_back(0);
        }
        if (sign) {
            res.first = -res.first;
        }
        return res;
    }
    if (u.data.size() != n + m + 1) {
        u.data.push_back(0);
    }
    uint32_t d = (uint32_t) (M / ((uint64_t) (v.data[n - 1] + 1)));

    u *= d;
    v *= d;
    big_integer q_ans(0);
    q_ans.data.resize(m + 1);
    while (j >= 0) {
        uint64_t tmp = (uint64_t) u.get_digit(j + n) * M + u.data[j + n - 1];
        uint64_t q = tmp / v.data[n - 1];
        uint64_t r = tmp % v.data[n - 1];
        while (q == M || (r < M && q * v.get_digit(n - 2) > M * r + u.get_digit(j + n - 2))) {
            --q;
            r += v.data[n - 1];
        }
        big_integer w(v);
        w *= (ul) q;
        if (u.sub_prefix(w, j)) {
            q--;
            u.add(v, j);
        }
        q_ans.data[j] = (uint32_t) q;
        j--;
    }
    u = div_long_short(u, d).first;
    if (q_ans.get_sign()) {
        q_ans.data.push_back(0);
    }
    if (sign) {
        q_ans = -q_ans;
    }
    q_ans.fixe();
    u.fixe();
    return std::make_pair(q_ans, u);
}

int big_integer::sub_prefix(big_integer const &b, size_t ind) {
    uint32_t carry = 0;
    bool sign = b.get_sign();
    ul len = (std::max(data.size(), b.data.size() + ind));
    for (size_t i = data.size(); i < len; ++i) {
        data.push_back(get_sign() ? MAX_UL : 0);
    }
    for (size_t i = 0; i < b.data.size(); i++) {
        if (data[ind + i] - carry >= b.data[i]) {
            data[ind + i] = (uint32_t) (data[ind + i] - carry - b.data[i]);
            carry = 0;
        } else {
            data[ind + i] += M - carry - b.data[i];
            carry = 1;
        }
    }
    if (carry != 0 && b.data.size() < data.size()) {
        if (data[b.data.size() + ind] != 0) {
            data[b.data.size() + ind]--;
            carry = 0;
        } else {
            data[b.data.size() + ind] = (uint32_t) M - 1;
        }
    }
    if (!sign && get_sign()) {
        data.push_back(0);
    }
    return carry;
}
