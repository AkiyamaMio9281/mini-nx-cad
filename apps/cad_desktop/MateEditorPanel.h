#pragma once

#include "mnx/assembly/AssemblyDocument.h"

#include <QWidget>

class QLabel;

namespace mnx {

class MateEditorPanel final : public QWidget {
public:
    explicit MateEditorPanel(QWidget* parent = nullptr);

    void setAssembly(const AssemblyDocument* assembly, ComponentId selectedComponentId);

private:
    QLabel* selectedComponentValue_ = nullptr;
    QLabel* componentCountValue_ = nullptr;
    QLabel* mateCountValue_ = nullptr;
};

} // namespace mnx
