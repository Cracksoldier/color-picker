#include "MainWindow.h"
#include "ColorSlot.h"
#include "ComparisonPanel.h"
#include "ColorPicker.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Color Picker");
    setFixedWidth(340);

    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* root = new QVBoxLayout(central);
    root->setContentsMargins(10, 10, 10, 10);
    root->setSpacing(8);

    // Slot row
    auto* slotRow = new QHBoxLayout;
    slotRow->setSpacing(0);

    m_slotA = new ColorSlot("Color A", this);
    m_slotB = new ColorSlot("Color B", this);
    slotRow->addWidget(m_slotA);

    auto* slotSep = new QFrame(this);
    slotSep->setFrameShape(QFrame::VLine);
    slotSep->setStyleSheet("color: #45475a;");
    slotRow->addWidget(slotSep);

    slotRow->addWidget(m_slotB);
    root->addLayout(slotRow);

    // Divider
    auto* divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("color: #45475a;");
    root->addWidget(divider);

    // Comparison panel
    m_comparison = new ComparisonPanel(this);
    root->addWidget(m_comparison);

    // Action buttons
    auto* actionRow = new QHBoxLayout;
    actionRow->setSpacing(6);
    auto* swapBtn  = new QPushButton("Swap A \xe2\x86\x94 B", this);
    auto* resetBtn = new QPushButton("Reset", this);
    actionRow->addWidget(swapBtn);
    actionRow->addWidget(resetBtn);
    root->addLayout(actionRow);

    m_picker = new ColorPicker(this);

    connect(m_slotA, &ColorSlot::pickRequested, this, &MainWindow::onPickRequestedA);
    connect(m_slotB, &ColorSlot::pickRequested, this, &MainWindow::onPickRequestedB);
    connect(m_picker, &ColorPicker::colorPicked,  this, &MainWindow::onColorPicked);
    connect(m_picker, &ColorPicker::pickCanceled, this, &MainWindow::onPickCanceled);
    connect(swapBtn,  &QPushButton::clicked, this, &MainWindow::swapColors);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::resetColors);

    connect(m_slotA, &ColorSlot::colorChanged, m_comparison, &ComparisonPanel::setColorA);
    connect(m_slotB, &ColorSlot::colorChanged, m_comparison, &ComparisonPanel::setColorB);

    adjustSize();
}

void MainWindow::onPickRequestedA() {
    if (m_pendingPick != PickTarget::None) return;
    m_pendingPick = PickTarget::SlotA;
    m_picker->startPick();
}

void MainWindow::onPickRequestedB() {
    if (m_pendingPick != PickTarget::None) return;
    m_pendingPick = PickTarget::SlotB;
    m_picker->startPick();
}

void MainWindow::onColorPicked(QColor color) {
    if (m_pendingPick == PickTarget::SlotA)
        m_slotA->setColor(color);
    else if (m_pendingPick == PickTarget::SlotB)
        m_slotB->setColor(color);
    m_pendingPick = PickTarget::None;
}

void MainWindow::onPickCanceled() {
    m_pendingPick = PickTarget::None;
}

void MainWindow::swapColors() {
    QColor a = m_slotA->color();
    QColor b = m_slotB->color();
    m_slotA->setColor(b);
    m_slotB->setColor(a);
}

void MainWindow::resetColors() {
    m_slotA->setColor(Qt::transparent);
    m_slotB->setColor(Qt::transparent);
}
