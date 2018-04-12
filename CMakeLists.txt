cmake_minimum_required(VERSION 3.5.1)

macro(pg_set_option var default type docstring)
    if (NOT DEFINED ${var})
        set(${var} ${default})
    endif()

    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

project(PhysGame)

pg_set_option(CMAKE_BUILD_TYPE "Release" STRING "Type of build")

pg_set_option(PG_INTEGRATOR "Euler" STRING "The integrator to use in physics calculations")
set_property(CACHE PG_INTEGRATOR PROPERTY STRINGS
  "Euler" "Heun" "Runge-Kutta ^ 4"
)

if (PG_INTEGRATOR MATCHES "Runge-Kutta ^ 4")
  add_definitions(-DPG_INTEGRATOR_RUNGE_KUTTA_POW4)
elseif (PG_INTEGRATOR MATCHES "Heun")
  add_definitions(-DPG_INTEGRATOR_HEUN)
endif()

add_subdirectory(src)
