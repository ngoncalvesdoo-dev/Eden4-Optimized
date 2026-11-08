# SPDX-FileCopyrightText: 2015 Greg Wicks <gpwicks@email.wm.edu>
# SPDX-License-Identifier: GPL-2.0-or-later

add_library(getopt
    getopt.c
    getopt.h
)

create_target_directory_groups(getopt)

target_compile_definitions(getopt PUBLIC STATIC_GETOPT)
target_include_directories(getopt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Flags autorais adicionadas para otimização máxima de performance:
target_compile_options(getopt PRIVATE -O3 -ffast-math -w)
