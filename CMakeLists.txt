cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(dispatcher_trainer
    src/main.cpp

    src/model/DispatcherModel.cpp
    src/model/Robot.cpp
    src/model/SafetyZone.cpp
    src/model/RobotPath.cpp
    src/model/Alarm.cpp
    src/model/ARRobotHint.cpp
    src/SimulatorCore.cpp
    src/EventLog.cpp

    src/visual/ConsoleVisualRenderer.cpp
)

target_include_directories(dispatcher_trainer PRIVATE include)

target_compile_options(dispatcher_trainer PRIVATE -Wall -Wextra -Wpedantic)
enable_testing()
add_executable(test_template_rules
    tests/test_template_rules.cpp
)

target_include_directories(test_template_rules PRIVATE include)

target_compile_options(test_template_rules PRIVATE -Wall -Wextra -Wpedantic)

add_test(NAME template_rules COMMAND test_template_rules)
