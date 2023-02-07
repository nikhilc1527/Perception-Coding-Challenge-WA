cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    perception
    VERSION 0.1.0
    DESCRIPTION "Response for WA Perception Coding Challenge"
    HOMEPAGE_URL "https://github.com/nikhilc1527/Perception-Coding-Challenge-WA"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    perception_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    perception_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(perception_lib PUBLIC cxx_std_17)

# ---- Declare executable ----

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(perception_exe source/main.cpp)
add_executable(perception::exe ALIAS perception_exe)

set_property(TARGET perception_exe PROPERTY OUTPUT_NAME perception)

target_compile_features(perception_exe PRIVATE cxx_std_17)

target_link_libraries(perception_exe PRIVATE perception_lib opencv_core opencv_imgcodecs ${OpenCV_LIBS})

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT perception_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of perception"
  )
endif()

include(cmake/dev-mode.cmake)
