#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/VisualDescriptor.hpp"

#include <memory>
#include <string>
#include <vector>

namespace dispatcher {

class DispatcherModel {
private:
    std::vector<std::unique_ptr<ISystemElement>> elements;

public:
    void addElement(std::unique_ptr<ISystemElement> element);

    std::vector<std::string> collectSummaries() const;
    std::vector<VisualDescriptor> collectVisualDescriptors() const;
};

} // namespace dispatcher