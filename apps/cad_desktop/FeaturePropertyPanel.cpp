#include "FeaturePropertyPanel.h"

#include <QFormLayout>
#include <QLabel>

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

FeaturePropertyPanel::FeaturePropertyPanel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QFormLayout(this);
    layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    nameValue_ = new QLabel(tr("No feature selected"), this);
    typeValue_ = new QLabel(QString(), this);
    idValue_ = new QLabel(QString(), this);
    stateValue_ = new QLabel(QString(), this);

    layout->addRow(tr("Name"), nameValue_);
    layout->addRow(tr("Type"), typeValue_);
    layout->addRow(tr("ID"), idValue_);
    layout->addRow(tr("State"), stateValue_);
}

void FeaturePropertyPanel::setFeature(const Feature* feature, bool dirty) {
    if(feature == nullptr) {
        nameValue_->setText(tr("No feature selected"));
        typeValue_->clear();
        idValue_->clear();
        stateValue_->clear();
        return;
    }

    nameValue_->setText(QString::fromStdString(feature->name()));
    typeValue_->setText(typeLabel(feature->type()));
    idValue_->setText(QString::number(feature->id().value()));
    stateValue_->setText(dirty ? tr("Dirty") : tr("Clean"));
}

} // namespace mnx
