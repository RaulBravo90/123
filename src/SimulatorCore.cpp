
#include "dispatcher/core/SimulatorCore.hpp"

#include <algorithm>
#include <iterator>
#include <utility>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace dispatcher {

void SimulatorCore::saveTextLog(
    const std::filesystem::path& path
) const {
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream out(path, std::ios::app);

    if (!out) {
        throw std::runtime_error("Cannot open event log file");
    }

    for (const auto& event : events_) {
        out << "Event #" << event.id
            << " [" << eventTypeName(event.type)
            << ", priority=" << event.priority << "]: "
            << event.message << '\n';
    }
}

    void SimulatorCore::addParameter(ParameterRecord parameter) {
    parameters_.push_back(std::move(parameter));
}

void SimulatorCore::addEvent(Event event) {
    events_.push_back(std::move(event));
}

CommandResult SimulatorCore::applyOperatorCommand(
    const std::string& name,
    const std::string& value
) {
    const auto found = std::find_if(
        parameters_.begin(),
        parameters_.end(),
        [&name](const ParameterRecord& parameter) {
            return parameter.name == name;
        }
    );

    if (found == parameters_.end()) {
        return CommandError{
            "Parameter not found: " + name
        };
    }

    if (value.empty()) {
        return CommandWarning{
            "Empty value. Parameter was not changed: " + name
        };
    }

    found->value = value;

    return CommandOk{
        "Parameter updated: " + name + " = " + value
    };
}

std::optional<ParameterRecord> SimulatorCore::findParameter(
    const std::string& name) const {
    const auto found = std::find_if(
        parameters_.begin(),
        parameters_.end(),
        [&name](const ParameterRecord& parameter) {
            return parameter.name == name;
        });

    if (found == parameters_.end()) {
        return std::nullopt;
    }

    return *found;
}

std::size_t SimulatorCore::countAlarmEvents() const {
    return static_cast<std::size_t>(
        std::count_if(
            events_.begin(),
            events_.end(),
            [](const Event& event) {
                return event.type == EventType::Warning
                    || event.type == EventType::Alarm;
            }
        )
    );
}

std::vector<Event> SimulatorCore::getAlarmEvents() const {
    std::vector<Event> result;

    std::copy_if(
        events_.begin(),
        events_.end(),
        std::back_inserter(result),
        [](const Event& event) {
            return event.type == EventType::Warning
                || event.type == EventType::Alarm;
        }
    );

    return result;
}

std::vector<Event> SimulatorCore::getSortedEvents() const {
    std::vector<Event> result = events_;

    std::sort(
        result.begin(),
        result.end(),
        [](const Event& left, const Event& right) {
            if (left.priority != right.priority) {
                return left.priority > right.priority;
            }

            return left.id < right.id;
        }
    );

    return result;
}

std::map<EventType, std::size_t>
SimulatorCore::getEventStatistics() const {
    std::map<EventType, std::size_t> statistics;

    for (const auto& event : events_) {
        ++statistics[event.type];
    }

    return statistics;
}

std::vector<std::string>
SimulatorCore::getParameterDescriptions() const {
    std::vector<std::string> result;
    result.reserve(parameters_.size());

    std::transform(
        parameters_.begin(),
        parameters_.end(),
        std::back_inserter(result),
        [](const ParameterRecord& parameter) {
            return parameter.name + " = " + parameter.value;
        }
    );

    return result;
}

std::string eventTypeName(EventType type) {
    switch (type) {
        case EventType::Info:
            return "Info";
        case EventType::OperatorAction:
            return "Operator action";
        case EventType::Warning:
            return "Warning";
        case EventType::Alarm:
            return "Alarm";
    }

    return "Unknown";
}

} // namespace dispatcher
