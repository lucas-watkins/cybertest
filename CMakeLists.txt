set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
cmake_minimum_required(VERSION 3.3)
project(cybertest)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

FetchContent_Declare(
        OpenXLSX
        GIT_REPOSITORY https://github.com/troldal/OpenXLSX
        GIT_TAG d566370
        GIT_SHALLOW 1
)

FetchContent_Declare(
        Asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio
        GIT_TAG bd500f0
        GIT_SHALLOW 1
)

FetchContent_Declare(
        Crow
        GIT_REPOSITORY https://github.com/CrowCpp/Crow
        GIT_TAG f8c060c
        GIT_SHALLOW 1
)

FetchContent_MakeAvailable(OpenXLSX Asio)

set(ASIO_INCLUDE_DIR "${asio_SOURCE_DIR}/asio/include")
FetchContent_MakeAvailable(Crow)

file(GLOB RESOURCES "web/*.*")
message(NOTICE "Web Resources: ${RESOURCES}")
file(COPY ${RESOURCES} DESTINATION web/)

file(GLOB TEMPLATES "web/templates/*.*")
message(NOTICE "Web Templates: ${TEMPLATES}")
file(COPY ${TEMPLATES} DESTINATION web/templates/)

add_executable(cybertest src/main.cpp src/validation.cpp
        src/questiongenerator.cpp)
target_include_directories(cybertest PRIVATE include)
target_link_libraries(cybertest PRIVATE OpenXLSX::OpenXLSX Crow::Crow)
