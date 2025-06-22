#pragma once
#include <string>
class Encryptor {
public:
    Encryptor(const std::string& key) : key_(key) {
        std::string result = " ";
        char s = 4;
        for (int i = 0; i < key.length(); i++) {
            // apply transformation to each character
            // Encrypt Uppercase letters
            if (isupper(key[i]))
                result += char(int(key[i] + s - 65) % 26 + 65);

            // Encrypt Lowercase letters
            else
                result += char(int(key[i] + s - 97) % 26 + 97);
        }
    }
private:
    std::string key_;
};