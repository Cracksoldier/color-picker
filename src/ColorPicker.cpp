#include "ColorPicker.h"
#include "PickerOverlay.h"
#ifdef HAVE_WAYLAND_PORTAL
#include "WaylandPortalPicker.h"
#endif
#include <QGuiApplication>
#include <QProcessEnvironment>

ColorPicker::ColorPicker(QObject* parent)
    : QObject(parent)
{}

bool ColorPicker::isWayland() const {
    if (QGuiApplication::platformName().startsWith("wayland"))
        return true;
    // Running as X11 under XWayland (e.g. AppImage without Qt Wayland plugin):
    // grabWindow on the root X11 window returns black because the Wayland
    // compositor never renders to it. Use the portal over DBus instead.
    return !QProcessEnvironment::systemEnvironment().value("WAYLAND_DISPLAY").isEmpty();
}

void ColorPicker::startPick() {
#ifdef HAVE_WAYLAND_PORTAL
    if (isWayland()) {
        auto* portal = new WaylandPortalPicker(this);
        connect(portal, &WaylandPortalPicker::colorPicked, this, &ColorPicker::colorPicked);
        connect(portal, &WaylandPortalPicker::pickCanceled, this, &ColorPicker::pickCanceled);
        connect(portal, &WaylandPortalPicker::colorPicked, portal, &QObject::deleteLater);
        connect(portal, &WaylandPortalPicker::pickCanceled, portal, &QObject::deleteLater);
        portal->startPick();
        return;
    }
#endif
    auto* overlay = new PickerOverlay(nullptr);
    connect(overlay, &PickerOverlay::colorPicked, this, &ColorPicker::colorPicked);
    connect(overlay, &PickerOverlay::pickCanceled, this, &ColorPicker::pickCanceled);
    overlay->activate();
}
