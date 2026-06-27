#include "ColorWheelWidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QConicalGradient>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QtMath>

// Geometry constants (widget is 240×240, center at 120,120)
static constexpr qreal kCx     = 120.0;
static constexpr qreal kCy     = 120.0;
static constexpr qreal kOuterR = 110.0; // outer edge of hue ring
static constexpr qreal kRingW  = 18.0;  // ring width
static constexpr qreal kInnerR = 88.0;  // kOuterR - kRingW - 4px gap
static constexpr qreal kHalfSq = 62.0;  // kInnerR / sqrt(2), rounded down

ColorWheelWidget::ColorWheelWidget(QWidget* parent)
    : QWidget(parent)
    , m_color(QColor::fromHsv(0, 255, 255))
{
    setFixedSize(240, 240);
}

void ColorWheelWidget::setColor(QColor c) {
    m_color = c.toHsv();
    update();
}

QRectF ColorWheelWidget::squareRect() const {
    return {kCx - kHalfSq, kCy - kHalfSq, kHalfSq * 2, kHalfSq * 2};
}

ColorWheelWidget::DragRegion ColorWheelWidget::regionAt(QPointF pos) const {
    qreal dx = pos.x() - kCx;
    qreal dy = pos.y() - kCy;
    qreal dist2 = dx * dx + dy * dy;

    if (dist2 > kOuterR * kOuterR)
        return DragRegion::None;

    // Between kInnerR and kOuterR: ring (includes 4px gap)
    if (dist2 > kInnerR * kInnerR)
        return DragRegion::Ring;

    // Inside inner circle
    if (squareRect().contains(pos))
        return DragRegion::Square;

    // Corner area between square and inner circle: treat as ring
    return DragRegion::Ring;
}

void ColorWheelWidget::updateFromRing(QPointF pos) {
    qreal dx = pos.x() - kCx;
    qreal dy = pos.y() - kCy;
    // atan2 gives CW angle on screen; gradient sweeps CCW, so hue = 360 - cwAngle
    qreal cwDeg = qRadiansToDegrees(qAtan2(dy, dx));
    if (cwDeg < 0) cwDeg += 360.0;
    int hue = qRound(360.0 - cwDeg) % 360;
    int sat = qMax(0, m_color.hsvSaturation());
    int val = m_color.value();
    m_color.setHsv(hue, sat, val);
    update();
    emit colorChanged(m_color);
}

void ColorWheelWidget::updateFromSquare(QPointF pos) {
    QRectF sq = squareRect();
    int sat = qRound(qBound(0.0, (pos.x() - sq.left()) / sq.width(),  1.0) * 255.0);
    int val = qRound(qBound(0.0, 1.0 - (pos.y() - sq.top()) / sq.height(), 1.0) * 255.0);
    int hue = qMax(0, m_color.hsvHue());
    m_color.setHsv(hue, sat, val);
    update();
    emit colorChanged(m_color);
}

void ColorWheelWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button() != Qt::LeftButton) return;
    m_dragging = regionAt(e->position());
    if (m_dragging == DragRegion::Ring)   updateFromRing(e->position());
    if (m_dragging == DragRegion::Square) updateFromSquare(e->position());
}

void ColorWheelWidget::mouseMoveEvent(QMouseEvent* e) {
    if (!(e->buttons() & Qt::LeftButton)) return;
    if (m_dragging == DragRegion::Ring)   updateFromRing(e->position());
    if (m_dragging == DragRegion::Square) updateFromSquare(e->position());
}

void ColorWheelWidget::mouseReleaseEvent(QMouseEvent*) {
    m_dragging = DragRegion::None;
}

void ColorWheelWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QPointF center(kCx, kCy);
    int hue = qMax(0, m_color.hsvHue());
    int sat = qMax(0, m_color.hsvSaturation());
    int val = m_color.value();

    // --- Hue ring ---
    // QConicalGradient sweeps CCW on screen: east(0°)→north(90°)→west(180°)→south(270°)
    QConicalGradient hueGrad(center, 0.0);
    for (int i = 0; i <= 360; i++)
        hueGrad.setColorAt(i / 360.0, QColor::fromHsv(i % 360, 255, 255));

    QPainterPath outerCircle, innerCircle;
    outerCircle.addEllipse(center, kOuterR, kOuterR);
    innerCircle.addEllipse(center, kOuterR - kRingW, kOuterR - kRingW);
    p.fillPath(outerCircle.subtracted(innerCircle), hueGrad);

    // --- SV square ---
    QRectF sq = squareRect();

    // Horizontal: left=white (S=0,V=255) → right=full hue (S=255,V=255)
    QLinearGradient hGrad(sq.left(), 0, sq.right(), 0);
    hGrad.setColorAt(0.0, QColor::fromHsv(hue, 0, 255));
    hGrad.setColorAt(1.0, QColor::fromHsv(hue, 255, 255));
    p.fillRect(sq, hGrad);

    // Vertical overlay: top=transparent → bottom=black
    QLinearGradient vGrad(0, sq.top(), 0, sq.bottom());
    vGrad.setColorAt(0.0, QColor(0, 0, 0, 0));
    vGrad.setColorAt(1.0, QColor(0, 0, 0, 255));
    p.fillRect(sq, vGrad);

    // --- Ring indicator ---
    // Indicator is at screen CW angle = (360 - hue), using cos/sin of that:
    // cos(360-h) = cos(h), sin(360-h) = -sin(h)
    qreal hueRad  = qDegreesToRadians(static_cast<qreal>(hue));
    qreal ringMid = kOuterR - kRingW / 2.0;
    QPointF ringPt(kCx + ringMid * qCos(hueRad),
                   kCy - ringMid * qSin(hueRad));
    p.setPen(QPen(QColor(0, 0, 0, 160), 1.5));
    p.setBrush(Qt::white);
    p.drawEllipse(ringPt, 6.0, 6.0);

    // --- SV crosshair ---
    qreal sx = sq.left() + (sat / 255.0) * sq.width();
    qreal sy = sq.top()  + (1.0 - val / 255.0) * sq.height();
    p.setPen(QPen(QColor(0, 0, 0, 200), 1.5));
    p.setBrush(Qt::white);
    p.drawEllipse(QPointF(sx, sy), 5.0, 5.0);
}
