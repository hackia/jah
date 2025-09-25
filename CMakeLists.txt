cmake_minimum_required(VERSION 3.15)
project(jah)

cmake_policy(SET CMP0135 NEW)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

option(ENABLE_CLANG_TIDY "Enable clang-tidy linter" ON)
if (ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if (CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    else ()
        message(STATUS "clang-tidy not found; proceeding without it")
    endif ()
endif ()

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_library(ji_core
        Ji.cpp Ji.hpp
        Branch.cpp Branch.hpp
        Features.cpp Features.hpp
        Forge.cpp Forge.hpp
        Hub.cpp Hub.hpp
        Ci.cpp Ci.hpp
        View.cpp View.hpp
        Queue.cpp Queue.hpp
        Workflow.cpp Workflow.hpp
        Singularity.cpp Singularity.hpp
        Constellation.cpp Constellation.hpp
        Discussions.cpp Discussions.hpp
)

add_library(ji_domain
        Ticket.cpp Ticket.hpp
        Atom.cpp Atom.hpp
)

add_library(ji_io
        Storage.cpp Storage.hpp
        Hook.cpp Hook.hpp
)

add_executable(jah main.cpp)

target_link_libraries(jah PRIVATE ji_core ji_domain ji_io)

enable_testing()

add_executable(jah_test
        tests/JiTest.cpp
        tests/BranchTest.cpp
        tests/FeaturesTest.cpp
        tests/ForgeTest.cpp
        tests/HubTest.cpp
        tests/CiTest.cpp
        tests/ViewTest.cpp
        tests/QueueTest.cpp
        tests/WorkflowTest.cpp
        tests/SingularityTest.cpp
        tests/ConstellationTest.cpp
        tests/DiscussionsTest.cpp
        tests/TicketTest.cpp
        tests/AtomTest.cpp
        tests/StorageTest.cpp
        tests/HookTest.cpp
)

target_link_libraries(jah_test
        PRIVATE
        ji_core
        ji_domain
        ji_io
        GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(jah_test)
install(TARGETS jah DESTINATION bin)
install(FILES Ji.hpp DESTINATION include)
install(TARGETS ji_core ji_domain ji_io DESTINATION lib)