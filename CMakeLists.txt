cmake_minimum_required(VERSION 3.31.6)
project(jah VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_VERSION VERSION_LESS 3.25 AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LINUX true)
endif ()

if (APPLE)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif (BSD)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif (LINUX)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif (UNIX)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif (WIN32)
    if (MSVC)
        add_compile_options(/W4 /WX /permissive- /experimental:external /external:anglebrackets /external:W0)
    else ()
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    endif ()
else ()
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif ()

find_package(PostgreSQL REQUIRED)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
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
target_include_directories(ji_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(ji_domain
        Ticket.cpp Ticket.hpp
        Atom.cpp Atom.hpp
)
target_link_libraries(ji_domain PRIVATE ${PostgreSQL_LIBRARIES})

add_library(ji_io
        Storage.cpp Storage.hpp
        Hook.cpp Hook.hpp
)
target_include_directories(ji_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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

include(GNUInstallDirs)
include(GoogleTest)
gtest_discover_tests(jah_test)

install(TARGETS jah
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES Ji.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jah
)

install(TARGETS ji_core ji_domain ji_io
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)