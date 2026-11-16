#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"

#include <string>

namespace dispatcher {

class Alarm : public ISystemElement,
              public IVisualDescriptorProvider {
private:
    std::string m_id;
    std::string m_description;
    ElementStatus m_status;

public:
    Alarm(const std::string& id,
          const std::string& description,
          ElementStatus status);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;
};

} // namespace dispatcher