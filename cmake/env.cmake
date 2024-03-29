#
# Copyright (c) 2020 Florian Becker <fb@vxapps.com> (VX APPS).
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

# for debugging of build steps
option(CMAKE_VERBOSE_MAKEFILE "Show the complete build commands" OFF)

# possibility to disable build steps
option(LOGGER_BUILD_EXAMPLES "Build examples" ON)
option(LOGGER_BUILD_TESTS "Build tests" ON)

# for remove log severities
# Possible values:
# LOGGINGALL
# LOGGINGVERBOSE same as LOGGINGALL
# LOGGINGDEBUG
# LOGGINGWARNING
# LOGGINGERROR
# LOGGINGFATAL
# LOGGINGNONE - drop everything
# Default is LOGGINGINFO, but this is not an vailid definition :)
# Example for build type:
# add_compile_options("$<$<CONFIG:DEBUG>:-DLOGGINGVERBOSE>")
# Example for all build types:
# add_compile_options("-DLOGGINGVERBOSE")

# General
set(CMAKE_TLS_VERIFY TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

# CMake
set(CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Project
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(3RDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(TESTS_SHARED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests/shared)

# Force C++17 or C++20 if available
include(CheckCXXCompilerFlag)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR WIN32)
  check_cxx_compiler_flag(/std:c++20 HAVE_FLAG_STD_CXX20)
else()
  check_cxx_compiler_flag(-std=c++20 HAVE_FLAG_STD_CXX20)
  check_cxx_compiler_flag(-std=c++2a HAVE_FLAG_STD_CXX2A)
endif()

# Clang-8 have some issues, that are not repairable
if(CMAKE_CXX_COMPILER_ID MATCHES "[cC][lL][aA][nN][gG]" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9")
  set(HAVE_FLAG_STD_CXX20 0)
  set(HAVE_FLAG_STD_CXX2A 0)
endif()

if(HAVE_FLAG_STD_CXX20 OR HAVE_FLAG_STD_CXX2A)
  set(CMAKE_CXX_STANDARD 20)
else()
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# IPO/LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
  # It's available, set it for all following items
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else()
  message(WARNING "IPO is not supported: ${output}")
endif()

# Warning flags
# Case insensitive match
if(CMAKE_CXX_COMPILER_ID MATCHES "[cC][lL][aA][nN][gG]")
  include(${CMAKE}/clang_warnings.cmake)

  set(WARNING_FLAGS_SPACED "")
  foreach(WARNING_FLAG ${WARNING_FLAGS})
    set(WARNING_FLAGS_SPACED "${WARNING_FLAGS_SPACED} ${WARNING_FLAG}")
  endforeach()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Werror -Weffc++")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS_SPACED}")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  include(${CMAKE}/gcc_warnings.cmake)

  set(WARNING_FLAGS_SPACED "")
  foreach(WARNING_FLAG ${WARNING_FLAGS})
    set(WARNING_FLAGS_SPACED "${WARNING_FLAGS_SPACED} ${WARNING_FLAG}")
  endforeach()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Weffc++ -Wpedantic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS_SPACED}")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  include(${CMAKE}/msvc_warnings.cmake)

  set(WARNING_FLAGS_SPACED "")
  foreach(WARNING_FLAG ${WARNING_FLAGS})
    set(WARNING_FLAGS_SPACED "${WARNING_FLAGS_SPACED} ${WARNING_FLAG}")
  endforeach()

  # Force to always compile with W4
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS_SPACED}")
endif()

# Project modules/variables
set(CMAKE_MODULE_PATH ${CMAKE}/modules)

# Includes
include(${CMAKE}/create_package.cmake)
include(${CMAKE}/doxygen.cmake)
include(${CMAKE}/find_package.cmake)
