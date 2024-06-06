#include <Arduino.h>
#include "apmath.hpp"
#include <vector>


std::vector<char> input_buffer = std::vector<char>();
uint32_t input_len = 0;


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  
  Serial.println("ARITMETICA A PRECISIONE ARBITRARIA");
  Serial.println("DIMOSTRAZIONE 21/03/2024 SU ESP32");
  Serial.println("IMPLEMENTAZIONE DI ALESSANDRO SALERNO");
  Serial.println();
}

void loop() {
    while (Serial.available() > 0) {
        char c = Serial.read();

        if (c == '\r') {
            input_buffer.clear();
            break;
        }

        Serial.print(c);
        Serial.flush();

        if (c == 0x08 && input_len > 0) {
            input_buffer.pop_back();
            input_len--;
            Serial.print(' ');
            Serial.print((char) 0x08);
            Serial.flush();
            break;
        }

        if (c == '\n') {
            char *input_buf = new char[input_len + 1];
            strncpy(input_buf, input_buffer.data(), input_len);
            input_buf[input_len] = 0;
            
            char *input = input_buf;
            char *num_buf = new char[input_len];
            char *num_ptr = num_buf;

            for (*num_ptr = *input++; (*++num_ptr = *input) != 0 && isdigit(*input); input++);
            *num_ptr = 0;

            apmath::Number a(num_buf);

            num_ptr = num_buf;
            for (*num_ptr = *input++; (*++num_ptr = *input) != 0 && isdigit(*input); input++);
            *num_ptr = 0;
            
            apmath::Number b(num_buf);
            apmath::Number c = a + b;
            const char *sum = c.c_str();

            Serial.print("= ");
            Serial.println(sum);
            Serial.println();

            input_len = 0;
            input_buffer.clear();
            delete sum;
            delete input_buf;
            delete num_buf;
            break;
        }

        if (isalnum(c) || c == '+' || c == '-') {
            input_buffer.push_back(c);
            input_len++;
        }
    }
}
