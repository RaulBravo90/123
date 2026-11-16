#include "dispatcher/core/check_result.hpp"
#include "dispatcher/core/parameter.hpp"
#include "dispatcher/core/range.hpp"
#include "dispatcher/core/rules.hpp"

#include <cassert>
#include <stdexcept>

using namespace dispatcher;

int main()
{
    // Проверка диапазона целых чисел
    Range<int> robotCountRange(1, 10);

    assert(robotCountRange.contains(1));
    assert(robotCountRange.contains(5));
    assert(robotCountRange.contains(10));
    assert(!robotCountRange.contains(0));
    assert(!robotCountRange.contains(11));

    // Проверка запрета неправильного диапазона
    bool exceptionThrown = false;

    try {
        Range<int> invalidRange(10, 1);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    // Проверка параметра и правила для double
    Parameter<double> battery(
        "battery_level",
        "Battery level",
        85.0,
        "%"
    );

    RangeRule<double> batteryRule(
        battery,
        Range<double>(20.0, 100.0),
        "Battery level is outside the normal range"
    );

    CheckResult result = batteryRule.check();

    assert(result.parameterId == "battery_level");
    assert(result.severity == Severity::Ok);

    // Меняем значение и проверяем предупреждение
    battery.setValue(15.0);

    result = batteryRule.check();

    assert(result.parameterId == "battery_level");
    assert(result.severity == Severity::Warning);
    assert(result.message == "Battery level is outside the normal range");

    return 0;
}