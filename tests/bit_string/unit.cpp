#include "bit_string.hpp"

#include <gtest/gtest.h>

namespace tests::bit_string {

class BitStringTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}

  lib::bit_string::BitString empty_bitstring_;
  lib::bit_string::BitString bitstring_0101_{"0101"};
  lib::bit_string::BitString bitstring_0011_{"0011"};
  lib::bit_string::BitString bitstring_1111_{"1111"};
  lib::bit_string::BitString bitstring_0000_{"0000"};
};

TEST_F(BitStringTest, DefaultConstructor) {
  EXPECT_TRUE(empty_bitstring_.Equals(lib::bit_string::BitString("")));
}

TEST_F(BitStringTest, SizeValueConstructor) {
  lib::bit_string::BitString bs(3, '1');
  EXPECT_TRUE(bs.Equals(lib::bit_string::BitString("111")));
}

TEST_F(BitStringTest, InitializerListConstructor) {
  lib::bit_string::BitString bs{'1', '0', '1'};
  EXPECT_TRUE(bs.Equals(lib::bit_string::BitString("101")));
}

TEST_F(BitStringTest, StringConstructor) {
  lib::bit_string::BitString bs("1101");
  EXPECT_TRUE(bs.Equals(lib::bit_string::BitString({'1', '1', '0', '1'})));
}

TEST_F(BitStringTest, InvalidValueThrowsException) {
  EXPECT_THROW(lib::bit_string::BitString(3, '2'), std::invalid_argument);
}

TEST_F(BitStringTest, InvalidStringThrowsException) {
  EXPECT_THROW(lib::bit_string::BitString("102"), std::invalid_argument);
  EXPECT_THROW(lib::bit_string::BitString("abc"), std::invalid_argument);
  EXPECT_THROW(lib::bit_string::BitString("1a0"), std::invalid_argument);
}

TEST_F(BitStringTest, ConstructFromArray) {
  lib::array::Array arr{'1', '0', '1', '0'};
  lib::bit_string::BitString bs(arr);
  EXPECT_TRUE(bs.Equals(lib::bit_string::BitString("1010")));
}

TEST_F(BitStringTest, ConstructFromArrayWithInvalidData) {
  lib::array::Array arr{'1', '0', '2'}; // '2' - invalid
  EXPECT_THROW(lib::bit_string::BitString bs(arr), std::invalid_argument);
}

TEST_F(BitStringTest, ConstructFromArrayWithLeadingZeroes) {
  lib::array::Array arr{'0', '0', '1', '0'};
  lib::bit_string::BitString bs(arr);
  EXPECT_TRUE(
      bs.Equals(lib::bit_string::BitString("001"))); // leading zeroes removed
}

TEST_F(BitStringTest, AdditionOperation) {
  lib::bit_string::BitString a("0101"); // 10
  lib::bit_string::BitString b("1100"); // 3
  auto result = lib::bit_string::BitString::Add(a, b);

  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("1011"))); // 13
}

TEST_F(BitStringTest, AdditionWithCarry) {
  lib::bit_string::BitString a("1111"); // 15
  lib::bit_string::BitString b("1000"); // 1
  auto result = lib::bit_string::BitString::Add(a, b);

  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("00001"))); // 16
}

TEST_F(BitStringTest, AdditionDifferentLengths) {
  lib::bit_string::BitString a("1");   // 1
  lib::bit_string::BitString b("111"); // 7
  auto result = lib::bit_string::BitString::Add(a, b);

  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("0001"))); // 8
}

TEST_F(BitStringTest, SubtractionOperation) {
  lib::bit_string::BitString a("1011"); // 13
  lib::bit_string::BitString b("1100"); // 3
  auto result = lib::bit_string::BitString::Substract(a, b);

  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("0101"))); // 10
}

TEST_F(BitStringTest, SubtractionWithBorrow) {
  lib::bit_string::BitString a("0001"); // 8
  lib::bit_string::BitString b("1000"); // 1
  auto result = lib::bit_string::BitString::Substract(a, b);

  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("1110"))); // 7
}

TEST_F(BitStringTest, SubtractionEqualOperands) {
  lib::bit_string::BitString a("0101");
  lib::bit_string::BitString b("0101");
  auto result = lib::bit_string::BitString::Substract(a, b);

  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("0")));
}

TEST_F(BitStringTest, SubtractionThrowsWhenASmallerThanB) {
  lib::bit_string::BitString a("0100"); // 2
  lib::bit_string::BitString b("0101"); // 10

  EXPECT_THROW(lib::bit_string::BitString::Substract(a, b),
               std::invalid_argument);
}

TEST_F(BitStringTest, ComplexAdditionAndSubtraction) {
  lib::bit_string::BitString a("0101"); // 10
  lib::bit_string::BitString b("1100"); // 3
  lib::bit_string::BitString c("1010"); // 5

  auto sum = lib::bit_string::BitString::Add(a, b);            // 10 + 3 = 13
  auto result = lib::bit_string::BitString::Substract(sum, c); // 13 - 5 = 8

  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("0001")));
}

