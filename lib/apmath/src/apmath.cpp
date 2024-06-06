#include "apmath.hpp"


namespace apmath {
    numnode::numnode(uint8_t val) {
        this->value = val;
        this->prev = nullptr;
        this->next = nullptr;
    }

    numnode::numnode() {
        this->value = 0;
        this->prev = nullptr;
        this->next = nullptr;
    }

    numnode::numnode(uint8_t val, struct numnode *prev, struct numnode *next) {
        this->value = val;
        this->prev = prev;
        this->next = next;
    }

    Number::Number(const char *input) {
        this->m_Head = nullptr;
        this->m_Rear = nullptr;

        switch (input[0]) {
        case '-':
            this->m_Sign = numsign_t::NEGATIVE;
            input++;
            break;
        
        case '+':
            input++;
        default:
            this->m_Sign = numsign_t::POSITIVE;
            break;
        }

        for (; *input == '0'; input++);
        for (char *cp = (char *) input, ch = *cp; (ch = *cp) != 0; cp++) {
            uint8_t digit = (const uint8_t) ch - '0';
            numnode_t *tmp = new numnode_t(digit);

            if (this->m_Head == nullptr) {
                this->m_Head = tmp;
                this->m_Rear = tmp;
                continue;
            }

            tmp->prev = this->m_Rear;
            this->m_Rear->next = tmp;
            this->m_Rear = tmp;
        }

        if (this->m_Head == nullptr) {
            numnode_t *n = new numnode_t(0);
            this->m_Head = n;
            this->m_Rear = 0;
            this->m_Sign = numsign_t::POSITIVE;
        }
    }

    Number::Number(const Number &other) {
        this->m_Head = nullptr;
        this->m_Rear = nullptr;
        this->m_Sign = other.m_Sign;

        numnode_t *original = other.m_Head;

        while (original != nullptr) {
            numnode_t *tmp = new numnode_t(original->value);
         
            original = original->next;

            if (this->m_Head == nullptr) {
                this->m_Head = tmp;
                this->m_Rear = tmp;
                continue;
            }

            tmp->prev = this->m_Rear;
            this->m_Rear->next = tmp;
            this->m_Rear = tmp;
        }
    }

    Number::Number() {
        this->m_Head = nullptr;
        this->m_Rear = nullptr;
        this->m_Sign = numsign_t::POSITIVE;
    }

    Number::~Number() {
        numnode_t *ptr = this->m_Rear;

        while (ptr != nullptr) {
            numnode_t *prev = ptr->prev;
            delete ptr;
            ptr = prev;
        }
    }

    Number Number::operator+(const Number other) {
        Number a_base = Number(*this);
        Number b_base = Number(other);

        Number *a = &a_base;
        Number *b = &b_base;

        result_t r = (result_t) {
            .new_head = a->m_Head,
            .new_rear = a->m_Rear,
            .new_sign = a->m_Sign
        };

        a->_add_nodes(a->m_Rear, b->m_Rear, a->m_Sign, b->m_Sign, &r);
        a->m_Head = r.new_head;
        a->m_Sign = r.new_sign;
        a->m_Rear = r.new_rear;

        return *a;
    }

    Number Number::operator-(const Number other) {
        this->_negate();
        Number r = *this + other;
        this->_negate();
        return r;
    }

    const char *Number::c_str() {
        int len = this->_length();   // length of the number
        int off = 0;                // offset (1 if '-' is present, else 0)
        char sign = 0;              // Sign char

        if (this->m_Sign == numsign_t::NEGATIVE) {
            sign = '-';
            off = 1;
        }

        char *buf = new char[len + 1];
        buf[0] = sign; // place sign if present
        buf[len] = 0;  // terminate C String (null termination)
        
        numnode_t *tmp = this->m_Head;

        while (tmp != nullptr) {
            buf[off] = (char) (tmp->value + '0');
            tmp = tmp->next;
            off++;
        }

        return buf;
    }

    int Number::_length() {
        numnode_t *tmp = this->m_Head;
        int len = 0;

        while (tmp != nullptr) {
            tmp = tmp->next;
            len++;
        }

        // len is incremented by 1 if negative
        // in order to make room for the '-'
        len += this->m_Sign == numsign_t::NEGATIVE;
        return len;
    }

