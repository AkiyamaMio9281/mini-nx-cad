#include "AssemblyTreeWidget.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QVariant>

#include <utility>

namespace mnx {

AssemblyTreeWidget::AssemblyTreeWidget(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    tree_ = new QTreeWidget(this);
    tree_->setHeaderLabels({tr("Component"), tr("Part"), tr("ID")});
    layout->addWidget(tree_);

    connect(tree_, &QTreeWidget::itemSelectionChanged, this, [this]() {
        const QList<QTreeWidgetItem*> selectedItems = tree_->selectedItems();
        if(selectedItems.isEmpty() || !selectionCallback_) {
            return;
        }
        const auto idValue = selectedItems.front()->data(0, Qt::UserRole).toULongLong();
        selectionCallback_(ComponentId(static_cast<ComponentId::ValueType>(idValue)));
    });
}

void AssemblyTreeWidget::setAssemblyDocument(const AssemblyDocument* assembly) {
    assembly_ = assembly;
    refresh();
}

void AssemblyTreeWidget::refresh() {
    tree_->clear();
    if(assembly_ == nullptr) {
        return;
    }

    for(const ComponentInstance& component : assembly_->components()) {
        if(!component.transformNode().parentId().has_value()) {
            tree_->addTopLevelItem(makeItem(component));
        }
    }

    for(const ComponentInstance& component : assembly_->components()) {
        if(!component.transformNode().parentId().has_value()) {
            continue;
        }
        const auto parentItems = tree_->findItems(
            QString::number(component.transformNode().parentId()->value()),
            Qt::MatchRecursive,
            2
        );
        if(parentItems.isEmpty()) {
            tree_->addTopLevelItem(makeItem(component));
        } else {
            parentItems.front()->addChild(makeItem(component));
        }
    }

    tree_->expandAll();
    for(int column = 0; column < tree_->columnCount(); ++column) {
        tree_->resizeColumnToContents(column);
    }
}

void AssemblyTreeWidget::setSelectionCallback(std::function<void(ComponentId)> callback) {
    selectionCallback_ = std::move(callback);
}

QTreeWidgetItem* AssemblyTreeWidget::makeItem(const ComponentInstance& component) const {
    auto* item = new QTreeWidgetItem();
    item->setText(0, QString::fromStdString(component.name()));
    item->setText(1, QString::fromStdString(component.sourceDocumentName()));
    item->setText(2, QString::number(component.id().value()));
    item->setData(0, Qt::UserRole, QVariant::fromValue<qulonglong>(component.id().value()));
    return item;
}

} // namespace mnx
