#include "ComparisonPanel.h"
#include "ColorMath.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>

ComparisonPanel::ComparisonPanel(QWidget* parent)
    : QWidget(parent)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(6);

    auto* title = new QLabel("Comparison", this);
    title->setStyleSheet("font-weight: bold; font-size: 13px; color: #cdd6f4;");
    title->setAlignment(Qt::AlignCenter);
    root->addWidget(title);

    // Swatch row
    auto* swatchRow = new QHBoxLayout;
    swatchRow->setSpacing(8);

    auto makeSwatchBox = [&](QLabel*& swatch, QLabel*& hex, const QString& tag) {
        auto* col = new QVBoxLayout;
        col->setSpacing(3);
        auto* tagLabel = new QLabel(tag, this);
        tagLabel->setAlignment(Qt::AlignCenter);
        tagLabel->setStyleSheet("font-size: 10px; color: #a6adc8;");
        col->addWidget(tagLabel);
        swatch = new QLabel(this);
        swatch->setFixedSize(56, 56);
        swatch->setStyleSheet("background: #313244; border-radius: 8px; border: 2px solid #45475a;");
        col->addWidget(swatch, 0, Qt::AlignHCenter);
        hex = new QLabel("—", this);
        hex->setAlignment(Qt::AlignCenter);
        hex->setStyleSheet("font-family: monospace; font-size: 10px;");
        col->addWidget(hex);
        swatchRow->addLayout(col);
    };

    makeSwatchBox(m_swatchA, m_hexA, "A");

    auto* sep = new QFrame(this);
    sep->setFrameShape(QFrame::VLine);
    sep->setStyleSheet("color: #45475a;");
    swatchRow->addWidget(sep);

    makeSwatchBox(m_swatchB, m_hexB, "B");
    root->addLayout(swatchRow);

    // Divider
    auto* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #45475a;");
    root->addWidget(line);

    // Metrics grid
    auto* grid = new QGridLayout;
    grid->setSpacing(4);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 0);

    auto addRow = [&](int row, const QString& labelText, QLabel*& badge) {
        auto* lbl = new QLabel(labelText, this);
        lbl->setStyleSheet("font-size: 11px; color: #a6adc8;");
        grid->addWidget(lbl, row, 0);
        badge = new QLabel("—", this);
        badge->setAlignment(Qt::AlignCenter);
        badge->setFixedWidth(52);
        badge->setStyleSheet("font-size: 10px; color: #a6adc8; border-radius: 4px; padding: 1px 4px;");
        grid->addWidget(badge, row, 1);
    };

    auto* ratioRow = new QHBoxLayout;
    auto* ratioLbl = new QLabel("Contrast ratio:", this);
    ratioLbl->setStyleSheet("font-size: 11px; color: #a6adc8;");
    m_ratioLabel = new QLabel("—", this);
    m_ratioLabel->setStyleSheet("font-size: 11px; font-weight: bold; font-family: monospace;");
    ratioRow->addWidget(ratioLbl);
    ratioRow->addStretch();
    ratioRow->addWidget(m_ratioLabel);
    root->addLayout(ratioRow);
    root->addLayout(grid);

    addRow(0, "AA normal text (≥4.5):", m_aaNormalBadge);
    addRow(1, "AA large text  (≥3.0):", m_aaLargeBadge);
    addRow(2, "AAA normal text (≥7.0):", m_aaaNormalBadge);
    addRow(3, "AAA large text  (≥4.5):", m_aaaLargeBadge);
}

QString ComparisonPanel::badgeStyle(bool pass) {
    if (pass)
        return "font-size: 10px; font-weight: bold; color: white; background: #40a02b; border-radius: 4px; padding: 1px 6px;";
    return "font-size: 10px; font-weight: bold; color: white; background: #d20f39; border-radius: 4px; padding: 1px 6px;";
}

void ComparisonPanel::updateSwatch(QLabel* swatch, QLabel* hex, QColor c) {
    if (!c.isValid() || c == Qt::transparent) {
        swatch->setStyleSheet("background: #313244; border-radius: 8px; border: 2px solid #45475a;");
        hex->setText("—");
    } else {
        swatch->setStyleSheet(QString("background: %1; border-radius: 8px; border: 2px solid #45475a;")
            .arg(ColorMath::toHex(c)));
        hex->setText(ColorMath::toHex(c));
    }
}

void ComparisonPanel::updateMetrics() {
    bool bothValid = m_colorA.isValid() && m_colorA != Qt::transparent
                  && m_colorB.isValid() && m_colorB != Qt::transparent;
    if (!bothValid) {
        m_ratioLabel->setText("—");
        for (auto* badge : {m_aaNormalBadge, m_aaLargeBadge, m_aaaNormalBadge, m_aaaLargeBadge}) {
            badge->setText("—");
            badge->setStyleSheet("font-size: 10px; color: #a6adc8; border-radius: 4px; padding: 1px 4px;");
        }
        return;
    }

    double ratio = ColorMath::contrastRatio(m_colorA, m_colorB);
    m_ratioLabel->setText(QString("%1:1").arg(ratio, 0, 'f', 2));

    auto setBadge = [](QLabel* badge, bool pass) {
        badge->setText(pass ? "PASS" : "FAIL");
        badge->setStyleSheet(ComparisonPanel::badgeStyle(pass));
    };

    setBadge(m_aaNormalBadge,  ratio >= 4.5);
    setBadge(m_aaLargeBadge,   ratio >= 3.0);
    setBadge(m_aaaNormalBadge, ratio >= 7.0);
    setBadge(m_aaaLargeBadge,  ratio >= 4.5);
}

void ComparisonPanel::setColorA(QColor c) {
    m_colorA = c;
    updateSwatch(m_swatchA, m_hexA, c);
    updateMetrics();
}

void ComparisonPanel::setColorB(QColor c) {
    m_colorB = c;
    updateSwatch(m_swatchB, m_hexB, c);
    updateMetrics();
}
