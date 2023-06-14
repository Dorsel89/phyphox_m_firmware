# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_sources(app PRIVATE src/platform.c)
target_sources(app PRIVATE src/VL53L4CD_api.c)
target_sources(app PRIVATE src/tof.c)

target_sources(app PRIVATE src/bas.c)
target_sources(app PRIVATE src/ble.c)
target_sources(app PRIVATE src/shtc3.c)
target_sources(app PRIVATE src/mlx90392.c)
target_sources(app PRIVATE src/bmp5.c)
target_sources(app PRIVATE src/bmi3.c)
target_sources(app PRIVATE src/bmi323.c)
target_sources(app PRIVATE src/bmiZephyr.c)
target_sources(app PRIVATE src/bmpZephyr.c)
target_sources(app PRIVATE src/main.c)