    // WARNINGS: This function is not safe
    // this function may affect the input you pass to it
    // MAKE COPIES!
    void Number::_add_nodes(numnode_t *a, numnode_t *b, numsign_t a_sign, numsign_t b_sign, result_t *r) {
        // If a or b point to NULL, then return
        // a or b may be NULL if there's nothing to add enymore
        // or if the method has been invoked on unallocated values
        if (a == nullptr || b == nullptr)
            return;

        // The sign coefficient
        // This number may be 1 or -1
        // depending on the sign
        int8_t sign_coeff = 1;

        // If the signs of a and b are conflicting
        // e.g. if a is positive and b is negative
        if (b_sign != a_sign) {
            sign_coeff = -1;
            
            // If `a` and `b` point to the tail of a number
            // and the absolute value of `b` is greater than `a`
            if (a->next == nullptr && b->next == nullptr
                && this->_greater(a, b)) {
                // The larger number determins the sign
                // e.g. 1 -2 (|2| > |1|), thus sign is - (negative)
                r->new_sign = b_sign;

                // Function is recursivelly called
                // by inverting `a` and `b`, the process
                // of subtraction is easier
                this->_add_nodes(b, a, b_sign, a_sign, r);
                        
                // The b number is set as the a number
                r->new_head = b;
                while (r->new_head->prev != nullptr) {
                    r->new_head = r->new_head->prev;
                }

                return;
            }
        }
        
        int8_t sum = a->value + b->value * sign_coeff;
        numnode_t *left = a->prev;

        // If a new head is needed
        // a->prev may be NULL if a is the head of the list
        if (a->prev == nullptr && (sum >= 10 || (b->prev != nullptr))) {
            left = new numnode_t(0, nullptr, a);
            a->prev = left;
            r->new_head = left; // signals that the head has been changed
        }

        // If the result does not need carry
        // sum may be lower than 10 if the operation is complete
        if (sum < 10 && sum >= 0) {
            a->value = sum;
            this->_add_nodes(left, b->prev, a_sign, b_sign, r);

            // If `left` is still head and its value is 0,
            // the sign is set to POSITIVE and the number is truncated
            if (left == nullptr && a->value == 0) {
                // Delete secondary leading zeros
                while (a->next != nullptr && a->next->value == 0) {
                    numnode_t *next_next = a->next->next;
                    delete a->next;
                    a->next = next_next;
                    if (next_next != nullptr)
                        next_next->prev = a;
                }
                
                // Delete primary leading zeros
                if (a->next != nullptr) {
                    r->new_head = a->next;
                    a->next->prev = nullptr;
                    delete a;
                    return;
                }

                // If result is 0, the sign must be positive
                r->new_sign = numsign_t::POSITIVE;
                r->new_rear = a;
            }

            return;
        }

        // IMPLICIT ELSE
        // This code ONLY EXECUTES IF sum >= 10

        // If the result of the sum is negative
        // the result may be negative if `a_sign` and `b_sign`
        // are conflicting, and `a` and `b` are out of order
        // e.g. +7 - 8 = -1
        if (sum < 0) {
            // The value is incremente by 10
            // so to have sum = abs(sum)
            sum += 10;
        }

        // The sum (mod 10) is used as the result
        // the modulo of the sum is the single-digit
        // result for a
        a->value = sum % 10;

        // "fake" carry node with value = 1
        // only used as b for next recursion
        numnode_t carry = 1;

        this->_add_nodes(left, &carry, a_sign, b_sign, r);
        this->_add_nodes(left, b->prev, a_sign, b_sign, r);
    }

    void Number::_negate() {
        this->m_Sign = (numsign_t) !(bool)this->m_Sign;
    }

    bool Number::_greater(numnode_t *a, numnode_t *b) {
        while (a->prev != nullptr && b->prev != nullptr) {
            a = a->prev;
            b = b->prev;
        }

        if (b->prev != nullptr)
            return true;
        
        if (a->prev != nullptr)
            return false;

        while (a != nullptr && b != nullptr) {
            if (b->value > a->value)
                return true;

            if (a->value > b->value)
                return false;

            a = a->next;
            b = b->next;
        }

        return false;
    }
}