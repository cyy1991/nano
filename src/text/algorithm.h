#pragma once

#include "arch.h"
#include <string>
#include <vector>

namespace zob
{
        ///
        /// \brief returns the lower case string
        ///
        ZOB_PUBLIC std::string lower(const std::string& str);

        ///
        /// \brief returns the upper case string
        ///
        ZOB_PUBLIC std::string upper(const std::string& str);

        ///
        /// \brief replace all occurencies of a character with another one
        ///
        ZOB_PUBLIC std::string replace(const std::string& str, const char token, const char newtoken);

        ///
        /// \brief check if a string contains a given character
        ///
        ZOB_PUBLIC bool contains(const std::string& str, const char token);

        ///
        /// \brief check if two strings are equal (case sensitive)
        ///
        ZOB_PUBLIC bool equals(const std::string& str1, const std::string& str2);

        ///
        /// \brief check if two strings are equal (case insensitive)
        ///
        ZOB_PUBLIC bool iequals(const std::string& str1, const std::string& str2);

        ///
        /// \brief check if a string starts with a token (case sensitive)
        ///
        ZOB_PUBLIC bool starts_with(const std::string& str, const std::string& token);

        ///
        /// \brief check if a string starts with a token (case insensitive)
        ///
        ZOB_PUBLIC bool istarts_with(const std::string& str, const std::string& token);

        ///
        /// \brief check if a string ends with a token (case sensitive)
        ///
        ZOB_PUBLIC bool ends_with(const std::string& str, const std::string& token);

        ///
        /// \brief check if a string ends with a token (case insensitive)
        ///
        ZOB_PUBLIC bool iends_with(const std::string& str, const std::string& token);

        ///
        /// \brief tokenize a string using the given delimeters
        ///
        ZOB_PUBLIC std::vector<std::string> split(const std::string& str, const char* delimeters);
}

