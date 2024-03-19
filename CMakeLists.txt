cmake_minimum_required(VERSION 3.0.0)
project(MakerbaseClient VERSION 0.1.0)

# if (CMAKE_SYSTEM_NAME MATCHES "Linux")
#     find_package(PkgConfig REQUIRED)
#     pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
#     message(STATUS "----- GTK3_INCLUDE_DIRS: ${GTK3_INCLUDE_DIRS}")
#     message(STATUS "----- GKT3_LIBRARIES: ${GTK3_LIBRARIES}")
#     message(STATUS "----- GTK3_LINK_LIBRARIES: ${GTK3_LINK_LIBRARIES}")
#     include_directories(${GTK3_INCLUDE_DIRS})
#     link_directories(${GTK3_LIBRARY_DIRS})
#     list(APPEND FC_DEP_LIBS ${GTK3_LIBRARIES})
# endif()

include(CTest)
enable_testing()

include_directories(./include)
# link_directories(./lib)

add_executable(xindi main.cpp)
add_subdirectory(src)
target_link_libraries(xindi src -lpthread -lboost_system -lwpa_client)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)