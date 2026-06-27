#pragma once
#include <QDialog>
#include <QColor>

class ColorWheelWidget;
class QLabel;
class QLineEdit;

class ColorWheelDialog : public QDialog {
    Q_OBJECT
public:
    explicit ColorWheelDialog(QColor initial, QWidget* parent = nullptr);
    [[nodiscard]] QColor selectedColor() const { return m_color; }

private slots:
    void onWheelColorChanged(QColor c);
    void onHexEdited();

private:
    void updateSwatch();

    QColor            m_color;
    ColorWheelWidget* m_wheel;
    QLabel*           m_swatch;
    QLineEdit*        m_hexEdit;
};
