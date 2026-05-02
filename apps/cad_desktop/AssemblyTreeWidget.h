#pragma once

#include "mnx/assembly/AssemblyDocument.h"
#include "mnx/core/ids/ComponentId.h"

#include <QWidget>

#include <functional>

class QTreeWidget;
class QTreeWidgetItem;

namespace mnx {

class AssemblyTreeWidget final : public QWidget {
public:
    explicit AssemblyTreeWidget(QWidget* parent = nullptr);

    void setAssemblyDocument(const AssemblyDocument* assembly);
    void refresh();
    void setSelectionCallback(std::function<void(ComponentId)> callback);

private:
    [[nodiscard]] QTreeWidgetItem* makeItem(const ComponentInstance& component) const;

    const AssemblyDocument* assembly_ = nullptr;
    QTreeWidget* tree_ = nullptr;
    std::function<void(ComponentId)> selectionCallback_;
};

} // namespace mnx
