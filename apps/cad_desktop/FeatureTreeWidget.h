#pragma once

#include "mnx/core/ids/FeatureId.h"
#include "mnx/modeling/FeatureGraph.h"

#include <QWidget>

#include <functional>

class QTreeWidget;
class QTreeWidgetItem;

namespace mnx {

class FeatureTreeWidget final : public QWidget {
public:
    explicit FeatureTreeWidget(QWidget* parent = nullptr);

    void setFeatureGraph(const FeatureGraph* graph);
    void refresh();
    void setSelectionCallback(std::function<void(FeatureId)> callback);

private:
    [[nodiscard]] QTreeWidgetItem* makeItem(const Feature& feature) const;

    const FeatureGraph* graph_ = nullptr;
    QTreeWidget* tree_ = nullptr;
    std::function<void(FeatureId)> selectionCallback_;
};

} // namespace mnx
