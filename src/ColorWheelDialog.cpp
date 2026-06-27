#include "ColorWheelDialog.h"
#include "ColorWheelWidget.h"
#include "ColorMath.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QIcon>
#include <QImage>
#include <QPainter>
#include <oclero/qlementine/icons/Icons16.hpp>

namespace {
QIcon makeIcon(const char* path) {
    const int sz = 16, margin = 4;
    QPixmap src(sz, sz);
    src.fill(Qt::transparent);
    {
        QPainter p(&src);
        QIcon(path).paint(&p, 0, 0, sz, sz);
    }
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < sz; ++y) {
        auto* row = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < sz; ++x)
            row[x] = qRgba(255, 255, 255, qAlpha(row[x]));
    }
    QPixmap px(sz + margin, sz);
    px.fill(Qt::transparent);
    QPainter p2(&px);
    p2.drawImage(0, 0, img);
    return QIcon(px);
}
}

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

    auto* btnRow = new QHBoxLayout;
    btnRow->setSpacing(6);
    auto* okBtn     = new QPushButton("OK", this);
    auto* cancelBtn = new QPushButton(tr("Cancel"), this);

    namespace qi = oclero::qlementine::icons;
    const QSize iconSz(20, 16);
    okBtn->setIcon(makeIcon(qi::iconPath(qi::Icons16::Shape_CheckTick)));
    okBtn->setIconSize(iconSz);
    cancelBtn->setIcon(makeIcon(qi::iconPath(qi::Icons16::Misc_Forbidden)));
    cancelBtn->setIconSize(iconSz);

    btnRow->addWidget(okBtn, 1);
    btnRow->addWidget(cancelBtn);
    root->addLayout(btnRow);

    connect(okBtn,     &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
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
