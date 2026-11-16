#include "dispatcher/model/DispatcherModel.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"

#include <utility>

namespace dispatcher {

void DispatcherModel::addElement(std::unique_ptr<ISystemElement> element) {
    elements.push_back(std::move(element));
}

std::vector<std::string> DispatcherModel::collectSummaries() const {
    std::vector<std::string> result;

    for (const auto& element : elements) {
        result.push_back(element->summary());
    }

    return result;
}

std::vector<VisualDescriptor> DispatcherModel::collectVisualDescriptors() const {
    std::vector<VisualDescriptor> result;

    for (const auto& element : elements) {
        const auto* provider =
            dynamic_cast<const IVisualDescriptorProvider*>(element.get());

        if (provider != nullptr) {
            result.push_back(provider->toVisualDescriptor());
        }
    }

    return result;
}

} // namespace dispatcher