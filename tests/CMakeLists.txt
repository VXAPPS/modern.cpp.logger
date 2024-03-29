#
# Copyright (c) 2021 Florian Becker <fb@vxapps.com> (VX APPS).
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

project(test_simple_null)

add_executable(${PROJECT_NAME}
  ${PROJECT_NAME}.cpp
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  modern.cpp.logger
  GTest::gtest_main
  Threads::Threads
)

gtest_add_tests(${PROJECT_NAME}
  SOURCES ${PROJECT_NAME}.cpp
)

project(test_simple_cout)

add_executable(${PROJECT_NAME}
  ${PROJECT_NAME}.cpp
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  modern.cpp.logger
  GTest::gtest_main
  Threads::Threads
)

gtest_add_tests(${PROJECT_NAME}
  SOURCES ${PROJECT_NAME}.cpp
)

project(test_simple_file)

add_executable(${PROJECT_NAME}
  ${PROJECT_NAME}.cpp
  shared/TestHelper.cpp
  shared/TestHelper.h
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  modern.cpp.logger
  GTest::gtest_main
  Threads::Threads
)

gtest_add_tests(${PROJECT_NAME}
  SOURCES ${PROJECT_NAME}.cpp
)

project(test_simple_xml)

add_executable(${PROJECT_NAME}
  ${PROJECT_NAME}.cpp
  shared/TestHelper.cpp
  shared/TestHelper.h
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  modern.cpp.logger
  GTest::gtest_main
  Threads::Threads
)

gtest_add_tests(${PROJECT_NAME}
  SOURCES ${PROJECT_NAME}.cpp
)

project(test_thread_null)

add_executable(${PROJECT_NAME}
  ${PROJECT_NAME}.cpp
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  modern.cpp.logger
  GTest::gtest_main
  Threads::Threads
)

gtest_add_tests(${PROJECT_NAME}
  SOURCES ${PROJECT_NAME}.cpp
)

project(test_thread_cout)

add_executable(${PROJECT_NAME}
  ${PROJECT_NAME}.cpp
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  modern.cpp.logger
  GTest::gtest_main
  Threads::Threads
)

gtest_add_tests(${PROJECT_NAME}
  SOURCES ${PROJECT_NAME}.cpp
)

project(test_thread_file)

add_executable(${PROJECT_NAME}
  ${PROJECT_NAME}.cpp
  shared/TestHelper.cpp
  shared/TestHelper.h
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  modern.cpp.logger
  GTest::gtest_main
  Threads::Threads
)

gtest_add_tests(${PROJECT_NAME}
  SOURCES ${PROJECT_NAME}.cpp
)

project(test_thread_xml)

add_executable(${PROJECT_NAME}
  ${PROJECT_NAME}.cpp
  shared/TestHelper.cpp
  shared/TestHelper.h
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  modern.cpp.logger
  GTest::gtest_main
  Threads::Threads
)

gtest_add_tests(${PROJECT_NAME}
  SOURCES ${PROJECT_NAME}.cpp
)
