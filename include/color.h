#pragma once
#include <string>

namespace Color {
    // Foreground
    inline const std::string RESET   = "\033[0m";
    inline const std::string BOLD    = "\033[1m";
    inline const std::string DIM     = "\033[2m";
    inline const std::string RED     = "\033[31m";
    inline const std::string GREEN   = "\033[32m";
    inline const std::string YELLOW  = "\033[33m";
    inline const std::string BLUE    = "\033[34m";
    inline const std::string MAGENTA = "\033[35m";
    inline const std::string CYAN    = "\033[36m";
    inline const std::string WHITE   = "\033[37m";

    inline std::string bold(const std::string& s)    { return BOLD    + s + RESET; }
    inline std::string red(const std::string& s)     { return RED     + s + RESET; }
    inline std::string green(const std::string& s)   { return GREEN   + s + RESET; }
    inline std::string yellow(const std::string& s)  { return YELLOW  + s + RESET; }
    inline std::string cyan(const std::string& s)    { return CYAN    + s + RESET; }
    inline std::string magenta(const std::string& s) { return MAGENTA + s + RESET; }
    inline std::string dim(const std::string& s)     { return DIM     + s + RESET; }
}
