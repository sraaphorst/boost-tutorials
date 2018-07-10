set(apps
        example64_1
        example64_2
        example64_3
        example64_4
        example64_5
        example64_6
        example64_7
        enums
        )

foreach (app ${apps})
    add_executable(${app} ${app}.cpp)
    target_link_libraries(${app} ${Boost_SERIALIZATION_LIBRARY})
endforeach()
