#include "PickerOverlay.h"
#include "ColorMath.h"
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QCursor>

PickerOverlay::PickerOverlay(QWidget* parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);
}

void PickerOverlay::activate() {
    // Grab before showing so the overlay itself doesn't appear in the screenshot
    QScreen* screen = QGuiApplication::primaryScreen();
    m_screenshot = screen->grabWindow(WId(0));

    QTimer::singleShot(50, this, [this, screen]() {
        setGeometry(screen->geometry());
        showFullScreen();
        raise();
        activateWindow();
        QGuiApplication::setOverrideCursor(Qt::CrossCursor);
    });
}

void PickerOverlay::paintEvent(QPaintEvent*) {
    QPainter p(this);

    // Draw the screenshot as background
    p.drawPixmap(0, 0, m_screenshot);

    if (m_screenshot.isNull()) return;

    QPoint cur = mapFromGlobal(QCursor::pos());
    QImage img = m_screenshot.toImage();

    // Sample hover color
    if (img.rect().contains(cur))
        m_hoverColor = img.pixelColor(cur);

    // Loupe parameters
    const int srcRadius = 8;    // px sampled from screenshot (half-size)
    const int loupeSize = 128;
    const int offsetX   = 24;
    const int offsetY   = -loupeSize - 24;

    QRect loupeRect(cur.x() + offsetX, cur.y() + offsetY, loupeSize, loupeSize);
    // Flip loupe to the other side if it would go off-screen
    if (loupeRect.right() > width())
        loupeRect.moveLeft(cur.x() - offsetX - loupeSize);
    if (loupeRect.top() < 0)
        loupeRect.moveTop(cur.y() + 24);

    // Sample region from screenshot
    QRect srcRect(cur.x() - srcRadius, cur.y() - srcRadius,
                  srcRadius * 2 + 1, srcRadius * 2 + 1);
    srcRect = srcRect.intersected(img.rect());
    if (srcRect.isEmpty()) return;

    QPixmap loupePixmap = m_screenshot.copy(srcRect).scaled(
        loupeSize, loupeSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    // Clip to circle
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath circlePath;
    circlePath.addEllipse(loupeRect);
    p.setClipPath(circlePath);
    p.drawPixmap(loupeRect, loupePixmap);
    p.setClipping(false);

    // Border
    p.setPen(QPen(QColor("#cdd6f4"), 2));
    p.drawEllipse(loupeRect);

    // Center crosshair lines
    QPoint center = loupeRect.center();
    p.setPen(QPen(QColor("#cdd6f4"), 1));
    p.drawLine(center.x() - 6, center.y(), center.x() + 6, center.y());
    p.drawLine(center.x(), center.y() - 6, center.x(), center.y() + 6);

    // Hex label below loupe
    if (m_hoverColor.isValid()) {
        QString hex = ColorMath::toHex(m_hoverColor);
        QRect hexRect(loupeRect.left(), loupeRect.bottom() + 4, loupeSize, 18);
        p.fillRect(hexRect, QColor(30, 30, 46, 220));
        p.setPen(QColor("#cdd6f4"));
        QFont f = p.font();
        f.setFamily("monospace");
        f.setPixelSize(11);
        p.setFont(f);
        p.drawText(hexRect, Qt::AlignCenter, hex);
    }
}

void PickerOverlay::mouseMoveEvent(QMouseEvent*) {
    update();
}

void PickerOverlay::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->globalPosition().toPoint();
        QRect screenGeom = QGuiApplication::primaryScreen()->geometry();
        QPoint localPos = pos - screenGeom.topLeft();

        QColor picked;
        if (!m_screenshot.isNull()) {
            QImage img = m_screenshot.toImage();
            if (img.rect().contains(localPos))
                picked = img.pixelColor(localPos);
        }
        finish();
        if (picked.isValid())
            emit colorPicked(picked);
        else
            emit pickCanceled();
    } else {
        finish();
        emit pickCanceled();
    }
}

void PickerOverlay::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        finish();
        emit pickCanceled();
    }
}

void PickerOverlay::finish() {
    QGuiApplication::restoreOverrideCursor();
    hide();
    deleteLater();
}