TEST_F(BitStringTest, ComparisonOperations) {
  // Greater tests
  EXPECT_TRUE(bitstring_0011_.Greater(bitstring_0101_));
  EXPECT_FALSE(bitstring_0101_.Greater(bitstring_0011_));

  // Smaller tests
  EXPECT_TRUE(bitstring_0101_.Smaller(bitstring_0011_));
  EXPECT_FALSE(bitstring_0011_.Smaller(bitstring_0101_));

  // Equals tests
  EXPECT_TRUE(bitstring_0101_.Equals(lib::bit_string::BitString("0101")));
  EXPECT_FALSE(bitstring_0101_.Equals(bitstring_0011_));
}

TEST_F(BitStringTest, AndOperation) {
  auto result =
      lib::bit_string::BitString::And(bitstring_0101_, bitstring_0011_);
  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("0001")));

  // AND с самой собой
  auto self_and =
      lib::bit_string::BitString::And(bitstring_0101_, bitstring_0101_);
  EXPECT_TRUE(self_and.Equals(bitstring_0101_));

  // AND с нулями
  auto with_zeros =
      lib::bit_string::BitString::And(bitstring_0101_, bitstring_0000_);
  EXPECT_TRUE(with_zeros.Equals(lib::bit_string::BitString("0")));
}

TEST_F(BitStringTest, OrOperation) {
  auto result =
      lib::bit_string::BitString::Or(bitstring_0101_, bitstring_0011_);
  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("0111")));

  // OR с самой собой
  auto self_or =
      lib::bit_string::BitString::Or(bitstring_0101_, bitstring_0101_);
  EXPECT_TRUE(self_or.Equals(bitstring_0101_));

  // OR с нулями
  auto with_zeros =
      lib::bit_string::BitString::Or(bitstring_0101_, bitstring_0000_);
  EXPECT_TRUE(with_zeros.Equals(bitstring_0101_));
}

TEST_F(BitStringTest, XorOperation) {
  auto result =
      lib::bit_string::BitString::Xor(bitstring_0101_, bitstring_0011_);
  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("011")));

  // XOR с самой собой дает 0
  auto self_xor =
      lib::bit_string::BitString::Xor(bitstring_0101_, bitstring_0101_);
  EXPECT_TRUE(self_xor.Equals(lib::bit_string::BitString("0")));

  // XOR с нулями дает саму себя
  auto with_zeros =
      lib::bit_string::BitString::Xor(bitstring_0101_, bitstring_0000_);
  EXPECT_TRUE(with_zeros.Equals(bitstring_0101_));
}

TEST_F(BitStringTest, NotOperation) {
  auto result = lib::bit_string::BitString::Not(bitstring_0101_);
  EXPECT_TRUE(result.Equals(lib::bit_string::BitString("101")));

  auto all_ones_inverted = lib::bit_string::BitString::Not(bitstring_1111_);
  EXPECT_TRUE(all_ones_inverted.Equals(lib::bit_string::BitString("0")));

  auto zeros_inverted = lib::bit_string::BitString::Not(bitstring_0000_);
  EXPECT_TRUE(zeros_inverted.Equals(
      lib::bit_string::BitString("1"))); // После удаления ведущих нулей
}

TEST_F(BitStringTest, RemoveLeadingZeroes) {
  lib::bit_string::BitString bs("1000");
  EXPECT_TRUE(bs.Equals(lib::bit_string::BitString("1")));

  lib::bit_string::BitString bs2("0000");
  EXPECT_TRUE(bs2.Equals(lib::bit_string::BitString("0")));

  lib::bit_string::BitString bs3("0100");
  EXPECT_TRUE(bs3.Equals(lib::bit_string::BitString("01")));
}

TEST_F(BitStringTest, DifferentLengthOperations) {
  lib::bit_string::BitString short_bs("1");
  lib::bit_string::BitString long_bs("0101");

  auto and_result = lib::bit_string::BitString::And(long_bs, short_bs);
  auto or_result = lib::bit_string::BitString::Or(long_bs, short_bs);
  auto xor_result = lib::bit_string::BitString::Xor(long_bs, short_bs);

  EXPECT_TRUE(and_result.Equals(lib::bit_string::BitString("0")));
  EXPECT_TRUE(or_result.Equals(lib::bit_string::BitString("1101")));
  EXPECT_TRUE(xor_result.Equals(
      lib::bit_string::BitString("1101"))); // 1010 XOR 0001 = 1011
}

TEST_F(BitStringTest, EdgeCases) {
  // Один бит
  lib::bit_string::BitString single_bit("1");
  EXPECT_TRUE(single_bit.Equals(lib::bit_string::BitString("1")));

  // Все нули
  lib::bit_string::BitString all_zeros("0000");
  EXPECT_TRUE(all_zeros.Equals(lib::bit_string::BitString("0")));

  // Все единицы
  lib::bit_string::BitString all_ones("1111");
  EXPECT_TRUE(all_ones.Equals(lib::bit_string::BitString("1111")));
}

TEST_F(BitStringTest, ComplexOperations) {
  // Комбинированные операции: (A AND B) OR (A XOR B)
  lib::bit_string::BitString a("0101");
  lib::bit_string::BitString b("0011");

  auto and_result = lib::bit_string::BitString::And(a, b);
  auto xor_result = lib::bit_string::BitString::Xor(a, b);
  auto combined = lib::bit_string::BitString::Or(and_result, xor_result);

  EXPECT_TRUE(combined.Equals(lib::bit_string::BitString("0111")));
}

} // namespace tests::bit_string
