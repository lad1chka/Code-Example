#include <algorithm>
#include <iostream>
#include <vector>

const int BASE = 1000000;

int size_digit() {
    int x = 1;
    int size_str = 0;
    while (x != BASE) {
        x *= 10;
        size_str++;
    }
    return size_str;
}

class bigint {
private:
    std::vector<unsigned int> digits;

public:
    bigint() : digits{0} {
    }

    bigint(unsigned int digit) {  // cppcheck-suppress noExplicitConstructor
        while (digit > 0) {
            digits.push_back(digit % BASE);
            digit /= BASE;
        }
    }

    explicit bigint(std::string str) {
        std::reverse(str.begin(), str.end());
        std::string digit;
        auto new_size_digits =
            static_cast<unsigned int>((str.size() - 1) / size_digit() + 1);
        digits.resize(new_size_digits);
        int j = 0;
        for (int i = 0; i < static_cast<int>(str.size()); i++) {
            while (j < size_digit() && i < static_cast<int>(str.size())) {
                digit += str[i];
                i++;
                j++;
            }
            std::reverse(digit.begin(), digit.end());
            digits[(i - 1) / size_digit()] =
                static_cast<unsigned int>(std::stoll(digit));
            digit.clear();
            i--;
            j = 0;
        }
        while (digits[new_size_digits - 1] == 0 && new_size_digits > 1) {
            digits.pop_back();
            new_size_digits--;
        }
    }

    [[nodiscard]] std::string to_string() const {
        std::string str;
        for (unsigned int digit : digits) {
            std::string digit1 = std::to_string(digit);
            std::reverse(digit1.begin(), digit1.end());
            while (static_cast<int>(digit1.size()) < size_digit()) {
                digit1 += '0';
            }
            str += digit1;
        }
        while (str[str.size() - 1] == '0' && str.size() > 1) {
            str.pop_back();
        }
        std::reverse(str.begin(), str.end());
        return str;
    }

    explicit operator unsigned int() const {
        unsigned int digit = 0;
        for (unsigned int value = 0; value < digits.size(); value++) {
            digit = digit * BASE + digits[digits.size() - value - 1];
        }
        return digit;
    }

    friend bool operator==(const bigint &left, const bigint &right);

    friend bool operator!=(const bigint &left, const bigint &right);

    friend bool operator<(const bigint &left, const bigint &right);

    friend bool operator<=(const bigint &left, const bigint &right);

    friend bool operator>(const bigint &left, const bigint &right);

    friend bool operator>=(const bigint &left, const bigint &right);

    friend bigint operator+(const bigint &left, const bigint &right);

    friend bigint &operator+=(bigint &left, const bigint &right);

    friend bigint operator-(const bigint &left, const bigint &right);

    friend bigint &operator-=(bigint &left, const bigint &right);

    bigint &operator++() {
        *this += 1;
        return *this;
    }

    bigint operator++(int) {
        bigint old = *this;
        *this += 1;
        return old;
    }

    bigint &operator--() {
        *this -= 1;
        return *this;
    }

    bigint operator--(int) {
        bigint old = *this;
        *this -= 1;
        return old;
    }

    friend std::ostream &
    operator<<(std::ostream &os, const bigint &cout_bigint) {
        unsigned int size_digit_in_cout_bigint = BASE;
        for (int i = static_cast<int>(cout_bigint.digits.size() - 1); i >= 0;
             i--) {
            if (cout_bigint.digits[i] < BASE / 10 &&
                i != static_cast<int>(cout_bigint.digits.size() - 1) &&
                cout_bigint.digits[i] != 0) {
                while (cout_bigint.digits[i] <
                       size_digit_in_cout_bigint / 10 - 1) {
                    size_digit_in_cout_bigint /= 10;
                    os << 0;
                }
                size_digit_in_cout_bigint = BASE;
            }
            if (cout_bigint.digits[i] == 0) {
                if (static_cast<int>(cout_bigint.digits.size()) == 1) {
                    os << 0;
                    return os;
                } else {
                    for (int j = 1; j < size_digit(); j++) {
                        os << 0;
                    }
                }
            }
            os << cout_bigint.digits[i];
        }
        return os;
    }

    friend std::istream &operator>>(std::istream &input, bigint &cin_bigint) {
        std::string cin_string;
        input >> cin_string;
        cin_bigint = bigint(cin_string);
        return input;
    }
};

bool operator==(const bigint &left, const bigint &right) {
    return (left.digits == right.digits);
}

bool operator!=(const bigint &left, const bigint &right) {
    return !(left == right);
}

bool operator<(const bigint &left, const bigint &right) {
    std::string left1 = left.to_string();
    std::string right1 = right.to_string();
    if (left1.size() > right1.size()) {
        return false;
    } else if (left1.size() < right1.size()) {
        return true;
    } else {
        return std::lexicographical_compare(
            left1.begin(), left1.end(), right1.begin(), right1.end()
        );
    }
}

bool operator>(const bigint &left, const bigint &right) {
    return !(left < right || left == right);
}

bool operator<=(const bigint &left, const bigint &right) {
    return !(left > right);
}

bool operator>=(const bigint &left, const bigint &right) {
    return !(left < right);
}

bigint operator+(const bigint &left, const bigint &right) {
    bigint sum = left;
    sum += right;
    return sum;
}

bigint &operator+=(bigint &left, const bigint &right) {
    unsigned int size_sum = static_cast<unsigned int>(
        std::max(left.digits.size(), right.digits.size())
    );
    bigint right1 = right;
    left.digits.resize(size_sum, 0);
    right1.digits.resize(size_sum, 0);
    for (int i = 0; i < static_cast<int>(size_sum); i++) {
        left.digits[i] += right1.digits[i];
        if (left.digits[i] > BASE - 1) {
            if (i == static_cast<int>(size_sum) - 1) {
                left.digits.resize(size_sum + 1, 0);
            }
            left.digits[i] -= BASE;
            left.digits[i + 1]++;
        }
    }
    return left;
}

bigint operator-(const bigint &left, const bigint &right) {
    bigint sum = left;
    sum -= right;
    return sum;
}

bigint &operator-=(bigint &left, const bigint &right) {
    bigint right1 = right;
    auto max_size = static_cast<unsigned int>(left.digits.size());
    right1.digits.resize(max_size, 0);
    for (int i = 0; i < static_cast<int>(max_size); i++) {
        if (left.digits[i] >= right1.digits[i]) {
            left.digits[i] -= right1.digits[i];
        } else {
            left.digits[i] += BASE;
            int j = i + 1;
            while (left.digits[j] == 0) {
                left.digits[j] = BASE - 1;
                j++;
            }
            left.digits[j] -= 1;
            left.digits[i] -= right1.digits[i];
        }
    }
    while (left.digits[max_size - 1] == 0 && left.digits.size() > 1) {
        left.digits.pop_back();
        max_size--;
    }
    return left;
}

/*int main(){

}*/

