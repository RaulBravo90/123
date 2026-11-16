#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"

#include <string>

namespace dispatcher {

class ARRobotHint : public ISystemElement,
                    public IVisualDescriptorProvider {
private:
    std::string m_id;
    std::string m_text;
    std::string m_targetId;
    ElementStatus m_status;

public:
    ARRobotHint(const std::string& id,
                const std::string& text,
                const std::string& targetId,
                ElementStatus status = ElementStatus::Normal);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;
};

} // namespace dispatcher