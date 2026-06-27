#pragma once
#include <QWidget>
#include <QColor>

class QLabel;
class QPushButton;

class ColorSlot : public QWidget {
    Q_OBJECT
public:
    explicit ColorSlot(const QString& label, QWidget* parent = nullptr);
    [[nodiscard]] QColor color() const { return m_color; }

public slots:
    void setColor(QColor c);

signals:
    void colorChanged(QColor color);
    void pickRequested();

private slots:
    void copyHex();

private:
    void updateDisplay();

    QColor       m_color;
    QLabel*      m_swatchLabel;
    QLabel*      m_hexLabel;
    QLabel*      m_rgbLabel;
    QLabel*      m_hslLabel;
    QPushButton* m_pickButton;
    QPushButton* m_copyButton;
};
