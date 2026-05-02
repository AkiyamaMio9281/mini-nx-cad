#pragma once

#include "mnx/core/utils/Result.h"

#include <map>
#include <string>
#include <variant>
#include <vector>

namespace mnx::persistence {

class JsonValue {
public:
    using Array = std::vector<JsonValue>;
    using Object = std::map<std::string, JsonValue>;

    JsonValue() = default;
    JsonValue(std::nullptr_t);
    JsonValue(bool value);
    JsonValue(double value);
    JsonValue(int value);
    JsonValue(std::string value);
    JsonValue(const char* value);
    JsonValue(Array value);
    JsonValue(Object value);

    [[nodiscard]] bool isNull() const;
    [[nodiscard]] bool isBool() const;
    [[nodiscard]] bool isNumber() const;
    [[nodiscard]] bool isString() const;
    [[nodiscard]] bool isArray() const;
    [[nodiscard]] bool isObject() const;

    [[nodiscard]] bool asBool(bool fallback = false) const;
    [[nodiscard]] double asNumber(double fallback = 0.0) const;
    [[nodiscard]] const std::string& asString() const;
    [[nodiscard]] const Array& asArray() const;
    [[nodiscard]] const Object& asObject() const;
    [[nodiscard]] const JsonValue* find(const std::string& key) const;

    [[nodiscard]] std::string dump(int indent = 0) const;
    [[nodiscard]] static Result<JsonValue> parse(const std::string& text);

private:
    std::variant<std::nullptr_t, bool, double, std::string, Array, Object> value_;
};

} // namespace mnx::persistence
