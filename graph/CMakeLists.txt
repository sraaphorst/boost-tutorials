set(apps
        graph
        graph2
        graph3
        bfs
        bfs_paths
        bfs_distpaths
        dijkstra_paths
        dijkstra_userdef
        play_props)


foreach (app ${apps})
    add_executable(${app} ${app}.cpp)
        target_link_libraries(${app} ${Boost_LOG_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
endforeach()
