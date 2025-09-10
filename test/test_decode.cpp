#include "Common/wxTools.h"
#include <gtest/gtest.h>

struct DataItem
{
    std::shared_ptr<char> data;
    size_t size;
};

DataItem CreateData()
{
    const std::string data{"(null)"};
    DataItem item;
    item.data = std::shared_ptr<char>(new char[data.size()], std::default_delete<char[]>());
    std::copy(data.begin(), data.end(), item.data.get());
    item.size = data.size();
    return item;
}

// 参数化测试结构体
struct DecodeTestParam
{
    int format;
    wxString name;
    wxString expected;
};

class DecodeTest : public ::testing::TestWithParam<DecodeTestParam>
{};

TEST_P(DecodeTest, TestDecode)
{
    auto data = CreateData();
    auto param = GetParam();
    auto ret = DoDecodeBytes(data.data, (int &) data.size, param.format);
    EXPECT_EQ(ret, param.expected) << "Failed for format: " << param.name.ToStdWstring();
}

// clang-format off
#define DECODE_TESTS_ITEMS \
    DecodeTestParam{static_cast<int>(TextFormat::Bin), GetTextFormatName(TextFormat::Bin), "00101000 01101110 01110101 01101100 01101100 00101001"}, \
    DecodeTestParam{static_cast<int>(TextFormat::Oct), GetTextFormatName(TextFormat::Oct), "050 156 165 154 154 051"}, \
    DecodeTestParam{static_cast<int>(TextFormat::Dec), GetTextFormatName(TextFormat::Dec), "40 110 117 108 108 41"}, \
    DecodeTestParam{static_cast<int>(TextFormat::Hex), GetTextFormatName(TextFormat::Hex), "28 6E 75 6C 6C 29"}, \
    DecodeTestParam{static_cast<int>(TextFormat::HexWithoutSpace), GetTextFormatName(TextFormat::HexWithoutSpace), "286E756C6C29"}, \
    DecodeTestParam{static_cast<int>(TextFormat::Ascii), GetTextFormatName(TextFormat::Ascii), "(null)"}, \
    DecodeTestParam{static_cast<int>(TextFormat::Utf8), GetTextFormatName(TextFormat::Utf8), "(null)"}, \
    DecodeTestParam{static_cast<int>(TextFormat::GB2312), GetTextFormatName(TextFormat::GB2312), "(null)"}, \
    DecodeTestParam{static_cast<int>(TextFormat::CSGB2312), GetTextFormatName(TextFormat::CSGB2312), "(null)"}, \
    DecodeTestParam{static_cast<int>(TextFormat::GBK), GetTextFormatName(TextFormat::GBK), "(null)"}, \
    DecodeTestParam{static_cast<int>(TextFormat::GB18030), GetTextFormatName(TextFormat::GB18030), "(null)"}

INSTANTIATE_TEST_SUITE_P(DecodeTests, DecodeTest, ::testing::Values(DECODE_TESTS_ITEMS));