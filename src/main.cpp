#include "dispatcher/core/parameter.hpp"
#include "dispatcher/core/range.hpp"
#include "dispatcher/core/rules.hpp"
#include "dispatcher/model/DispatcherModel.hpp"
#include "dispatcher/model/Robot.hpp"
#include "dispatcher/model/SafetyZone.hpp"
#include "dispatcher/model/RobotPath.hpp"
#include "dispatcher/model/Alarm.hpp"
#include "dispatcher/model/ARRobotHint.hpp"
#include "dispatcher/visual/ConsoleVisualRenderer.hpp"
#include "dispatcher/core/SimulatorCore.hpp"
#include "dispatcher/EventLog.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include <variant>
#include <limits>
#include <fstream>

using namespace dispatcher;

int main() {
    std::filesystem::create_directories("logs");

{
    std::ofstream clearLog("logs/session_log.txt", std::ios::trunc);
}



    DispatcherModel model;

    // Робот роботизированной линии
    model.addElement(std::make_unique<Robot>("R-01", 85, ElementStatus::Normal)
);
       // Зона безопасности
    model.addElement(std::make_unique<SafetyZone>(
            "Z-01",
            SafetyZoneState::Clear
        )
    );

    // Траектория движения робота
   model.addElement(
    std::make_unique<RobotPath>(
        "P-01",
        "Assembly point",
        "Loading station",
        ElementStatus::Normal
    )
);

    // Предупреждение диспетчеру
    model.addElement(
    std::make_unique<Alarm>(
        "A-01",
        "Battery level is low",
        ElementStatus::Warning
    )
);

    // AR-подсказка для робота
   model.addElement(
    std::make_unique<ARRobotHint>(
        "AR-01",
        "Check robot R-01 battery",
        "R-01",
        ElementStatus::Warning
    )
);

    std::cout << "----- Robotic Line -----" << std::endl;

    const auto summaries = model.collectSummaries();

    for (const auto& summary : summaries) {
        std::cout << summary << std::endl;
    }

    std::cout << "----- Visuals -----" << std::endl;

    ConsoleVisualRenderer renderer;
    renderer.render(model.collectVisualDescriptors());

        std::cout << "----- Parameter Checks -----" << std::endl;

    Parameter<int> robotCount(
        "robot_count",
        "Robot count",
        5,
        "robots"
    );

    Parameter<int> taskQueue(
        "task_queue",
        "Task queue",
        12,
        "tasks"
    );

    Parameter<double> batteryLevel(
        "battery_level",
        "Battery level",
        85.0,
        "%"
    );

    std::vector<std::unique_ptr<ICheckRule>> rules;

    rules.push_back(std::make_unique<RangeRule<int>>(
        robotCount,
        Range<int>(1, 10),
        "Robot count is outside the normal range"
    ));

    rules.push_back(std::make_unique<RangeRule<int>>(
        taskQueue,
        Range<int>(0, 20),
        "Task queue is outside the normal range"
    ));

    rules.push_back(std::make_unique<RangeRule<double>>(
        batteryLevel,
        Range<double>(20.0, 100.0),
        "Battery level is outside the normal range"
    ));

    for (const auto& rule : rules) {
        const CheckResult result = rule->check();

        std::cout << result.parameterId
                  << ": " << result.message
                  << std::endl;
    }

        std::cout << "----- Safety Zone Check -----" << std::endl;

    Parameter<std::string> safetyZoneState(
        "safety_zone_state",
        "Safety zone state",
        "Clear",
        ""
    );

    if (safetyZoneState.value() == "Clear") {
        std::cout << safetyZoneState.id() << ": OK" << std::endl;
    } else {
        std::cout << safetyZoneState.id()
                  << ": Safety zone requires attention"
                  << std::endl;
    }

    std::cout << "----- STL Parameters and Events -----" << std::endl;

    SimulatorCore core;

    core.addParameter({"robot_count", "5"});
    core.addParameter({"task_queue", "12"});
    core.addParameter({"safety_zone_state", "Clear"});
    core.addParameter({"battery_level", "85%"});

    core.addEvent({1, EventType::Info, "Robotic line started", 1});
    core.addEvent({2, EventType::Warning, "Robot battery is low", 3});
    core.addEvent({3, EventType::OperatorAction, "Operator checked robot", 2});
    core.addEvent({4, EventType::Alarm, "Safety zone violation", 5});

    EventLog eventLog("logs/session_log.txt");
    eventLog.write("Dispatcher session started");

    core.saveTextLog("logs/session_log.txt");


    int choice = -1;

    while (choice != 0) {
        std::cout << "\n===== Simulator Menu =====" << std::endl;
        std::cout << "1. Find parameter by name" << std::endl;
        std::cout << "2. Show alarm and warning events" << std::endl;
        std::cout << "3. Show events sorted by priority" << std::endl;
        std::cout << "4. Show event statistics" << std::endl;
        std::cout << "5. Show parameter list" << std::endl;
        std::cout << "6. Execute operator command" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Choose an option: ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Exiting menu." << std::endl;
            break;
        }

        switch (choice) {
            case 1: {
                std::string name;

                std::cout << "Enter parameter name: ";
                std::cin >> name;

                auto parameter = core.findParameter(name);

                if (parameter.has_value()) {
                     std::cout << parameter->name
                               << " = "
                               << parameter->value
                             << '\n';
                } else {
                        std::cout << "Parameter not found.\n";
                       }

                break;
            }

            case 2: {
                const auto alarmEvents = core.getAlarmEvents();

                std::cout << "Alarm and warning events: "
                          << alarmEvents.size() << std::endl;

                for (const auto& event : alarmEvents) {
                    std::cout << "  [" << event.priority << "] "
                              << event.id << ": "
                              << event.message << " ("
                              << eventTypeName(event.type) << ")"
                              << std::endl;
                }

                break;
            }

            case 3: {
                const auto sortedEvents = core.getSortedEvents();

                std::cout << "Events sorted by priority:"
                          << std::endl;

                for (const auto& event : sortedEvents) {
                    std::cout << "  [" << event.priority << "] "
                              << event.id << ": "
                              << event.message << std::endl;
                }

                break;
            }

            case 4: {
                std::cout << "Event statistics:" << std::endl;

                for (const auto& [type, count] :
                     core.getEventStatistics()) {
                    std::cout << "  " << eventTypeName(type)
                              << ": " << count << std::endl;
                }

                break;
            }

            case 5: {
                std::cout << "Parameter list:" << std::endl;

                for (const auto& description :
                     core.getParameterDescriptions()) {
                    std::cout << "  " << description << std::endl;
                }

                break;
            }

            case 6: {
                    std::string name;
                    std::string value;

                    std::cout << "Enter parameter name: ";
                    std::cin >> name;

                    std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                  '\n'
                    );

                    std::cout << "Enter new value (press Enter to leave empty): ";
                    std::getline(std::cin, value);

                        const CommandResult result =
                        core.applyOperatorCommand(name, value);

                    std::visit( [&eventLog](const auto& commandResult) {
                        using T =
                            std::decay_t<decltype(commandResult)>;

                        if constexpr (
                            std::is_same_v<T, CommandOk>
                        ) {
                            std::cout << "[OK] "
                                      << commandResult.message
                                      << '\n';
                             eventLog.write(
                    std::string("[OK] ") + commandResult.message
                            );
                        } else if constexpr (
                            std::is_same_v<T, CommandWarning>
                        ) {
                            std::cout << "[WARNING] "
                                      << commandResult.message
                                      << '\n';
                                      eventLog.write(
                    std::string("[WARNING] ") + commandResult.message
                            );
                        } else if constexpr (
                            std::is_same_v<T, CommandError>
                        ) {
                            std::cout << "[ERROR] "
                                      << commandResult.message
                                      << '\n';
                                      eventLog.write(
                    std::string("[ERROR] ") + commandResult.message
                            );
                        }
                    },
                    result
                );

                break;
            }

            case 0:
                std::cout << "Exiting simulator menu." << std::endl;
                break;

            default:
                std::cout << "Unknown option. Try again."
                          << std::endl;
                break;
        }
    }

    return 0;
}