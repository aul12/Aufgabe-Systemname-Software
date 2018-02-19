cmake_minimum_required(VERSION 3.9)
project(BurningShip C)
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GDK_PIXBUFF REQUIRED gdk-pixbuf-2.0)

set(CMAKE_C_STANDARD 11)

add_executable(BurningShip main.c burning_ship.c)

target_link_libraries(BurningShip ${GDK_PIXBUFF_LIBRARIES} m)
target_include_directories(BurningShip PUBLIC ${GDK_PIXBUFF_INCLUDE_DIRS})
target_compile_options(BurningShip PUBLIC ${GDK_PIXBUFF_CFLAGS_OTHER})
