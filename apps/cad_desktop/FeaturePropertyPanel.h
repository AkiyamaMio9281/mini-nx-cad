#pragma once

#include "mnx/modeling/Feature.h"

#include <QWidget>

class QLabel;

namespace mnx {

class FeaturePropertyPanel final : public QWidget {
public:
    explicit FeaturePropertyPanel(QWidget* parent = nullptr);

    void setFeature(const Feature* feature, bool dirty);

private:
    QLabel* nameValue_ = nullptr;
    QLabel* typeValue_ = nullptr;
    QLabel* idValue_ = nullptr;
    QLabel* stateValue_ = nullptr;
};

} // namespace mnx
