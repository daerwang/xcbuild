#
# Copyright (c) 2015-present, Facebook, Inc.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree. An additional grant
# of patent rights can be found in the PATENTS file in the same directory.
#

add_library(dependency SHARED
            Sources/DependencyInfo.cpp
            Sources/DependencyInfoFormat.cpp
            Sources/BinaryDependencyInfo.cpp
            Sources/DirectoryDependencyInfo.cpp
            Sources/MakefileDependencyInfo.cpp
            )

target_link_libraries(dependency PUBLIC util ext)
target_include_directories(dependency PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Headers")
install(TARGETS dependency DESTINATION lib)

add_executable(dependency-info-tool Tools/dependency-info-tool.cpp)
target_link_libraries(dependency-info-tool dependency util)
install(TARGETS dependency-info-tool DESTINATION bin)

if (BUILD_TESTING)
  ADD_UNIT_GTEST(dependency BinaryDependencyInfo Tests/test_BinaryDependencyInfo.cpp)
  ADD_UNIT_GTEST(dependency MakefileDependencyInfo Tests/test_MakefileDependencyInfo.cpp)
  ADD_UNIT_GTEST(dependency DirectoryDependencyInfo Tests/test_DirectoryDependencyInfo.cpp)
endif ()
