#include "dispatcher/model/Alarm.hpp"

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

Alarm::Alarm(const std::string& id,
             const std::string& description,
             ElementStatus status)
    : m_id(id),
      m_description(description),
      m_status(status) {
    if (m_id.empty()) {
        throw std::invalid_argument("Alarm ID must not be empty");
    }

    if (m_description.empty()) {
        throw std::invalid_argument(
            "Alarm description must not be empty"
        );
    }
}

std::string Alarm::id() const {
    return m_id;
}

std::string Alarm::kind() const {
    return "Alarm";
}

std::string Alarm::status() const {
    return toString(m_status);
}

std::string Alarm::summary() const {
    return "Alarm " + m_id
        + ": " + m_description
        + " [" + status() + "]";
}

VisualDescriptor Alarm::toVisualDescriptor() const {
    std::string color = "green";
    int priority = 1;

    if (m_status == ElementStatus::Warning) {
        color = "yellow";
        priority = 3;
    } else if (m_status == ElementStatus::Critical) {
        color = "red";
        priority = 5;
    } else if (m_status == ElementStatus::Offline) {
        color = "gray";
        priority = 3;
    }

    return VisualDescriptor{
        m_id,
        "alarms",
        m_description,
        color,
        priority
    };
}

} // namespace dispatcher