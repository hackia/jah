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
find_package(CURL REQUIRED)
find_package(Boost 1.71.0 REQUIRED COMPONENTS system thread)
find_package(SQLite3 REQUIRED)
link_directories(${SQLITE_LINK_DIR})
include_directories(include ${SQLITE_INCLUDE})
find_package(PostgreSQL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
include_directories(${GTK4_INCLUDE_DIRS})


add_library(jah_atoms libs/jah/src/atoms/Forge.cpp libs/jah/src/atoms/Atom.cpp libs/jah/src/atoms/Singularity.cpp libs/jah/src/atoms/Queue.cpp libs/jah/include/atoms/Forge.hpp libs/jah/include/atoms/Atom.hpp libs/jah/include/atoms/Singularity.hpp libs/jah/include/atoms/Queue.hpp)
add_library(jah_branch libs/jah/include/branches/Branches.hpp libs/jah/include/branches/Flow.hpp libs/jah/src/branches/Branches.cpp libs/jah/src/branches/Flow.cpp)
add_library(jah_config libs/jah/include/config/Config.hpp libs/jah/src/config/Config.cpp)
add_library(jah_core libs/jah/include/core/Model.hpp libs/jah/src/core/Model.cpp libs/jah/include/core/View.hpp libs/jah/src/core/View.cpp libs/jah/include/core/Controller.hpp libs/jah/src/core/Controller.cpp libs/jah/include/core/Wizard.hpp libs/jah/src/core/Wizard.cpp)
add_library(jah_hooks libs/jah/include/hooks/Hook.hpp libs/jah/src/hooks/Hook.cpp libs/jah/include/hooks/Ci.hpp libs/jah/src/hooks/Ci.cpp)
add_library(jah_hub libs/jah/include/hub/Discussions.hpp libs/jah/src/hub/Discussions.cpp libs/jah/include/hub/Hub.hpp libs/jah/src/hub/Hub.cpp libs/jah/include/hub/Ticket.hpp libs/jah/src/hub/Ticket.cpp)

add_library(api_hub api/hub/include/Discussions.hpp api/hub/src/Discussions.cpp api/hub/include/Tickets.hpp api/hub/src/Tickets.cpp)
add_library(api_atoms api/atoms/include/Queue.hpp api/atoms/src/Queue.cpp api/atoms/include/Singularity.hpp api/atoms/src/Singularity.cpp api/atoms/include/Atom.hpp api/atoms/src/Atom.cpp api/atoms/include/Forge.hpp api/atoms/src/Forge.cpp)
add_library(api_branches api/branches/include/Flow.hpp api/branches/src/Flow.cpp api/branches/include/Branches.hpp api/branches/src/Branches.cpp)

add_executable(hub apps/gui/hub/src/main.cpp apps/gui/hub/libs/include/Hub.hpp apps/gui/hub/libs/src/Hub.cpp)
target_link_libraries(hub api_hub ${GTK4_LIBRARIES} CURL::libcurl)

add_executable(flow apps/gui/flow/src/main.cpp apps/gui/flow/libs/include/Flow.hpp apps/gui/flow/libs/src/Flow.cpp)
target_link_libraries(flow jah_atoms jah_branch jah_config jah_core jah_hooks jah_hub ${GTK4_LIBRARIES} CURL::libcurl)

add_executable(ji apps/gui/ji/src/main.cpp apps/gui/ji/libs/include/Ji.hpp apps/gui/ji/libs/src/Ji.cpp)
target_link_libraries(ji jah_atoms jah_branch jah_config jah_core jah_hooks jah_hub ${GTK4_LIBRARIES} CURL::libcurl)

add_executable(jah apps/cli/jah/main.cpp)
target_link_libraries(jah jah_atoms jah_branch jah_config jah_core jah_hooks jah_hub)

add_executable(atomizer apps/gui/atomizer/src/main.cpp apps/gui/atomizer/libs/include/Atomizer.hpp apps/gui/atomizer/libs/src/Atomizer.cpp)
target_link_libraries(atomizer api_atoms ${GTK4_LIBRARIES} CURL::libcurl)

add_executable(atom apps/api/atoms/main.cpp)
target_link_libraries(atom api_atoms Boost::system Boost::thread)

add_executable(branches apps/api/branches/main.cpp)
target_link_libraries(branches api_branches)

# Install executables
install(TARGETS hub flow ji jah atomizer atom branches
        RUNTIME DESTINATION bin
)

# Install libraries
install(TARGETS jah_atoms jah_branch jah_config jah_core jah_hooks jah_hub api_hub api_atoms api_branches
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

# Install desktop files
install(FILES
        apps/gui/hub/desktop/hub.desktop
        apps/gui/flow/desktop/flow.desktop
        apps/gui/ji/desktop/ji.desktop
        apps/gui/atomizer/desktop/org.jah.atomizer.desktop
        DESTINATION share/applications
)

# Install desktop files
install(FILES
        apps/gui/atomizer/icons/Atomizer/scalable/apps/org.jah.atomizer.svg
        DESTINATION share/icons/hicolor/scalable/apps/
)


# Install headers
install(DIRECTORY libs/jah/include/ api/hub/include/ api/atoms/include/ api/branches/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp"
)

