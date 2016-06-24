cmake_minimum_required(VERSION 2.8.7)

project(arb)

set (CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug, Release)" FORCE)

set (BUILD_SHARED_LIBS yes CACHE BOOL "Build shared library or not")
set (BUILD_TESTS yes CACHE BOOL "Build shared library or not")

if (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR
        CMAKE_BUILD_TYPE STREQUAL "Release"))
    message("${CMAKE_BUILD_TYPE}")
    message(FATAL_ERROR "CMAKE_BUILD_TYPE must be one of: Debug, Release (current value: '${CMAKE_BUILD_TYPE}')")
endif ()

if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(PLATFORM "x64")
else ()
    set(PLATFORM "Win32")
endif()

set(MSVC_USE_MT yes CACHE BOOL "Use MT flags when compiling in MSVC")
set(MSVC_WARNING_LEVEL 1 CACHE STRING "MSVC warning level")

if (MSVC)
    if (MSVC_USE_MT)
	    foreach(CompilerFlag CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE)
	      string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	    endforeach()
    endif()
    foreach(CompilerFlag CMAKE_C_FLAGS)
      set(${CompilerFlag} "${${CompilerFlag}} /W${MSVC_WARNING_LEVEL}")
    endforeach()
endif()


set (DEPS mpir mpfr pthreads flint)

foreach (LIB ${DEPS})
    set (LIB_NAMES ${LIB})
    if ("${LIB}" STREQUAL "mpir")
        set (LIB_NAMES ${LIB_NAMES} gmp)
    elseif ("${LIB}" STREQUAL "pthreads")
        set (LIB_NAMES ${LIB_NAMES} pthread)
    endif ()
    string (TOUPPER ${LIB} LIB_UPPER)
    find_library(${LIB_UPPER}_LIBRARY NAMES ${LIB_NAMES} HINTS ../${LIB}/${PLATFORM}/${CMAKE_BUILD_TYPE})
    if (NOT ${LIB_UPPER}_LIBRARY)
        message(FATAL_ERROR "${LIB} library not found.")
    endif()
    add_library(${LIB} UNKNOWN IMPORTED)
    set_property(TARGET ${LIB} PROPERTY IMPORTED_LOCATION ${${LIB_UPPER}_LIBRARY})
    message("${LIB} found in ${${LIB_UPPER}_LIBRARY}")

endforeach ()

set (DEP_HEADERS gmp.h mpfr.h pthread.h flint/flint.h)

foreach (HEADER ${DEP_HEADERS})
    get_filename_component(LIB ${HEADER} NAME_WE)
    string(TOUPPER ${LIB} HEADER_PKG)
    find_path(${HEADER_PKG}_INCLUDE_DIR NAMES ${HEADER} HINTS ../${LIB}/${PLATFORM}/${CMAKE_BUILD_TYPE})
    if (NOT ${HEADER_PKG}_INCLUDE_DIR)
        message(FATAL_ERROR "${HEADER} header not found.")
    endif()
    message("${HEADER} found in ${${HEADER_PKG}_INCLUDE_DIR}")
    set (DEP_INCLUDE_DIRS ${DEP_INCLUDE_DIRS} ${${HEADER_PKG}_INCLUDE_DIR})
endforeach ()

set (FOLDERS
        acb
        acb_calc
        acb_dirichlet
        acb_hypgeom
        acb_mat
        acb_modular
        acb_poly
        arb
        arb_calc
        arb_mat
        arb_poly
        arf
        bernoulli
        bool_mat
        fmpz_extras
        fmpr
        hypgeom
        mag
        partitions
)

foreach (FOLDER ${FOLDERS})
    file(GLOB TEMP "${FOLDER}/*.c")
    set(SRC ${SRC} ${TEMP})
endforeach ()

include_directories(BEFORE ${arb_SOURCE_DIR})
include_directories(BEFORE ${DEP_INCLUDE_DIRS})

add_library(arb ${SRC})

target_link_libraries(arb ${DEPS})

if(NOT MSVC)
    target_link_libraries(arb m)
endif()

install(TARGETS arb
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
        )

foreach (FOLDER ${FOLDERS})
    set(HEADERS ${HEADERS} ${FOLDER}.h)
endforeach ()

install(FILES ${HEADERS} DESTINATION include)

if (BUILD_TESTS)
    enable_testing()
    foreach (FOLDER ${FOLDERS})
        file(GLOB TEMP "${FOLDER}/test/*.c")
        foreach (TEST_SOURCE ${TEMP})
            get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
            add_executable(${FOLDER}-${TEST_NAME} ${TEST_SOURCE})
            target_link_libraries(${FOLDER}-${TEST_NAME} arb)
            add_test(${FOLDER}-${TEST_NAME} ${FOLDER}-${TEST_NAME})
        endforeach ()
    endforeach ()
endif ()

