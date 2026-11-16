#pragma once

#include "VisualDescriptor.hpp"

namespace dispatcher {

class IVisualDescriptorProvider {
public:
    virtual ~IVisualDescriptorProvider() = default;

    virtual VisualDescriptor toVisualDescriptor() const = 0;
};

} // namespace dispatcher