#include "ColorSlot.h"
#include "ColorMath.h"
#include "ColorWheelDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGuiApplication>
#include <QClipboard>
#include <QIcon>
#include <QPainter>
#include <oclero/qlementine/icons/Icons16.hpp>

namespace {
QIcon makeIcon(const char* path) {
    const int sz = 16, margin = 4;
    QPixmap px(sz + margin, sz);
    px.fill(Qt::transparent);
    QPainter p(&px);
    QIcon(path).paint(&p, 0, 0, sz, sz);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(0, 0, sz, sz, Qt::white);
    return QIcon(px);
}
}

ColorSlot::ColorSlot(const QString& label, QWidget* parent)
    : QWidget(parent)
    , m_color(Qt::transparent)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(4);

    auto* title = new QLabel(label, this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-weight: bold; font-size: 13px; color: #cdd6f4;");
    root->addWidget(title);

    m_swatchLabel = new QLabel(this);
    m_swatchLabel->setFixedSize(64, 64);
    m_swatchLabel->setObjectName("swatch");
    m_swatchLabel->setStyleSheet("background-color: #313244; border-radius: 8px; border: 2px solid #45475a;");
    m_swatchLabel->setAlignment(Qt::AlignCenter);
    root->addWidget(m_swatchLabel, 0, Qt::AlignHCenter);

    m_hexLabel = new QLabel("—", this);
    m_hexLabel->setAlignment(Qt::AlignCenter);
    m_hexLabel->setStyleSheet("font-family: monospace; font-size: 12px;");
    root->addWidget(m_hexLabel);

    m_rgbLabel = new QLabel("—", this);
    m_rgbLabel->setAlignment(Qt::AlignCenter);
    m_rgbLabel->setStyleSheet("font-size: 10px; color: #a6adc8;");
    root->addWidget(m_rgbLabel);

    m_hslLabel = new QLabel("—", this);
    m_hslLabel->setAlignment(Qt::AlignCenter);
    m_hslLabel->setStyleSheet("font-size: 10px; color: #a6adc8;");
    root->addWidget(m_hslLabel);

    auto* btnRow = new QHBoxLayout;
    btnRow->setSpacing(4);

    m_pickButton = new QPushButton("Pick", this);
    m_pickButton->setToolTip("Pick a color from the screen");
    m_copyButton = new QPushButton("Copy", this);
    m_copyButton->setToolTip("Copy hex code to clipboard");
    m_copyButton->setEnabled(false);

    btnRow->addWidget(m_pickButton);
    btnRow->addWidget(m_copyButton);
    root->addLayout(btnRow);

    m_wheelButton = new QPushButton("Color Wheel", this);
    m_wheelButton->setToolTip("Choose or edit color with the color wheel");
    root->addWidget(m_wheelButton);

    namespace qi = oclero::qlementine::icons;
    const QSize iconSz(20, 16);
    m_pickButton->setIcon(makeIcon(qi::iconPath(qi::Icons16::Misc_Eyedropper)));
    m_pickButton->setIconSize(iconSz);
    m_copyButton->setIcon(makeIcon(qi::iconPath(qi::Icons16::Action_Copy)));
    m_copyButton->setIconSize(iconSz);
    m_wheelButton->setIcon(makeIcon(qi::iconPath(qi::Icons16::Misc_ColorSwatch)));
    m_wheelButton->setIconSize(iconSz);

    connect(m_pickButton,  &QPushButton::clicked, this, &ColorSlot::pickRequested);
    connect(m_wheelButton, &QPushButton::clicked, this, &ColorSlot::onWheelClicked);
    connect(m_copyButton,  &QPushButton::clicked, this, &ColorSlot::copyHex);
}

void ColorSlot::setColor(QColor c) {
    m_color = c;
    updateDisplay();
    emit colorChanged(c);
}

void ColorSlot::updateDisplay() {
    if (!m_color.isValid() || m_color == Qt::transparent) {
        m_swatchLabel->setStyleSheet("background-color: #313244; border-radius: 8px; border: 2px solid #45475a;");
        m_hexLabel->setText("—");
        m_rgbLabel->setText("—");
        m_hslLabel->setText("—");
        m_copyButton->setEnabled(false);
        return;
    }
    m_swatchLabel->setStyleSheet(QString(
        "background-color: %1; border-radius: 8px; border: 2px solid #45475a;"
    ).arg(ColorMath::toHex(m_color)));
    m_hexLabel->setText(ColorMath::toHex(m_color));
    m_rgbLabel->setText(ColorMath::toRgbStr(m_color));
    m_hslLabel->setText(ColorMath::toHslStr(m_color));
    m_copyButton->setEnabled(true);
}

void ColorSlot::copyHex() {
    QGuiApplication::clipboard()->setText(ColorMath::toHex(m_color));
}

void ColorSlot::onWheelClicked() {
    QColor initial = (m_color.isValid() && m_color != Qt::transparent)
                     ? m_color : QColor::fromHsv(0, 255, 255);
    ColorWheelDialog dlg(initial, this);
    if (dlg.exec() == QDialog::Accepted)
        setColor(dlg.selectedColor());
}
