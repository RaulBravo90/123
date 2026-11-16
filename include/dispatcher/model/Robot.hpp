#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"

#include <string>

namespace dispatcher {

class Robot : public ISystemElement,
              public IVisualDescriptorProvider {
private:
    std::string m_id;
    int m_batteryLevel;
    ElementStatus m_status;

public:
    Robot(const std::string& id,
          int batteryLevel,
          ElementStatus status);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;
};

} // namespace dispatcher