
#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <filesystem>

namespace dispatcher {

struct ParameterRecord {
    std::string name;
    std::string value;
};

enum class EventType {
    Info,
    OperatorAction,
    Warning,
    Alarm
};

struct Event {
    std::size_t id{};
    EventType type{EventType::Info};
    std::string message;
    int priority{};
};

struct CommandOk {
    std::string message;
};

struct CommandWarning {
    std::string message;
};

struct CommandError {
    std::string message;
};

using CommandResult = std::variant<
    CommandOk,
    CommandWarning,
    CommandError
>;

class SimulatorCore {
public:
    void addParameter(ParameterRecord parameter);
    void addEvent(Event event);
    void saveTextLog(const std::filesystem::path& path) const;

    CommandResult applyOperatorCommand(
    const std::string& name,
    const std::string& value
);

    std::optional<ParameterRecord> findParameter(
    const std::string& name) const;

    std::size_t countAlarmEvents() const;
    std::vector<Event> getAlarmEvents() const;
    std::vector<Event> getSortedEvents() const;

    std::map<EventType, std::size_t> getEventStatistics() const;

    std::vector<std::string> getParameterDescriptions() const;

private:
    std::vector<ParameterRecord> parameters_;
    std::vector<Event> events_;
};

std::string eventTypeName(EventType type);

} // namespace dispatcher
