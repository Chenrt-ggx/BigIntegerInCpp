#include "constant.h"

std::string piCalculate(int length) {
    int total = length / width + width + 1;
    std::vector<int> result(total);
    std::vector<int> adder(total);
    std::vector<int> buffer(total);
    result[1] = adder[1] = mask;
    int index = 1;
    for (bool flag = true; flag; ++index) {
        int p = 0;
        for (int i = 1; i < total; ++i) {
            int update = p * mask + adder[i];
            p = update % (index << 1 | 1), buffer[i] = update / (index << 1 | 1);
        }
        for (int i = 1; i < total; ++i) {
            buffer[i] *= index;
            if (buffer[i] >= mask) {
                buffer[i - 1] += buffer[i] / mask;
                buffer[i] %= mask;
            }
        }
        flag = false;
        for (int i = 1; i < total; ++i) {
            adder[i] = buffer[i];
            result[i] += adder[i] << 1;
            flag |= adder[i];
            if (result[i] >= mask) {
                result[i - 1] += result[i] / mask;
                result[i] %= mask;
            }
        }
    }
    std::string ans = "3.";
    for (int i = 1; i < total; ++i) {
        std::string str = std::to_string(result[i]);
        ans += std::string(width - str.length(), '0') + str;
    }
    return ans.substr(0, length + 2);
}

std::string eCalculate(int length) {
    int total = length / width + width + 1;
    std::vector<int> result(total);
    std::vector<int> adder(total);
    std::vector<int> buffer(total);
    result[1] = adder[1] = mask;
    int index = 2;
    for (bool flag = true; flag; ++index) {
        int p = 0;
        for (int i = 1; i < total; ++i) {
            int update = p * mask + adder[i];
            p = update % index, buffer[i] = update / index;
        }
        flag = false;
        for (int i = 1; i < total; ++i) {
            result[i] += adder[i] = buffer[i];
            flag |= adder[i];
            if (result[i] >= mask) {
                result[i - 1] += result[i] / mask;
                result[i] %= mask;
            }
        }
    }
    std::string ans = "2.";
    for (int i = 1; i < total; ++i) {
        std::string str = std::to_string(result[i]);
        ans += std::string(width - str.length(), '0') + str;
    }
    return ans.substr(0, length + 2);
}
