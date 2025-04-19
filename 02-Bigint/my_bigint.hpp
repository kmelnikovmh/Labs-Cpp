#include <cmath>
#include <sstream>
#include <string>
#include <vector>

namespace lab_bigint {
class bigint {
private:
    int BASE = 10;
    std::vector<int> digits;

    void remove_lead_zeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

public:
    bigint() {
        digits.push_back(0);
    }

    explicit bigint(const std::string& str) {
        int digit_length = static_cast<int>(log10(BASE));
        digits.reserve(str.size() / digit_length + 1);
        int i = str.size() - digit_length;
        for (; i >= 0; i -= digit_length) {
            digits.push_back(stoi(str.substr(i, digit_length)));
        }
        if (i < 0) {
            int j = digit_length;
            while (i < 0) {
                ++i;
                --j;
            }
            digits.push_back(atoi(str.substr(i, j).c_str()));
        }
        remove_lead_zeros();
    }

    // cppcheck-suppress[noExplicitConstructor]
    bigint(unsigned int num) {
        while (num > 0) {
            digits.push_back(static_cast<int>(num % BASE));
            num = num / BASE;
        }
        if (digits.empty()) {
            digits.push_back(0);
        }
        remove_lead_zeros();
    }

    friend bool operator==(const bigint& lhs, const bigint& rhs) {
        if (lhs.digits.size() != rhs.digits.size()) {
            return false;
        }
        for (size_t i = 0; i < lhs.digits.size(); ++i) {
            if (lhs.digits[i] != rhs.digits[i]) {
                return false;
            }
        }
        return true;
    }

    friend bool operator!=(const bigint& lhs, const bigint& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const bigint& lhs, const bigint& rhs) {
        int l = lhs.digits.size();
        int r = rhs.digits.size();
        if (l != r) {
            return l < r;
        } else {
            for (int i = l - 1; i >= 0; --i) {
                if (lhs.digits[i] != rhs.digits[i]) {
                    return lhs.digits[i] < rhs.digits[i];
                }
            }
        }
        return false;
    }

    friend bool operator>(const bigint& lhs, const bigint& rhs) {
        return (!(lhs < rhs)) && lhs != rhs;
    }

    friend bool operator>=(const bigint& lhs, const bigint& rhs) {
        return lhs > rhs || lhs == rhs;
    }

    friend bool operator<=(const bigint& lhs, const bigint& rhs) {
        return lhs < rhs || lhs == rhs;
    }

    std::string to_string() const {
        std::stringstream stream;
        for (int i = digits.size() - 1; i >= 0; --i) {
            std::string num = std::to_string(digits[i]);
            while (num.size() < log10(BASE)) {
                num = "0" + num;
            }
            stream << num;
        }
        std::string str = stream.str();
        size_t j = 0;
        while (j < str.size() && str[j] == '0') {
            ++j;
        }
        if (j != 0 && j != str.size()) {
            str = str.substr(j);
        } else if (j == str.size()) {
            str = "0";
        }
        return str;
    }

    explicit operator unsigned() {
        std::string str = "";
        for (int i = digits.size() - 1; i >= 0; --i) {
            str = str + std::to_string(digits[i]);
        }
        return atoi(str.c_str());
    }

    bigint& operator+=(const bigint& rhs) {
        int carry = 0;
        int lhsSize = digits.size(), rhsSize = rhs.digits.size();
        if (lhsSize < rhsSize) {
            for (int i = 0; i <= rhsSize - lhsSize - 1; ++i) {
                digits.push_back(0);
            }
            lhsSize = rhsSize;
        }
        for (int i = 0; i < lhsSize; ++i) {
            int sum;
            if (i < rhsSize) {
                sum = (digits[i] + rhs.digits[i]) + carry;
            } else {
                sum = (digits[i] + 0) + carry;
            }
            carry = sum / BASE;
            digits[i] = static_cast<int>(sum % BASE);
        }
        if (carry) {
            digits.push_back(carry);
        }
        return *this;
    }

    friend bigint operator+(bigint lhs, const bigint& rhs) {
        return lhs += rhs;
    }

    bigint& operator++() {
        (*this) += 1;
        return *this;
    }

    bigint operator++(int) {
        bigint result = *this;
        ++*this;
        return result;
    }

    bigint& operator-=(const bigint& rhs) {
        int lhsSize = digits.size();
        int rhsSize = rhs.digits.size();
        int borrow = 0;
        int sum;
        for (int i = 0; i < lhsSize; i++) {
            if (i < rhsSize) {
                sum = digits[i] - rhs.digits[i] + borrow;
            } else {
                sum = digits[i] + borrow;
            }
            if (sum < 0) {
                sum += BASE, borrow = -1;
            } else {
                borrow = 0;
            }
            digits[i] = sum;
        }
        remove_lead_zeros();
        return *this;
    }

    friend bigint operator-(bigint lhs, const bigint& rhs) {
        return lhs -= rhs;
    }

    bigint& operator--() {
        (*this) -= 1;
        return *this;
    }

    bigint operator--(int) {
        bigint result = *this;
        --*this;
        return result;
    }

    friend std::ostream& operator<<(std::ostream& ostream, const bigint& lhs) {
        return ostream << lhs.to_string();
    }

    friend std::istream& operator>>(std::istream& istream, bigint& lhs) {
        std::string str;
        istream >> str;
        lhs = bigint(str);
        return istream;
    }

    // TODO: *, /, base!=10^k
};
}  // namespace lab_bigint
