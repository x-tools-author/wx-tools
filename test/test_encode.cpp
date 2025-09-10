#include <gtest/gtest.h>

#include "Common/wxTools.h"

#define TEST_STRING "(null)"

struct EncodeTestParam
{
    int format;
    wxString name;
    std::string input;
};

class EncodeTest : public ::testing::TestWithParam<EncodeTestParam>
{};

TEST_P(EncodeTest, TestEncode)
{
    auto param = GetParam();
    int len = 0;
    auto ret = DoEncodeBytes(param.input, len, param.format);

    std::string str(ret.get(), len);
    EXPECT_EQ(str, std::string(TEST_STRING)) << "Failed for format: " << param.name.ToStdWstring();
}

// clang-format off
#define DECODE_TESTS_ITEMS \
    EncodeTestParam{static_cast<int>(TextFormat::Bin), GetTextFormatName(TextFormat::Bin), "00101000 01101110 01110101 01101100 01101100 00101001"}, \
    EncodeTestParam{static_cast<int>(TextFormat::Oct), GetTextFormatName(TextFormat::Oct), "050 156 165 154 154 051"}, \
    EncodeTestParam{static_cast<int>(TextFormat::Dec), GetTextFormatName(TextFormat::Dec), "40 110 117 108 108 41"}, \
    EncodeTestParam{static_cast<int>(TextFormat::Hex), GetTextFormatName(TextFormat::Hex), "28 6E 75 6C 6C 29"}, \
    EncodeTestParam{static_cast<int>(TextFormat::HexWithoutSpace), GetTextFormatName(TextFormat::HexWithoutSpace), "286E756C6C29"}, \
    EncodeTestParam{static_cast<int>(TextFormat::Ascii), GetTextFormatName(TextFormat::Ascii), TEST_STRING}, \
    EncodeTestParam{static_cast<int>(TextFormat::Utf8), GetTextFormatName(TextFormat::Utf8), TEST_STRING}, \
    EncodeTestParam{static_cast<int>(TextFormat::GB2312), GetTextFormatName(TextFormat::GB2312), TEST_STRING}, \
    EncodeTestParam{static_cast<int>(TextFormat::CSGB2312), GetTextFormatName(TextFormat::CSGB2312), TEST_STRING}, \
    EncodeTestParam{static_cast<int>(TextFormat::GBK), GetTextFormatName(TextFormat::GBK), TEST_STRING}, \
    EncodeTestParam{static_cast<int>(TextFormat::GB18030), GetTextFormatName(TextFormat::GB18030), TEST_STRING}

INSTANTIATE_TEST_SUITE_P(EncodeTests, EncodeTest, ::testing::Values(DECODE_TESTS_ITEMS));