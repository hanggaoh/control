#include "basic_syntax.hpp"
#include <cstdlib>
#include <iostream>

using namespace interview::foundations::syntax_practice;

int main() {
    int failures = 0;
    auto report = [&](bool enabled, bool passed, const char* group) {
        std::cout << (enabled ? (passed ? "PASS " : "FAIL ") : "SKIPPED ") << group << '\n';
        if (enabled && !passed) ++failures;
    };
    report(deduction_complete, std::is_same_v<borrowed_type, const int&>, "deduction");
    int number = 4;
    if (reference_complete) increment(number);
    report(reference_complete, number == 5, "reference");
    report(functions_complete,
           std::is_same_v<decltype(std::declval<reading&>().get()), int&> &&
           std::is_same_v<decltype(std::declval<const reading&>().get()), const int&>, "functions");
    device_name name{std::string{"sensor"}};
    report(classes_complete, name.get() == "sensor" &&
           !std::is_convertible_v<std::string, device_name>, "classes");
    std::string text{"pump"};
    const std::string fixed{"valve"};
    bool categories_ok = true;
    if (categories_complete) {
        categories_ok = forward_inspect(text) == 'A' &&
            forward_inspect(std::move(text)) == 'B' &&
            forward_inspect(fixed) == 'A' && forward_inspect(std::move(fixed)) == 'A' && text == "pump";
    }
    report(categories_complete, categories_ok, "categories");
    simulated_sensor simulated;
    const sensor& base = simulated;
    report(polymorphism_complete, base.read() == 42, "polymorphism");
    report(templates_complete, twice(3) == 6 && twice(-2) == -4, "templates");
    return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}
