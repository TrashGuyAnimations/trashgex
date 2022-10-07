//  trashgex © 2022 by Hackintosh Five is licensed under CC BY-SA 4.0. To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/

#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#define REGEX (PCRE2_SPTR)R"((^.*?)(?:(?:^(.)<)|(?:^(.).<)|(?: (.)<)|( ))?(?:\(.+\))(?:(> +$)|(> )|(?:>(.))|( ))?)"
#define REPLACEMENT (PCRE2_SPTR)R"(${2:+$2(> ^_^)>$9}${3:+$3<(^_^ <) $9}${4:+$1$4<(^_^ <) $9}${7:+$1$5 (> ^_^)>}${8:+$1$8<(^_^ <)$5})"

#define ERROR_LENGTH 256

#include <iostream>
#include <string>

unsigned char error[ERROR_LENGTH] = {};

int next(const pcre2_code *code, PCRE2_SPTR input, unsigned char *output, size_t input_size);

int main() {
    std::string input_str;
    std::getline(std::cin, input_str);
    size_t input_size = input_str.length() + 1;
    auto *input = new unsigned char[input_size];
    auto *output = new unsigned char[input_size];
    input_str.copy((char *)input, input_size - 1);
    (input)[input_size - 1] = 0;
    int errorcode;
    PCRE2_SIZE erroroffset;
    const pcre2_code *code = pcre2_compile(REGEX, PCRE2_ZERO_TERMINATED, PCRE2_UTF, &errorcode, &erroroffset, nullptr);
    if (code == nullptr) {
        std::cout << "Error " << errorcode << " at offset " << erroroffset << "\n";
        pcre2_get_error_message(errorcode, error, ERROR_LENGTH);
        std::cout << error;
        return errorcode;
    }
    int rc;
    do {
        std::cout << input << "\n";
        rc = next(code, input, output, input_size);
        std::swap(input, output);
    } while (rc == 1 && 0 != input[0]);
    return 0;
}

int next(const pcre2_code *code, PCRE2_SPTR input, unsigned char *output, size_t input_size) {
    int count = pcre2_substitute(code, input, PCRE2_ZERO_TERMINATED, 0, PCRE2_SUBSTITUTE_EXTENDED | PCRE2_SUBSTITUTE_UNSET_EMPTY, nullptr, nullptr, REPLACEMENT, PCRE2_ZERO_TERMINATED, output, &input_size);
    if (count < 0) {
        pcre2_get_error_message(count, error, ERROR_LENGTH);
        std::cout << error;
    }
    return count;
}
