#pragma once

#include "mnx/core/utils/Status.h"

#include <optional>
#include <stdexcept>
#include <utility>

namespace mnx {

template <typename T>
class Result {
public:
    Result(const T& value) : value_(value), status_(Status::ok()) {}
    Result(T&& value) : value_(std::move(value)), status_(Status::ok()) {}
    Result(Status status) : status_(std::move(status)) {
        if(status_.isOk()) {
            throw std::invalid_argument("Result failure status must not be OK");
        }
    }

    [[nodiscard]] static Result failure(Status status) {
        return Result(std::move(status));
    }

    [[nodiscard]] bool isOk() const {
        return status_.isOk();
    }

    [[nodiscard]] const Status& status() const {
        return status_;
    }

    [[nodiscard]] const T& value() const {
        if(!value_.has_value()) {
            throw std::logic_error("Attempted to read value from failed Result");
        }
        return *value_;
    }

    [[nodiscard]] T& value() {
        if(!value_.has_value()) {
            throw std::logic_error("Attempted to read value from failed Result");
        }
        return *value_;
    }

    explicit operator bool() const {
        return isOk();
    }

private:
    std::optional<T> value_;
    Status status_;
};

} // namespace mnx
