#include <gtest/gtest.h>
#include <numeric>
#include <zpp/zpp.hpp>
#include "utility.hpp"

TEST(read, read_throws_on_invalid_path) {
  EXPECT_THROW(zpp::read("invalid path"), std::filesystem::filesystem_error);
}

TEST(read, read_free_zpp) {
  auto zpp_file{zpp::read(source_location_parent_path() /
                          "../data/Da_Di_Collection_ZIMO-mfx-16Bit_S02.zpp")};

  EXPECT_EQ(zpp_file.cvs[0uz], (std::pair<uint16_t, uint8_t>{0u, 3u}));
  EXPECT_FALSE(zpp_file.coded);
}

TEST(read, read_free_coded) {
  auto zpp_file{
    zpp::read(source_location_parent_path() /
              "../data/Taurus_LeoSoundLab_Roco_8-Pol_MX_crypt.zpp")};

  EXPECT_EQ(zpp_file.cvs[0uz], (std::pair<uint16_t, uint8_t>{0u, 3u}));
  EXPECT_TRUE(zpp_file.coded);
}