#include "MateEditorPanel.h"

#include <QFormLayout>
#include <QLabel>

namespace mnx {

MateEditorPanel::MateEditorPanel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QFormLayout(this);
    layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    selectedComponentValue_ = new QLabel(tr("No component selected"), this);
    componentCountValue_ = new QLabel(QStringLiteral("0"), this);
    mateCountValue_ = new QLabel(QStringLiteral("0"), this);

    layout->addRow(tr("Selected"), selectedComponentValue_);
    layout->addRow(tr("Components"), componentCountValue_);
    layout->addRow(tr("Mates"), mateCountValue_);
}

void MateEditorPanel::setAssembly(const AssemblyDocument* assembly, ComponentId selectedComponentId) {
    if(assembly == nullptr) {
        selectedComponentValue_->setText(tr("No assembly"));
        componentCountValue_->setText(QStringLiteral("0"));
        mateCountValue_->setText(QStringLiteral("0"));
        return;
    }

    const ComponentInstance* selected = assembly->findComponent(selectedComponentId);
    selectedComponentValue_->setText(
        selected == nullptr ? tr("No component selected") : QString::fromStdString(selected->name())
    );
    componentCountValue_->setText(QString::number(assembly->components().size()));
    mateCountValue_->setText(QString::number(assembly->mates().size()));
}

} // namespace mnx
