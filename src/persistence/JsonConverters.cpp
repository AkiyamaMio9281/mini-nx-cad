#include "mnx/persistence/JsonConverters.h"

#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace mnx::persistence {

namespace {

const JsonValue::Array kEmptyArray;
const JsonValue::Object kEmptyObject;
const std::string kEmptyString;

[[nodiscard]] std::string escaped(const std::string& text) {
    std::string result;
    for(char ch : text) {
        if(ch == '"' || ch == '\\') {
            result.push_back('\\');
        }
        result.push_back(ch);
    }
    return result;
}

class Parser {
public:
    explicit Parser(const std::string& text) : text_(text) {}

    [[nodiscard]] Result<JsonValue> parse() {
        try {
            skipWhitespace();
            JsonValue value = parseValue();
            skipWhitespace();
            if(position_ != text_.size()) {
                return Result<JsonValue>::failure(Status::invalidArgument("Unexpected trailing JSON content"));
            }
            return value;
        } catch(const std::exception& error) {
            return Result<JsonValue>::failure(Status::invalidArgument(error.what()));
        }
    }

private:
    [[nodiscard]] JsonValue parseValue() {
        skipWhitespace();
        if(position_ >= text_.size()) {
            throw std::runtime_error("Unexpected end of JSON input");
        }
        const char ch = text_[position_];
        if(ch == '{') {
            return parseObject();
        }
        if(ch == '[') {
            return parseArray();
        }
        if(ch == '"') {
            return parseString();
        }
        if(ch == 't' || ch == 'f') {
            return parseBool();
        }
        if(ch == 'n') {
            expect("null");
            return JsonValue(nullptr);
        }
        return parseNumber();
    }

    [[nodiscard]] JsonValue parseObject() {
        consume('{');
        JsonValue::Object object;
        skipWhitespace();
        if(tryConsume('}')) {
            return object;
        }
        while(true) {
            std::string key = parseString();
            skipWhitespace();
            consume(':');
            object.emplace(std::move(key), parseValue());
            skipWhitespace();
            if(tryConsume('}')) {
                break;
            }
            consume(',');
        }
        return object;
    }

    [[nodiscard]] JsonValue parseArray() {
        consume('[');
        JsonValue::Array array;
        skipWhitespace();
        if(tryConsume(']')) {
            return array;
        }
        while(true) {
            array.push_back(parseValue());
            skipWhitespace();
            if(tryConsume(']')) {
                break;
            }
            consume(',');
        }
        return array;
    }

    [[nodiscard]] std::string parseString() {
        consume('"');
        std::string result;
        while(position_ < text_.size()) {
            const char ch = text_[position_++];
            if(ch == '"') {
                return result;
            }
            if(ch == '\\') {
                if(position_ >= text_.size()) {
                    throw std::runtime_error("Invalid JSON string escape");
                }
                result.push_back(text_[position_++]);
            } else {
                result.push_back(ch);
            }
        }
        throw std::runtime_error("Unterminated JSON string");
    }

    [[nodiscard]] JsonValue parseBool() {
        if(text_.compare(position_, 4, "true") == 0) {
            position_ += 4;
            return JsonValue(true);
        }
        expect("false");
        return JsonValue(false);
    }

    [[nodiscard]] JsonValue parseNumber() {
        const std::size_t start = position_;
        if(text_[position_] == '-') {
            ++position_;
        }
        bool hasDigit = false;
        while(position_ < text_.size() && std::isdigit(static_cast<unsigned char>(text_[position_]))) {
            ++position_;
            hasDigit = true;
        }
        if(position_ < text_.size() && text_[position_] == '.') {
            ++position_;
            while(position_ < text_.size() && std::isdigit(static_cast<unsigned char>(text_[position_]))) {
                ++position_;
                hasDigit = true;
            }
        }
        if(!hasDigit) {
            throw std::runtime_error("Invalid JSON number");
        }
        if(position_ < text_.size() && (text_[position_] == 'e' || text_[position_] == 'E')) {
            ++position_;
            if(position_ < text_.size() && (text_[position_] == '+' || text_[position_] == '-')) {
                ++position_;
            }
            bool hasExponentDigit = false;
            while(position_ < text_.size() && std::isdigit(static_cast<unsigned char>(text_[position_]))) {
                ++position_;
                hasExponentDigit = true;
            }
            if(!hasExponentDigit) {
                throw std::runtime_error("Invalid JSON number exponent");
            }
        }
        return JsonValue(std::stod(text_.substr(start, position_ - start)));
    }

