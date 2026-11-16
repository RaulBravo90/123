#include "dispatcher/model/SafetyZone.hpp"
#include <stdexcept>

namespace dispatcher {

namespace {

std::string toString(SafetyZoneState state) {
    switch (state) {
        case SafetyZoneState::Clear:
            return "Clear";
        case SafetyZoneState::Occupied:
            return "Occupied";
        case SafetyZoneState::Restricted:
            return "Restricted";
    }

    return "Unknown";
}

} // namespace

SafetyZone::SafetyZone(const std::string& id, SafetyZoneState state)
    : m_id(id), m_state(state) {
    if (m_id.empty()) {
        throw std::invalid_argument(
            "Safety zone ID must not be empty"
        );
    }
}

std::string SafetyZone::id() const {
    return m_id;
}

std::string SafetyZone::kind() const {
    return "SafetyZone";
}

std::string SafetyZone::status() const {
    return toString(m_state);
}

std::string SafetyZone::summary() const {
    return "Safety zone " + m_id + ": " + toString(m_state);
}

VisualDescriptor SafetyZone::toVisualDescriptor() const {
    std::string color = "green";
    int priority = 1;

    if (m_state == SafetyZoneState::Occupied) {
        color = "yellow";
        priority = 2;
    } else if (m_state == SafetyZoneState::Restricted) {
        color = "red";
        priority = 5;
    }

    return VisualDescriptor{
        m_id,
        "zone_map",
        "Safety zone: " + toString(m_state),
        color,
        priority
    };
}

} // namespace dispatcher