#include "dispatcher/model/Robot.hpp"

#include <stdexcept>

namespace dispatcher {

namespace {

std::string toString(ElementStatus status) {
    switch (status) {
        case ElementStatus::Normal: return "Normal";
        case ElementStatus::Warning: return "Warning";
        case ElementStatus::Critical: return "Critical";
        case ElementStatus::Offline: return "Offline";
    }

    return "Unknown";
}

} // namespace

Robot::Robot(const std::string& id,
             int batteryLevel,
             ElementStatus status)
    : m_id(id),
      m_batteryLevel(batteryLevel),
      m_status(status) {
    if (m_id.empty()) {
        throw std::invalid_argument("Robot ID must not be empty");
    }

    if (m_batteryLevel < 0 || m_batteryLevel > 100) {
        throw std::invalid_argument(
            "Robot battery level must be between 0 and 100"
        );
    }
}

std::string Robot::id() const {
    return m_id;
}

std::string Robot::kind() const {
    return "Robot";
}

std::string Robot::status() const {
    return toString(m_status);
}

std::string Robot::summary() const {
    return "Robot " + m_id
        + ": battery=" + std::to_string(m_batteryLevel)
        + "% [" + status() + "]";
}

VisualDescriptor Robot::toVisualDescriptor() const {
    std::string color = "green";
    int priority = 1;

    if (m_status == ElementStatus::Warning) {
        color = "yellow";
        priority = 2;
    } else if (m_status == ElementStatus::Critical) {
        color = "red";
        priority = 5;
    } else if (m_status == ElementStatus::Offline) {
        color = "gray";
        priority = 3;
    }

    return VisualDescriptor{
        m_id,
        "robots",
        "Robot " + m_id
            + ", battery=" + std::to_string(m_batteryLevel) + "%",
        color,
        priority
    };
}

} // namespace dispatcher