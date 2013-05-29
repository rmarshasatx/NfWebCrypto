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

# The settings in this file are common to all cros builds

get_filename_component(TOOLCHAIN_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
include("${TOOLCHAIN_DIR}/common.cmake")

if(NOT CROS_FLAGS)
    # Google needs cros builds to have -ggdb symbols to support their crash
    # logger infrastructure. These must be stripped from release targets.
    # cros builds need a sysroot for all builds.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb --sysroot=${GENERIC_SYSROOT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb --sysroot=${GENERIC_SYSROOT} -DCHROMEOS")
    # Once already appended, don't append again
    set(CROS_FLAGS TRUE CACHE BOOLEAN "." FORCE)
endif()
