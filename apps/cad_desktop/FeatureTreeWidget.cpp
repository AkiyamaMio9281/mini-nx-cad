#include "FeatureTreeWidget.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QVariant>

#include <utility>

namespace mnx {

namespace {

QString typeLabel(FeatureType type) {
    switch(type) {
    case FeatureType::Sketch:
        return QStringLiteral("Sketch");
    case FeatureType::Extrude:
        return QStringLiteral("Extrude");
    case FeatureType::Revolve:
        return QStringLiteral("Revolve");
    }
    return QStringLiteral("Feature");
}

} // namespace

FeatureTreeWidget::FeatureTreeWidget(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    tree_ = new QTreeWidget(this);
    tree_->setHeaderLabels({tr("Feature"), tr("Type"), tr("State")});
    layout->addWidget(tree_);

    connect(tree_, &QTreeWidget::itemSelectionChanged, this, [this]() {
        const QList<QTreeWidgetItem*> selectedItems = tree_->selectedItems();
        if(selectedItems.isEmpty() || !selectionCallback_) {
            return;
        }
        const auto idValue = selectedItems.front()->data(0, Qt::UserRole).toULongLong();
        selectionCallback_(FeatureId(static_cast<FeatureId::ValueType>(idValue)));
    });
}

void FeatureTreeWidget::setFeatureGraph(const FeatureGraph* graph) {
    graph_ = graph;
    refresh();
}

void FeatureTreeWidget::refresh() {
    tree_->clear();
    if(graph_ == nullptr) {
        return;
    }
    for(const Feature* feature : graph_->features()) {
        if(feature != nullptr) {
            tree_->addTopLevelItem(makeItem(*feature));
        }
    }
    tree_->expandAll();
    for(int column = 0; column < tree_->columnCount(); ++column) {
        tree_->resizeColumnToContents(column);
    }
}

void FeatureTreeWidget::setSelectionCallback(std::function<void(FeatureId)> callback) {
    selectionCallback_ = std::move(callback);
}

QTreeWidgetItem* FeatureTreeWidget::makeItem(const Feature& feature) const {
    auto* item = new QTreeWidgetItem();
    item->setText(0, QString::fromStdString(feature.name()));
    item->setText(1, typeLabel(feature.type()));
    item->setText(2, graph_->isDirty(feature.id()) ? tr("Dirty") : tr("Clean"));
    item->setData(0, Qt::UserRole, QVariant::fromValue<qulonglong>(feature.id().value()));
    return item;
}

} // namespace mnx
