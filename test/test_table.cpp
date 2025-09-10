#include <bitset>

#include <gtest/gtest.h>

#include "Common/wxTools.h"

TEST(TABLE_TEST, TEST_TABLE_BINARY)
{
    for (int i = 0; i < 256; ++i) {
        std::string ret = GetBinString(static_cast<uint8_t>(i));
        int value = std::stoi(ret, nullptr, 2);
        EXPECT_EQ(i, value);
    }
}

TEST(TABLE_TEST, TEST_TABLE_OCTAL)
{
    for (int i = 0; i < 256; ++i) {
        std::string ret = GetOctString(static_cast<uint8_t>(i));
        int value = std::stoi(ret, nullptr, 8);
        EXPECT_EQ(i, value);
    }
}

TEST(TABLE_TEST, TEST_TABLE_DECIMAL)
{
    for (int i = 0; i < 256; ++i) {
        std::string ret = GetDecString(static_cast<uint8_t>(i));
        int value = std::stoi(ret, nullptr, 10);
        EXPECT_EQ(i, value);
    }
}

TEST(TABLE_TEST, TEST_TABLE_HEXADECIMAL)
{
    for (int i = 0; i < 256; ++i) {
        std::string ret = GetHexString(static_cast<uint8_t>(i));
        int value = std::stoi(ret, nullptr, 16);
        EXPECT_EQ(i, value);
    }
}