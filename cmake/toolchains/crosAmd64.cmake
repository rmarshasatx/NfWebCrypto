# (c) 2013 Netflix, Inc.  All content herein is protected by
# U.S. copyright and other applicable intellectual property laws and
# may not be copied without the express permission of Netflix, Inc.,
# which reserves all rights.  Reuse of any of this content for any
# purpose without the permission of Netflix, Inc. is strictly
# prohibited.

# Settings specific to cros amd64 builds
# Choose this leaf file by specifying it as the cmake toolchain file

# We use the generic sysroot so the plugin will work with all devices. Make
# sure the generic sysroot has been initialized regardless of the devices the
# developer might actually be building for.
set(GENERIC_BOARD "amd64-generic")
set(GENERIC_SYSROOT "/build/${GENERIC_BOARD}")

# The cross-tools binaries
set(CROSSTOOLS_PREFIX "/usr/bin/x86_64-cros-linux-gnu")

# Toolchain paths
set(CMAKE_C_COMPILER "${CROSSTOOLS_PREFIX}-gcc")
set(CMAKE_CXX_COMPILER "${CROSSTOOLS_PREFIX}-g++")

get_filename_component(TOOLCHAIN_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
include("${TOOLCHAIN_DIR}/common/cros_common.cmake")

# Have to put all variable setting inside a context to work around cmake's
# appending behavior
if(NOT PEPPERCRYPTO_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64" CACHE STRING "CFLAGS" FORCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64" CACHE STRING "CXXFLAGS" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64" CACHE STRING "LDFLAGS" FORCE)
    set(CMAKE_RESOURCE_COMPILER_FLAGS ${CMAKE_RESOURCE_COMPILER_FLAGS} -m elf_x86_64 CACHE STRING "Resource compiler flags" FORCE)
    # Once already appended, don't append again
    set(PEPPERCRYPTO_FLAGS TRUE CACHE BOOLEAN "." FORCE)
endif()
