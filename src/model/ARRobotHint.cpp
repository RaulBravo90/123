#include "dispatcher/model/ARRobotHint.hpp"

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

ARRobotHint::ARRobotHint(const std::string& id,
                         const std::string& text,
                         const std::string& targetId,
                         ElementStatus status)
    : m_id(id),
      m_text(text),
      m_targetId(targetId),
      m_status(status) {
    if (m_id.empty()) {
        throw std::invalid_argument("AR hint ID must not be empty");
    }

    if (m_text.empty()) {
        throw std::invalid_argument("AR hint text must not be empty");
    }

    if (m_targetId.empty()) {
        throw std::invalid_argument("AR hint target ID must not be empty");
    }
}

std::string ARRobotHint::id() const {
    return m_id;
}

std::string ARRobotHint::kind() const {
    return "ARRobotHint";
}

std::string ARRobotHint::status() const {
    return toString(m_status);
}

std::string ARRobotHint::summary() const {
    return "AR robot hint " + m_id
        + ": " + m_text
        + " -> target: " + m_targetId
        + " [" + status() + "]";
}

VisualDescriptor ARRobotHint::toVisualDescriptor() const {
    std::string color = "blue";
    int priority = 3;

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
        "ar_robot_marker",
        m_text,
        color,
        priority
    };
}

} // namespace dispatcher