file(GLOB_RECURSE SRC *.cpp)
add_executable(ZPPExamples ${SRC})

sanitize(address,undefined)

target_common_warnings(ZPPExamples PRIVATE)

target_link_libraries(ZPPExamples PRIVATE ZPP::ZPP)
