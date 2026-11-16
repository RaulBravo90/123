#include "dispatcher/visual/ConsoleVisualRenderer.hpp"

#include <iostream>

void ConsoleVisualRenderer::render(const std::vector<VisualDescriptor>& items) const {
    for (const auto& item : items) {
        std::cout << "[" << item.visualKind << "] "
                  << item.label << " ("
                  << item.color << ", priority=" << item.priority
                  << ")" << std::endl;
    }
}