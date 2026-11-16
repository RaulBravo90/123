#include "dispatcher/Incident.hpp"
#include <stdexcept>

namespace dispatcher {

// Конструктор: инициализация и проверка обязательных инвариантов (Страница 10, Шаг 7)
Incident::Incident(int id, std::string title, std::string zone, Severity severity, VisualMarker marker)
    : id_(id), title_(std::move(title)), zone_(std::move(zone)), 
      severity_(severity), status_(IncidentStatus::Open), marker_(marker)
{
    // Проверка инвариантов сущности (Аналогично Шагу 6 для DispatchUnit)
    if (id_ <= 0) {
        throw std::invalid_argument("Incident ID must be positive");
    }
    if (title_.empty()) {
        throw std::invalid_argument("Incident title cannot be empty");
    }
    if (zone_.empty()) {
        throw std::invalid_argument("Incident zone cannot be empty");
    }
}

// Геттеры (Методы чтения, помеченные как const)
int Incident::id() const { return id_; }
const std::string& Incident::title() const { return title_; }
const std::string& Incident::zone() const { return zone_; }
Severity Incident::severity() const { return severity_; }
IncidentStatus Incident::status() const { return status_; }
VisualMarker Incident::marker() const { return marker_; }
std::optional<int> Incident::assignedUnitId() const { return assignedUnitId_; }

// Назначение ремонтной единицы на инцидент (Страница 10, Шаг 7)
bool Incident::assignUnit(int unitId) {
    if (status_ == IncidentStatus::Open) {
        assignedUnitId_ = unitId;
        status_ = IncidentStatus::Assigned; // Переводим в статус "Назначен"
        return true;
    }
    return false;
}

// Закрытие инцидента (Страница 10, Шаг 7)
void Incident::close() {
    status_ = IncidentStatus::Closed; // Переводим в статус "Закрыт"
}

} // namespace dispatcher
