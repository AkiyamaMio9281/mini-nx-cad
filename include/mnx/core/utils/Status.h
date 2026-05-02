#pragma once

#include <string>
#include <utility>

namespace mnx {

enum class StatusCode {
    Ok,
    InvalidArgument,
    NotFound,
    FailedPrecondition,
    InternalError,
};

class Status {
public:
    Status() = default;
    Status(StatusCode code, std::string message) : code_(code), message_(std::move(message)) {}

    [[nodiscard]] static Status ok() {
        return {};
    }

    [[nodiscard]] static Status invalidArgument(std::string message) {
        return {StatusCode::InvalidArgument, std::move(message)};
    }

    [[nodiscard]] static Status notFound(std::string message) {
        return {StatusCode::NotFound, std::move(message)};
    }

    [[nodiscard]] static Status failedPrecondition(std::string message) {
        return {StatusCode::FailedPrecondition, std::move(message)};
    }

    [[nodiscard]] static Status internalError(std::string message) {
        return {StatusCode::InternalError, std::move(message)};
    }

    [[nodiscard]] bool isOk() const {
        return code_ == StatusCode::Ok;
    }

    [[nodiscard]] StatusCode code() const {
        return code_;
    }

    [[nodiscard]] const std::string& message() const {
        return message_;
    }

    explicit operator bool() const {
        return isOk();
    }

private:
    StatusCode code_ = StatusCode::Ok;
    std::string message_;
};

} // namespace mnx
