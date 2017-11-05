
cmake_minimum_required(VERSION 2.8.4)

project(NetworkForwarder)

set(app_exe nfer)

# header
file(GLOB headers *.h)
file(GLOB sources *.c)

# resource
file(GLOB resources resource.rc)

# for IDE
# source_group("Header files" FILES ${headers})
source_group("Source files" FILES ${headers} ${sources})
source_group("Resource files" FILES ${resources})

# unicode
# add_definitions(-D_UNICODE -DUNICODE)

# debug
# add_definitions(-DNDEBUG)
# add_definitions(-g)

add_executable(${app_exe} ${sources} ${headers} ${resources})

# platform
if (MINGW)
    add_definitions(-D_WIN32)
    target_link_libraries(${app_exe} ws2_32)
elseif (WIN32)
    add_definitions(-D_WIN32)
elseif (UNIX)
    add_definitions(-D_LINUX)
endif()
