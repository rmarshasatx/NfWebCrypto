#
# Copyright 2013 Netflix, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.

# Settings specific to cros ARM builds
# Choose this leaf file by pointing to it on the cmake command line with
# -DCMAKE_TOOLCHAIN_FILE=<this file>

# Only daisy is supported right now. Might be better to set to arm-generic.
set(GENERIC_BOARD "daisy")
set(GENERIC_SYSROOT "/build/${GENERIC_BOARD}")

# The cross-tools binaries
set(CROSSTOOLS_PREFIX "/usr/bin/armv7a-cros-linux-gnueabi")

# Toolchain paths
set(CMAKE_C_COMPILER "${CROSSTOOLS_PREFIX}-gcc")
set(CMAKE_CXX_COMPILER "${CROSSTOOLS_PREFIX}-g++")

get_filename_component(TOOLCHAIN_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
include("${TOOLCHAIN_DIR}/common/cros_common.cmake")
