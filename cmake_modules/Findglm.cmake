CMAKE_MINIMUM_REQUIRED(VERSION 3.11)
if (NOT glm_FOUND)
    INCLUDE(FetchContent)
    FetchContent_Declare(glm URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.tar.gz)
    FetchContent_GetProperties(glm)
    if (NOT glm_POPULATED)
        SET(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(glm)
        SET(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        ADD_SUBDIRECTORY(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
        SET(glm_FOUND TRUE)
    endif()
endif()
