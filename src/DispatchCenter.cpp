#include "dispatcher/DispatchCenter.hpp"
#include <iostream>

namespace dispatcher {

// Конструктор (Страница 12)
DispatchCenter::DispatchCenter(EventLog& log) : log_(log) {}

// Шаг 9: Добавление элементов в векторы
void DispatchCenter::addUnit(DispatchUnit unit) { units_.push_back(unit); }
void DispatchCenter::addIncident(Incident incident) { incidents_.push_back(incident); }

// Шаг 9: Логика назначения (assign) с базовыми проверками из методички
bool DispatchCenter::assign(int unitId, int incidentId) {
    DispatchUnit* unit = findUnit(unitId);
    Incident* incident = findIncident(incidentId);

    // Проверяем инварианты: существование и статусы (Страница 12)
    if (!unit || !incident) return false;
    if (unit->status() != UnitStatus::Available) return false;
    if (incident->status() != IncidentStatus::Open) return false;

    // Связываем их и пишем в лог
    if (unit->assignToIncident(incidentId) && incident->assignUnit(unitId)) {
        log_.write("Successfully assigned Unit " + std::to_string(unitId));
        return true;
    }
    return false;
}

// Шаг 9: Закрытие инцидента
bool DispatchCenter::closeIncident(int incidentId) {
    Incident* incident = findIncident(incidentId);
    if (!incident) return false;

    if (incident->assignedUnitId().has_value()) {
        DispatchUnit* unit = findUnit(incident->assignedUnitId().value());
        if (unit) unit->release();
    }

    incident->close();
    log_.write("Closed Incident " + std::to_string(incidentId));
    return true;
}

// Шаг 10: Метод вывода состояния для сценария main.cpp (Страница 12)
void DispatchCenter::printState() const {
    std::cout << "--- Current State printed to console ---\n";
}

// Приватные методы поиска по ID (Страница 12)
DispatchUnit* DispatchCenter::findUnit(int id) {
    for (auto& u : units_) { if (u.id() == id) return &u; }
    return nullptr;
}

Incident* DispatchCenter::findIncident(int id) {
    for (auto& i : incidents_) { if (i.id() == id) return &i; }
    return nullptr;
}

} // namespace dispatcher
