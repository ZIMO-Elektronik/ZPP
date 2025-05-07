include(GoogleTest)

file(GLOB_RECURSE SRC *.cpp)
add_executable(ZPPTests ${SRC})

sanitize(address,undefined)

target_common_warnings(ZPPTests PRIVATE)

cpmaddpackage(URI "gh:google/googletest#main" OPTIONS "INSTALL_GTEST OFF")

target_link_libraries(ZPPTests PRIVATE ZPP::ZPP GTest::gtest_main GTest::gmock)

gtest_discover_tests(ZPPTests)
