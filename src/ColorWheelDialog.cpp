#include "ColorWheelDialog.h"
#include "ColorWheelWidget.h"
#include "ColorMath.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

ColorWheelDialog::ColorWheelDialog(QColor initial, QWidget* parent)
    : QDialog(parent)
    , m_color(initial.isValid() && initial != Qt::transparent
              ? initial.toHsv()
              : QColor::fromHsv(0, 255, 255))
{
    setWindowTitle("Color Wheel");
    setModal(true);

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(12, 12, 12, 12);
    root->setSpacing(8);

    m_wheel = new ColorWheelWidget(this);
    m_wheel->setColor(m_color);
    root->addWidget(m_wheel, 0, Qt::AlignHCenter);

    // Hex input row
    auto* hexRow = new QHBoxLayout;
    hexRow->setSpacing(6);

    m_swatch = new QLabel(this);
    m_swatch->setFixedSize(32, 32);

    m_hexEdit = new QLineEdit(this);
    m_hexEdit->setMaxLength(7);
    m_hexEdit->setPlaceholderText("#RRGGBB");
    m_hexEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("^#[0-9A-Fa-f]{0,6}$"), m_hexEdit));
    m_hexEdit->setStyleSheet(
        "QLineEdit { background-color: #313244; border: 1px solid #45475a;"
        " border-radius: 6px; padding: 4px 8px; font-family: monospace; }");

    hexRow->addWidget(m_swatch);
    hexRow->addWidget(m_hexEdit);
    root->addLayout(hexRow);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    root->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_wheel,   &ColorWheelWidget::colorChanged,
            this, &ColorWheelDialog::onWheelColorChanged);
    connect(m_hexEdit, &QLineEdit::textEdited,
            this, &ColorWheelDialog::onHexEdited);

    updateSwatch();
    m_hexEdit->setText(ColorMath::toHex(m_color));
}

void ColorWheelDialog::onWheelColorChanged(QColor c) {
    m_color = c;
    updateSwatch();
    m_hexEdit->setText(ColorMath::toHex(m_color));
}

void ColorWheelDialog::onHexEdited() {
    QColor c(m_hexEdit->text());
    if (!c.isValid()) return;
    m_color = c;
    updateSwatch();
    m_wheel->setColor(m_color);
}

void ColorWheelDialog::updateSwatch() {
    m_swatch->setStyleSheet(QString(
        "background-color: %1; border-radius: 6px; border: 1px solid #45475a;")
        .arg(ColorMath::toHex(m_color)));
}
