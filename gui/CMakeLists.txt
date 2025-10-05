add_executable(cpu_scheduler_qt
    main.cpp
    mainwindow.cpp
    ganttwidget.cpp
)

target_include_directories(cpu_scheduler_qt PRIVATE ${CMAKE_SOURCE_DIR}/src gui)
target_link_libraries(cpu_scheduler_qt PRIVATE Qt6::Widgets core)
