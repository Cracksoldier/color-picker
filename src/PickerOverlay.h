#pragma once
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QRect>

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
    void closeEvent(QCloseEvent* event) override;

private:
    void finish();

    QPixmap m_screenshot;
    QImage  m_image;       // cached toImage() result — computed once in activate()
    QRect   m_virtualGeom; // virtual desktop geometry used for overlay sizing and coordinate math
    QColor  m_hoverColor;
};
