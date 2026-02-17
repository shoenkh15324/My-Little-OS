# 테스트 리스트
set(TEST_SOURCES
    testBuffer.c
    #testAsync.c
    #testLog.c
)

# 각 테스트 파일마다 exe 생성 & src 코드 연결
foreach(testFile ${TEST_SOURCES})
    # exe 이름: test_파일이름에서 .c 제거
    get_filename_component(testName ${testFile} NAME_WE)
    add_executable(${testName} ${testFile})
    # include 경로
    target_include_directories(${testName} PRIVATE ${PROJECT_SOURCE_DIR}/inc)
    # compile definitions
    target_compile_definitions(${testName} PRIVATE

    )
    # 필요한 src 코드 링크
    if(${testName} STREQUAL "testBuffer")
        target_sources(${testName} PRIVATE
            ${PROJECT_SOURCE_DIR}/src/core/feature/buffer.c
            ${PROJECT_SOURCE_DIR}/src/core/feature/log.c
            ${PROJECT_SOURCE_DIR}/src/core/feature/osal.c
        )
    else()
        target_sources(${testName} PRIVATE
            ${PROJECT_SOURCE_DIR}/src/core/feature/buffer.c
            ${PROJECT_SOURCE_DIR}/src/core/feature/async.c
            ${PROJECT_SOURCE_DIR}/src/core/feature/log.c
        )
    endif()
    # CTest에 등록
    add_test(NAME ${testName} COMMAND ${testName})
endforeach()
