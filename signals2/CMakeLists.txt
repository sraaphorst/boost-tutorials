set(apps
        example67_1
        example67_2
        example67_3
        play
        play2
        )

foreach (app ${apps})
    add_executable(${app} ${app}.cpp)
    target_link_libraries(${app} ${CMAKE_THREAD_LIBS_INIT})
endforeach()