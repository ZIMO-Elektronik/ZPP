#include <algorithm>
#include <iostream>
#include <zpp/zpp.hpp>

int main() {
  auto zpp_file{zpp::read("./data/Da_Di_Collection_ZIMO-mfx-16Bit_S02.zpp")};

  // Print ID
  std::cout << "ZPP ID " << zpp_file.id << "\n";

  // Print version
  std::cout << "ZPP version " << zpp_file.version << "\n";

  // Print coded
  std::cout << "ZPP coded? " << (zpp_file.coded ? "yes" : "no") << "\n";
}