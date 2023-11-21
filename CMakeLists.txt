cmake_minimum_required(VERSION 3.12)
project(SpaceOddity)

set(SFML_DIR "/usr/local/Cellar/sfml/2.6.1/lib/cmake/SFML")
set(SFML_INCLUDE_DIRS "/usr/local/Cellar/sfml/2.6.1/include")
find_package(GLEW REQUIRED)
find_path(GLEW_INCLUDE_DIR NAMES GL/glew.h HINTS ${GLEW_ROOT_DIR}/include)

find_package(SFML REQUIRED COMPONENTS system window graphics)

# sanity check for glew
if (NOT GLEW_INCLUDE_DIR)
    message(FATAL_ERROR "GLEW headers not found.")
endif()
if (NOT SFML_INCLUDE_DIRS) # and for sfml
    message(FATAL_ERROR "SFML headers not found.")
endif()

find_package(glfw3 REQUIRED)
find_package(RapidJson REQUIRED)
find_package(Assimp REQUIRED)
        
# recursively populate the source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Create the executable
add_executable(spaceoddity ${SOURCE_FILES})
target_include_directories(spaceoddity PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${GLEW_INCLUDE_DIR}
    ${SFML_INCLUDE_DIRS}
    ${RAPID_JSON_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/glm
    ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(spaceoddity PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    GLEW::GLEW
    glfw
    ${ASSIMP_LIBRARIES}
)
target_compile_features(spaceoddity PRIVATE cxx_std_17)