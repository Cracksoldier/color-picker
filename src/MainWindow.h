#pragma once
#include <QMainWindow>
#include <QColor>

class ColorSlot;
class ComparisonPanel;
class ColorPicker;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onPickRequestedA();
    void onPickRequestedB();
    void onColorPicked(QColor color);
    void onPickCanceled();
    void swapColors();
    void resetColors();

private:
    enum class PickTarget { None, SlotA, SlotB };

    ColorSlot*      m_slotA;
    ColorSlot*      m_slotB;
    ComparisonPanel* m_comparison;
    ColorPicker*    m_picker;
    PickTarget      m_pendingPick = PickTarget::None;
};
