cmake_minimum_required(VERSION 3.10)
project(MyLittleOS)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fdiagnostics-color=always)
endif()

set(TARGET_NAME mario_daemon)

find_package(Threads REQUIRED)
find_library(RT_LIBRARY rt)

add_executable(${TARGET_NAME} 
    main.c
    # Core
    src/core/system.c
    src/core/osal.c
    src/core/log.c
    src/core/buffer.c
    src/core/async.c
    # Driver
    src/driver/driverCommon.c
    # Service
    src/service/serviceCommon.c
    # App
    src/app/appCommon.c
    src/app/sample/app.c
    src/app/sample/appBridge.c
)

target_include_directories(${TARGET_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/inc
)

target_compile_definitions(${TARGET_NAME} PUBLIC
    DIRAC_OS=1
    APP_SAMPLE=1
)

target_link_libraries(${TARGET_NAME} PRIVATE 
    Threads::Threads 
    ${RT_LIBRARY}
)
