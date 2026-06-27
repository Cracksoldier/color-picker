#pragma once
#include <QWidget>
#include <QColor>

class ColorWheelWidget : public QWidget {
    Q_OBJECT
public:
    explicit ColorWheelWidget(QWidget* parent = nullptr);
    [[nodiscard]] QColor color() const { return m_color; }

public slots:
    void setColor(QColor c);

signals:
    void colorChanged(QColor color);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    enum class DragRegion { None, Ring, Square };

    void updateFromRing(QPointF pos);
    void updateFromSquare(QPointF pos);
    [[nodiscard]] DragRegion regionAt(QPointF pos) const;
    [[nodiscard]] QRectF squareRect() const;

    QColor     m_color;
    int        m_lastHue  = 0;   // last explicitly chosen hue; survives achromatic transitions
    QPixmap    m_ringCache;      // pre-rendered hue ring (invariant, built once)
    DragRegion m_dragging = DragRegion::None;
};
