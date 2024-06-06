#include <Arduino.h>
#include <unity.h>
#include "apmath.hpp"


void test_allocation() {
    char *random = new char[5];
    random[4] = 0;

    for (int i = 0; i < 4; i++) {
        random[4] = (rand() % 10) + '0';
    }

    apmath::Number n =random;
    const char *out = n.c_str();

    TEST_ASSERT_EQUAL_STRING(random, out);

    delete random;
    delete out;
}

void test_add_pp() {
    apmath::Number a = "222";
    apmath::Number b = "4";
    apmath::Number sum = a + b;
    const char *repr = sum.c_str();

    TEST_ASSERT_EQUAL_STRING("226", repr);

    delete repr;
}

void test_add_np() {
    apmath::Number a = "-12345678";
    apmath::Number b = "87654321";
    apmath::Number sum = a + b;
    const char *repr = sum.c_str();

    TEST_ASSERT_EQUAL_STRING("75308643", repr);

    delete repr;
}

void test_add_nn() {
    apmath::Number a = "-111111";
    apmath::Number b = "-111111";
    apmath::Number sum = a + b;
    const char *repr = sum.c_str();

    TEST_ASSERT_EQUAL_STRING("-222222", repr);

    delete repr;
}

void test_add_pn() {
    apmath::Number a = "12345678";
    apmath::Number b = "-87654321";
    apmath::Number sum = b + a;
    const char *repr = sum.c_str();

    TEST_ASSERT_EQUAL_STRING("-75308643", repr);

    delete repr;
}

void setup() {
    delay(3000);

    UNITY_BEGIN();

    RUN_TEST(test_allocation);
    RUN_TEST(test_add_pp);
    RUN_TEST(test_add_np);
    RUN_TEST(test_add_pn);
    RUN_TEST(test_add_nn);

    UNITY_END();
}

void loop() {

}