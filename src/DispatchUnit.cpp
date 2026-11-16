#include "dispatcher/DispatchUnit.hpp"
#include <stdexcept>

namespace dispatcher {

// Конструктор: инициализация и проверка инвариантов (Страница 10, Шаг 6)
DispatchUnit::DispatchUnit(int id, std::string name, std::string zone)
    : id_(id), name_(std::move(name)), zone_(std::move(zone)), status_(UnitStatus::Available) 
{
    // Обязательные проверки инвариантов из методички (Страница 10)
    if (id_ <= 0) {
        throw std::invalid_argument("Unit ID must be positive");
    }
    if (name_.empty()) {
        throw std::invalid_argument("Unit name cannot be empty");
    }
    if (zone_.empty()) {
        throw std::invalid_argument("Unit zone cannot be empty");
    }
}

// Геттеры (все методы чтения помечены как const)
int DispatchUnit::id() const { return id_; }
const std::string& DispatchUnit::name() const { return name_; }
const std::string& DispatchUnit::zone() const { return zone_; }
UnitStatus DispatchUnit::status() const { return status_; }
std::optional<int> DispatchUnit::assignedIncidentId() const { return assignedIncidentId_; }

// Назначение юнита на инцидент (Страница 10, Шаг 6)
bool DispatchUnit::assignToIncident(int incidentId) {
    if (status_ == UnitStatus::Available) {
        assignedIncidentId_ = incidentId;
        status_ = UnitStatus::Busy; // Меняем статус на "Занят"
        return true;
    }
    return false;
}

// Освобождение юнита (Страница 10, Шаг 6)
void DispatchUnit::release() {
    assignedIncidentId_ = std::nullopt;
    status_ = UnitStatus::Available; // Возвращаем в "Доступен"
}

// Перевод на техобслуживание (Страница 10, Шаг 6)
void DispatchUnit::setMaintenance() {
    release(); // Сбрасываем назначения, если они были
    status_ = UnitStatus::Maintenance;
}

// Отключение юнита из сети (Страница 10, Шаг 6)
void DispatchUnit::setOffline() {
    release(); // Сбрасываем назначения, если они были
    status_ = UnitStatus::Offline;
}

} // namespace dispatcher
