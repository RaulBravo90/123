#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"

#include <string>

namespace dispatcher {

enum class SafetyZoneState {
    Clear,
    Occupied,
    Restricted
};

class SafetyZone : public ISystemElement,
                   public IVisualDescriptorProvider {
private:
    std::string m_id;
    SafetyZoneState m_state;

public:
    SafetyZone(const std::string& id, SafetyZoneState state);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;
};

} // namespace dispatcher