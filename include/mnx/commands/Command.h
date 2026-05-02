#pragma once

#include "mnx/core/utils/Status.h"

#include <string_view>

namespace mnx {

class Command {
public:
    virtual ~Command() = default;

    [[nodiscard]] virtual std::string_view label() const = 0;
    [[nodiscard]] virtual Status execute() = 0;
    [[nodiscard]] virtual Status undo() = 0;
};

} // namespace mnx
