//
// Created by Zachary on 2021/8/18.
//

#include <fstream>
#include "gtest/gtest.h"
#include "nbtpp.h"

TEST(NBT, size) {
    std::ifstream in("/mnt/d/Workplace/cppProjects/nbtpp/test/bigtest.nbt");
    ASSERT_TRUE(in.is_open());
    nbtpp::NBT nbt(&in, nbtpp::Edition::JAVA);
    auto root = nbt.getRootCompound();
    EXPECT_EQ(root->size(), 11);
    EXPECT_EQ(root->internalCompound.size(), 1);
}
