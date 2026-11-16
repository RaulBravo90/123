#pragma once
#include <filesystem>
#include <fstream>
#include <string_view>

namespace dispatcher {

// RAII-обертка над файлом журнала (Страница 4, Шаг 8)
class EventLog {
public:
    // Конструктор: открывает файл при создании объекта
    explicit EventLog(const std::filesystem::path& path);
    
    // Деструктор: автоматически закрывает файл при уничтожении объекта
    ~EventLog();

    // Метод записи сообщений в журнал
    void write(std::string_view message);

private:
    std::ofstream out_; // Поток вывода в файл журнала
};

} // namespace dispatcher
