cmake_minimum_required(VERSION 3.31.6)
project(jah VERSION 0.1.0 LANGUAGES CXX C)

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
enable_language(C)

find_package(SQLite3 REQUIRED)

link_directories(${SQLITE_LINK_DIR})
include_directories(include ${SQLITE_INCLUDE})

find_package(PostgreSQL REQUIRED)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)
FetchContent_Declare(
        blake3
        URL https://github.com/BLAKE3-team/BLAKE3/archive/refs/tags/1.5.1.zip
)
FetchContent_MakeAvailable(blake3)
add_library(blake3_lib STATIC
        ${blake3_SOURCE_DIR}/c/blake3.c
        ${blake3_SOURCE_DIR}/c/blake3_dispatch.c
        ${blake3_SOURCE_DIR}/c/blake3_portable.c
)
target_include_directories(blake3_lib PUBLIC ${blake3_SOURCE_DIR}/c)
add_library(ji_core
        Ji.cpp Ji.hpp Config.cpp Config.hpp Model.cpp Model.hpp Controller.cpp Controller.hpp ConfigManager.cpp ConfigManager.hpp
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
target_link_libraries(ji_core PRIVATE SQLite::SQLite3 blake3)

add_library(ji_domain
        Ticket.cpp Ticket.hpp
        Atom.cpp Atom.hpp
)
target_link_libraries(ji_domain PRIVATE ji_core SQLite::SQLite3 blake3_lib ${PostgreSQL_LIBRARIES})

add_library(ji_io
        Storage.cpp Storage.hpp
        Hook.cpp Hook.hpp
)
target_include_directories(ji_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(jah main.cpp)
target_link_libraries(jah PRIVATE ji_core ji_domain ji_io)

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