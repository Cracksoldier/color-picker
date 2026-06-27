#include "PickerOverlay.h"
#include "ColorMath.h"
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCloseEvent>
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
    // Grab the full virtual desktop (all monitors) before showing the overlay.
    // On X11, grabWindow(WId(0)) captures the root window which spans all screens.
    // We store the virtual geometry so mousePressEvent uses the same coordinate space.
    QScreen* screen = QGuiApplication::primaryScreen();
    m_virtualGeom = screen->virtualGeometry();
    m_screenshot  = screen->grabWindow(WId(0));
    m_image       = m_screenshot.toImage(); // cache once — reused in paint and click

    QTimer::singleShot(50, this, [this]() {
        setGeometry(m_virtualGeom);
        showFullScreen();
        raise();
        activateWindow();
        QGuiApplication::setOverrideCursor(Qt::CrossCursor);
    });
}

void PickerOverlay::paintEvent(QPaintEvent*) {
    if (m_screenshot.isNull()) return;

    QPainter p(this);
    p.drawPixmap(0, 0, m_screenshot);

    QPoint cur = mapFromGlobal(QCursor::pos());

    // Sample hover color from cached image
    if (m_image.rect().contains(cur))
        m_hoverColor = m_image.pixelColor(cur);

    // Loupe parameters
    const int srcRadius = 8;
    const int loupeSize = 128;
    const int offsetX   = 24;
    const int offsetY   = -loupeSize - 24;

    QRect loupeRect(cur.x() + offsetX, cur.y() + offsetY, loupeSize, loupeSize);
    // Flip loupe if it would go off-screen
    if (loupeRect.right() > width())
        loupeRect.moveLeft(cur.x() - offsetX - loupeSize);
    if (loupeRect.top() < 0)
        loupeRect.moveTop(cur.y() + 24);

    // Sample region from screenshot; use QPainter::drawPixmap with src rect to
    // avoid an intermediate allocation (scale and blit in one step)
    QRect srcRect(cur.x() - srcRadius, cur.y() - srcRadius,
                  srcRadius * 2 + 1, srcRadius * 2 + 1);
    srcRect = srcRect.intersected(m_screenshot.rect());
    if (srcRect.isEmpty()) return;

    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath circlePath;
    circlePath.addEllipse(loupeRect);
    p.setClipPath(circlePath);
    p.drawPixmap(loupeRect, m_screenshot, srcRect); // single-step scale+blit
    p.setClipping(false);

    // Border and crosshair
    p.setPen(QPen(QColor("#cdd6f4"), 2));
    p.drawEllipse(loupeRect);
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
        // Convert global click to screenshot-local coords using the same virtual
        // geometry recorded in activate() — consistent with the grabbed pixmap.
        QPoint pos      = event->globalPosition().toPoint();
        QPoint localPos = pos - m_virtualGeom.topLeft();

        QColor picked;
        if (!m_image.isNull() && m_image.rect().contains(localPos))
            picked = m_image.pixelColor(localPos);

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

// WM-initiated close (Alt+F4, compositor kill): ensure cursor is always restored.
void PickerOverlay::closeEvent(QCloseEvent* event) {
    finish();
    QWidget::closeEvent(event);
}

void PickerOverlay::finish() {
    QGuiApplication::restoreOverrideCursor();
    hide();
    deleteLater();
}