    void skipWhitespace() {
        while(position_ < text_.size() && std::isspace(static_cast<unsigned char>(text_[position_]))) {
            ++position_;
        }
    }

    void consume(char expected) {
        skipWhitespace();
        if(position_ >= text_.size() || text_[position_] != expected) {
            throw std::runtime_error("Unexpected JSON token");
        }
        ++position_;
    }

    [[nodiscard]] bool tryConsume(char expected) {
        skipWhitespace();
        if(position_ < text_.size() && text_[position_] == expected) {
            ++position_;
            return true;
        }
        return false;
    }

    void expect(const char* token) {
        const std::string expected(token);
        if(text_.compare(position_, expected.size(), expected) != 0) {
            throw std::runtime_error("Unexpected JSON literal");
        }
        position_ += expected.size();
    }

    const std::string& text_;
    std::size_t position_ = 0;
};

} // namespace

JsonValue::JsonValue(std::nullptr_t) : value_(nullptr) {}
JsonValue::JsonValue(bool value) : value_(value) {}
JsonValue::JsonValue(double value) : value_(value) {}
JsonValue::JsonValue(int value) : value_(static_cast<double>(value)) {}
JsonValue::JsonValue(std::string value) : value_(std::move(value)) {}
JsonValue::JsonValue(const char* value) : value_(std::string(value)) {}
JsonValue::JsonValue(Array value) : value_(std::move(value)) {}
JsonValue::JsonValue(Object value) : value_(std::move(value)) {}

bool JsonValue::isNull() const { return std::holds_alternative<std::nullptr_t>(value_); }
bool JsonValue::isBool() const { return std::holds_alternative<bool>(value_); }
bool JsonValue::isNumber() const { return std::holds_alternative<double>(value_); }
bool JsonValue::isString() const { return std::holds_alternative<std::string>(value_); }
bool JsonValue::isArray() const { return std::holds_alternative<Array>(value_); }
bool JsonValue::isObject() const { return std::holds_alternative<Object>(value_); }

bool JsonValue::asBool(bool fallback) const {
    return isBool() ? std::get<bool>(value_) : fallback;
}

double JsonValue::asNumber(double fallback) const {
    return isNumber() ? std::get<double>(value_) : fallback;
}

const std::string& JsonValue::asString() const {
    return isString() ? std::get<std::string>(value_) : kEmptyString;
}

const JsonValue::Array& JsonValue::asArray() const {
    return isArray() ? std::get<Array>(value_) : kEmptyArray;
}

const JsonValue::Object& JsonValue::asObject() const {
    return isObject() ? std::get<Object>(value_) : kEmptyObject;
}

const JsonValue* JsonValue::find(const std::string& key) const {
    if(!isObject()) {
        return nullptr;
    }
    const auto& object = std::get<Object>(value_);
    const auto iterator = object.find(key);
    return iterator == object.end() ? nullptr : &iterator->second;
}

std::string JsonValue::dump(int indent) const {
    std::ostringstream stream;
    const std::string pad(static_cast<std::size_t>(indent), ' ');
    if(isNull()) {
        stream << "null";
    } else if(isBool()) {
        stream << (asBool() ? "true" : "false");
    } else if(isNumber()) {
        stream << std::setprecision(17) << asNumber();
    } else if(isString()) {
        stream << '"' << escaped(asString()) << '"';
    } else if(isArray()) {
        stream << "[";
        const auto& array = asArray();
        for(std::size_t index = 0; index < array.size(); ++index) {
            stream << (index == 0 ? "\n" : ",\n") << pad << "  " << array[index].dump(indent + 2);
        }
        stream << (array.empty() ? "" : "\n" + pad) << "]";
    } else {
        stream << "{";
        const auto& object = asObject();
        std::size_t index = 0;
        for(const auto& [key, value] : object) {
            stream << (index++ == 0 ? "\n" : ",\n") << pad << "  \"" << escaped(key) << "\": "
                   << value.dump(indent + 2);
        }
        stream << (object.empty() ? "" : "\n" + pad) << "}";
    }
    return stream.str();
}

Result<JsonValue> JsonValue::parse(const std::string& text) {
    return Parser(text).parse();
}

} // namespace mnx::persistence
