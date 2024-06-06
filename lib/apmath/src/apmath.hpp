#pragma once
#include <Arduino.h>


namespace apmath {
    typedef enum {
        NEGATIVE = 0,
        POSITIVE = 1
    } numsign_t;

    typedef struct numnode {
        uint8_t value;
        struct numnode *prev;
        struct numnode *next;

        public:
        numnode();
        numnode(uint8_t val);
        numnode(uint8_t val, struct numnode *prev, struct numnode *next);
    } numnode_t;

    typedef struct {
        numnode_t *new_head;
        numnode_t *new_rear;
        numsign_t new_sign;
    } result_t;

    class Number {
        private:
        numnode_t *m_Head;
        numnode_t *m_Rear;
        numsign_t m_Sign;

        int _length();
        void _add_nodes(numnode_t *a, numnode_t *b, numsign_t a_sign, numsign_t b_sign, result_t *r);
        bool _greater(numnode_t *a, numnode_t *b);

        public:
        Number(const char *input);
        Number(const Number &other);
        Number();
        ~Number();
        
        Number operator+(const Number other);
        Number operator-(const Number other);

        const char *c_str();
        void _negate();
    };
}