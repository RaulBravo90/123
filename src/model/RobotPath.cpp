#include "dispatcher/model/RobotPath.hpp"

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

RobotPath::RobotPath(const std::string& id,
                     const std::string& from,
                     const std::string& to,
                     ElementStatus status)
    : m_id(id),
      m_from(from),
      m_to(to),
      m_status(status) {
    if (m_id.empty()) {
        throw std::invalid_argument("Robot path ID must not be empty");
    }

    if (m_from.empty() || m_to.empty()) {
        throw std::invalid_argument(
            "Robot path start and destination must not be empty"
        );
    }
}

std::string RobotPath::id() const {
    return m_id;
}

std::string RobotPath::kind() const {
    return "RobotPath";
}

std::string RobotPath::status() const {
    return toString(m_status);
}

std::string RobotPath::summary() const {
    return "Robot path " + m_id
        + ": " + m_from + " -> " + m_to
        + " [" + status() + "]";
}

VisualDescriptor RobotPath::toVisualDescriptor() const {
    std::string color = "blue";
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
        "paths",
        m_from + " -> " + m_to,
        color,
        priority
    };
}

} // namespace dispatcher