#pragma once

#include "dispatcher/visual/VisualDescriptor.hpp"

#include <vector>

class ConsoleVisualRenderer {
public:
    void render(const std::vector<VisualDescriptor>& items) const;
};