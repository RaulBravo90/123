#pragma once

#include <string>

struct VisualDescriptor {
    std::string elementId;
    std::string visualKind;
    std::string label;
    std::string color;
    int priority = 0;
};