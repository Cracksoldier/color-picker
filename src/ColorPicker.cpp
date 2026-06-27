#include "ColorPicker.h"
#include "PickerOverlay.h"
#include "WaylandPortalPicker.h"
#include <QGuiApplication>

ColorPicker::ColorPicker(QObject* parent)
    : QObject(parent)
{}

bool ColorPicker::isWayland() const {
    return QGuiApplication::platformName().startsWith("wayland");
}

void ColorPicker::startPick() {
    if (isWayland()) {
        auto* portal = new WaylandPortalPicker(this);
        connect(portal, &WaylandPortalPicker::colorPicked, this, &ColorPicker::colorPicked);
        connect(portal, &WaylandPortalPicker::pickCanceled, this, &ColorPicker::pickCanceled);
        connect(portal, &WaylandPortalPicker::colorPicked, portal, &QObject::deleteLater);
        connect(portal, &WaylandPortalPicker::pickCanceled, portal, &QObject::deleteLater);
        portal->startPick();
    } else {
        auto* overlay = new PickerOverlay(nullptr);
        connect(overlay, &PickerOverlay::colorPicked, this, &ColorPicker::colorPicked);
        connect(overlay, &PickerOverlay::pickCanceled, this, &ColorPicker::pickCanceled);
        overlay->activate();
    }
}
