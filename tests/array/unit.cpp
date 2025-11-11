#include "array.hpp"

#include <gtest/gtest.h>

namespace tests::array {

class ArrayTest : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {}

  lib::array::Array empty_array_;
  lib::array::Array array_with_values_{'a', 'b', 'c'};
};

TEST_F(ArrayTest, DefaultConstructor) {
  EXPECT_TRUE(empty_array_.Empty());
  EXPECT_EQ(empty_array_.Size(), 0);
  EXPECT_GE(empty_array_.Capacity(), 0);
}

TEST_F(ArrayTest, SizeValueConstructor) {
  lib::array::Array arr(5, 'A');
  EXPECT_EQ(arr.Size(), 5);
  EXPECT_FALSE(arr.Empty());
  EXPECT_EQ(arr.Get(0), 'A');
  EXPECT_EQ(arr.Get(4), 'A');
}

TEST_F(ArrayTest, InitializerListConstructor) {
  EXPECT_EQ(array_with_values_.Size(), 3);
  EXPECT_EQ(array_with_values_.Front(), 'a');
  EXPECT_EQ(array_with_values_.Back(), 'c');
  EXPECT_EQ(array_with_values_.Get(1), 'b');
}

TEST_F(ArrayTest, StringConstructor) {
  lib::array::Array arr("hello");
  EXPECT_EQ(arr.Size(), 5);
  EXPECT_EQ(arr.Get(0), 'h');
  EXPECT_EQ(arr.Get(4), 'o');
}

TEST_F(ArrayTest, CopyConstructor) {
  lib::array::Array copy(array_with_values_);

  EXPECT_EQ(array_with_values_.Size(), copy.Size());
  for (std::size_t i = 0; i < array_with_values_.Size(); ++i) {
    EXPECT_EQ(array_with_values_.Get(i), copy.Get(i));
  }
}

TEST_F(ArrayTest, MoveConstructor) {
  lib::array::Array original{'1', '2', '3'};
  lib::array::Array moved(std::move(original));

  EXPECT_EQ(moved.Size(), 3);
  EXPECT_TRUE(original.Empty() || original.Size() == 0);
}

TEST_F(ArrayTest, PushBackIncreasesSize) {
  lib::array::Array arr;
  arr.PushBack('a');
  arr.PushBack('b');

  EXPECT_EQ(arr.Size(), 2);
  EXPECT_EQ(arr.Front(), 'a');
  EXPECT_EQ(arr.Back(), 'b');
}

TEST_F(ArrayTest, PushBackResizesWhenNeeded) {
  lib::array::Array arr;
  std::size_t initial_capacity = arr.Capacity();

  // Заполняем до предела
  for (std::size_t i = 0; i < initial_capacity; ++i) {
    arr.PushBack('x');
  }

  EXPECT_EQ(arr.Size(), initial_capacity);
  EXPECT_EQ(arr.Capacity(), initial_capacity);

  // Следующий PushBack должен вызвать ресайз
  arr.PushBack('y');
  EXPECT_GT(arr.Capacity(), initial_capacity);
  EXPECT_EQ(arr.Size(), initial_capacity + 1);
}

TEST_F(ArrayTest, PopBackDecreasesSize) {
  lib::array::Array arr{'a', 'b', 'c'};
  arr.PopBack();

  EXPECT_EQ(arr.Size(), 2);
  EXPECT_EQ(arr.Back(), 'b');
}

TEST_F(ArrayTest, PopBackOnEmptyArray) {
  lib::array::Array arr;
  arr.PopBack(); // Не должно падать
  EXPECT_TRUE(arr.Empty());
}

TEST_F(ArrayTest, ClearResetsSize) {
  lib::array::Array arr{'a', 'b', 'c'};
  arr.Clear();

  EXPECT_TRUE(arr.Empty());
  EXPECT_EQ(arr.Size(), 0);
  EXPECT_EQ(arr.Capacity(),
            lib::array::DEFAULT_CAPACITY); // Capacity сохраняется
}

TEST_F(ArrayTest, ReserveIncreasesCapacity) {
  lib::array::Array arr;
  std::size_t new_capacity = 20;
  arr.Reserve(new_capacity);

  EXPECT_GE(arr.Capacity(), new_capacity);
  EXPECT_TRUE(arr.Empty());
}

TEST_F(ArrayTest, DataAccessMethods) {
  unsigned char *data = array_with_values_.Data();

  EXPECT_EQ(array_with_values_.Front(), 'a');
  EXPECT_EQ(array_with_values_.Back(), 'c');
  EXPECT_EQ(array_with_values_.Get(1), 'b');
  EXPECT_EQ(data[0], 'a');
  EXPECT_EQ(data[1], 'b');
  EXPECT_EQ(data[2], 'c');
}

TEST_F(ArrayTest, EmptyMethod) {
  EXPECT_TRUE(empty_array_.Empty());
  EXPECT_FALSE(array_with_values_.Empty());
}

} // namespace tests::array
