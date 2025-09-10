#include <memory>
#include <tuple>
#include <vector>

#include <gtest/gtest.h>

#include "Common/wxTools.h"

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

struct CrcTestParam
{
    CRCType type;
    std::string name;
    std::vector<char> refBe;
    std::vector<char> refLe;
};

class CRCParamTest : public ::testing::TestWithParam<CrcTestParam>
{};

TEST_P(CRCParamTest, Calculate)
{
    auto param = GetParam();
    auto data = CreateData();
    int algorithm = static_cast<int>(param.type);

    SCOPED_TRACE(param.name);

    auto retBe = DoCalculateCRC(data.data, data.size, algorithm, 0, 0, true);
    EXPECT_EQ(retBe, param.refBe) << "Big-endian failed for " << param.name;

    auto retLe = DoCalculateCRC(data.data, data.size, algorithm, 0, 0, false);
    EXPECT_EQ(retLe, param.refLe) << "Little-endian failed for " << param.name;
}

// clang-format off
#define CRC_TESTS_ITEMS \
    CrcTestParam{CRCType::CRC_8, "CRC_8", {char(0x1F)}, {char(0x1F)}}, \
    CrcTestParam{CRCType::CRC_8_ITU, "CRC_8_ITU", {char(0x4A)}, {char(0x4A)}}, \
    CrcTestParam{CRCType::CRC_8_ROHC, "CRC_8_ROHC", {char(0xA1)}, {char(0xA1)}}, \
    CrcTestParam{CRCType::CRC_8_MAXIM, "CRC_8_MAXIM", {char(0x3B)}, {char(0x3B)}}, \
    CrcTestParam{CRCType::CRC_16_IBM, "CRC_16_IBM", {char(0x2E), char(0xD9)}, {char(0xD9), char(0x2E)}}, \
    CrcTestParam{CRCType::CRC_16_MAXIM, "CRC_16_MAXIM", {char(0xD1), char(0x26)}, {char(0x26), char(0xD1)}}, \
    CrcTestParam{CRCType::CRC_16_USB, "CRC_16_USB", {char(0xCA), char(0x26)}, {char(0x26), char(0xCA)}}, \
    CrcTestParam{CRCType::CRC_16_MODBUS, "CRC_16_MODBUS", {char(0x35), char(0xD9)}, {char(0xD9), char(0x35)}}, \
    CrcTestParam{CRCType::CRC_16_CCITT, "CRC_16_CCITT", {char(0x80), char(0xF9)}, {char(0xF9), char(0x80)}}, \
    CrcTestParam{CRCType::CRC_16_CCITT_FALSE, "CRC_16_CCITT_FALSE", {char(0x0B), char(0x18)}, {char(0x18), char(0x0B)}}, \
    CrcTestParam{CRCType::CRC_16_x25, "CRC_16_x25", {char(0x77), char(0x76)}, {char(0x76), char(0x77)}}, \
    CrcTestParam{CRCType::CRC_16_XMODEM, "CRC_16_XMODEM", {char(0x05), char(0x08)}, {char(0x08), char(0x05)}}, \
    CrcTestParam{CRCType::CRC_16_DNP, "CRC_16_DNP", {char(0xB8), char(0x46)}, {char(0x46), char(0xB8)}}, \
    CrcTestParam{CRCType::CRC_32, "CRC_32", {char(0xEA), char(0xE9), char(0x31), char(0xF9)}, {char(0xF9), char(0x31), char(0xE9), char(0xEA)}}, \
    CrcTestParam{CRCType::CRC_32_MPEG2, "CRC_32_MPEG2", {char(0x41), char(0x5C), char(0x23), char(0xCA)}, {char(0xCA), char(0x23), char(0x5C), char(0x41)}}, \
    CrcTestParam{CRCType::Sum8, "Sum8", {char(0x0C)}, {char(0x0C)}}, \
    CrcTestParam{CRCType::Sum16, "Sum16", {char(0x02), char(0x0C)}, {char(0x0C), char(0x02)}}, \
    CrcTestParam{CRCType::Sum32, "Sum32", {char(0x00), char(0x00), char(0x02), char(0x0C)}, {char(0x0C), char(0x02), char(0x00), char(0x00)}}, \
    CrcTestParam{CRCType::Sum64, "Sum64", {char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x02), char(0x0C)}, {char(0x0C), char(0x02), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00)}}
// clang-format on

INSTANTIATE_TEST_SUITE_P(CRC_TESTS, CRCParamTest, ::testing::Values(CRC_TESTS_ITEMS));