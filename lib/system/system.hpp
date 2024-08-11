//
// Created by Charles on 11/08/2024.
//

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <string>
#include <cstdint>
#include <stdexcept>

// Replace this in "platformio.ini" ?
#define OS_NAME "PaxOS"

#define OS_VERSION_MAJOR "9"
#define OS_VERSION_MINOR "0"
#define OS_VERSION_PATCH "0"
#define OS_VERSION_BUILD "0"
#define OS_VERSION_CODENAME "Red Panic"

#define OS_VERSION OS_VERSION_MAJOR "." OS_VERSION_MINOR "." OS_VERSION_PATCH "-" OS_VERSION_BUILD " (" OS_VERSION_CODENAME ")"

namespace libsystem {
    void delay(uint64_t ms);
    void panic(const std::string& message);

    namespace exceptions {
        class RuntimeError final : public std::runtime_error {
        public:
            explicit RuntimeError(const std::string& message);
            explicit RuntimeError(const char* message);
        };

        class OutOfRange final : public std::out_of_range {
        public:
            explicit OutOfRange(const std::string& message);
            explicit OutOfRange(const char* message);
        };
    }
}

#endif //SYSTEM_HPP
