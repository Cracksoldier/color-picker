#pragma once
#include <QWidget>
#include <QPixmap>
#include <QColor>

class PickerOverlay : public QWidget {
    Q_OBJECT
public:
    explicit PickerOverlay(QWidget* parent = nullptr);
    void activate();

signals:
    void colorPicked(QColor color);
    void pickCanceled();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void finish();

    QPixmap m_screenshot;
    QColor  m_hoverColor;
};
