#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"

#include <string>

namespace dispatcher {

class RobotPath : public ISystemElement,
                  public IVisualDescriptorProvider {
private:
    std::string m_id;
    std::string m_from;
    std::string m_to;
    ElementStatus m_status;

public:
    RobotPath(const std::string& id,
              const std::string& from,
              const std::string& to,
              ElementStatus status);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;
};

} // namespace dispatcher