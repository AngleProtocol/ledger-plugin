cmake_minimum_required(VERSION 3.4)

project(Fuzzer LANGUAGES C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-g -ggdb2 -O3)

set(ANGLE_DIR "../src")
set(ETH_DIR "ethereum-plugin-sdk")
set(SDK_DIR "nanos-secure-sdk")

add_compile_definitions(
        APPNETWORK=1
        HAVE_SHA3
        HAVE_HASH
        HAVE_ECC
        HAVE_MATH
        IO_HID_EP_LENGTH=64
)

# Build with code coverage generation
if(CODE_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
        add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
        add_link_options(-fprofile-instr-generate -fcoverage-mapping)
    elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
        add_compile_options(-fprofile-arcs -ftest-coverage)
        link_libraries(gcov)
    else()
        message(FATAL_ERROR "Unsupported compiler used with code coverage generation")
    endif()
endif()

add_library(angle
        ${ANGLE_DIR}/handle_finalize.c
        ${ANGLE_DIR}/handle_init_contract.c
        ${ANGLE_DIR}/handle_provide_parameter.c
        ${ANGLE_DIR}/handle_provide_token.c
        ${ANGLE_DIR}/handle_query_contract_id.c
        ${ANGLE_DIR}/handle_query_contract_ui.c
        ${ANGLE_DIR}/helpers.c
        ${ETH_DIR}/include/eth_internals.c
)

add_executable(fuzz_calls fuzz_calls.c mocks.c)

target_link_libraries(fuzz_calls PUBLIC angle)
target_compile_options(fuzz_calls PUBLIC -fsanitize=fuzzer,address,undefined -fno-sanitize-recover=undefined)
target_link_options(fuzz_calls PUBLIC -fsanitize=fuzzer,address,undefined -fno-sanitize-recover=undefined)

include_directories(${ETH_DIR}/include)
include_directories("../src")
include_directories("../src/dbg")
include_directories(".")
include_directories(${SDK_DIR}/lib_cxng/include)
include_directories(${SDK_DIR}/lib_cxng/src)
include_directories(${SDK_DIR}/include)
