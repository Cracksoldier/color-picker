#include "WaylandPortalPicker.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusConnection>
#include <QDBusArgument>
#include <QDBusObjectPath>
#include <QUuid>

WaylandPortalPicker::WaylandPortalPicker(QObject* parent)
    : QObject(parent)
{}

void WaylandPortalPicker::startPick() {
    QDBusInterface iface(
        "org.freedesktop.portal.Desktop",
        "/org/freedesktop/portal/desktop",
        "org.freedesktop.portal.Screenshot",
        QDBusConnection::sessionBus()
    );

    if (!iface.isValid()) {
        emit pickCanceled();
        return;
    }

    QString token = "cp_" + QUuid::createUuid().toString(QUuid::Id128).left(8);
    QVariantMap options;
    options["handle_token"] = token;

    QDBusReply<QDBusObjectPath> reply = iface.call("PickColor", QString(""), options);
    if (!reply.isValid()) {
        emit pickCanceled();
        return;
    }

    QString handlePath = reply.value().path();

    bool connected = QDBusConnection::sessionBus().connect(
        "org.freedesktop.portal.Desktop",
        handlePath,
        "org.freedesktop.portal.Request",
        "Response",
        this,
        SLOT(onResponse(uint, QVariantMap))
    );

    if (!connected) {
        emit pickCanceled();
    }
}

void WaylandPortalPicker::onResponse(uint code, QVariantMap results) {
    // Unsubscribe from future signals
    QDBusConnection::sessionBus().disconnect(
        "org.freedesktop.portal.Desktop",
        {},
        "org.freedesktop.portal.Request",
        "Response",
        this,
        SLOT(onResponse(uint, QVariantMap))
    );

    if (code != 0) {
        emit pickCanceled();
        return;
    }

    if (!results.contains("color")) {
        emit pickCanceled();
        return;
    }

    // Color is encoded as (ddd) — sRGB in [0,1]
    QDBusArgument arg = results["color"].value<QDBusArgument>();
    double r = 0, g = 0, b = 0;
    arg.beginStructure();
    arg >> r >> g >> b;
    arg.endStructure();

    emit colorPicked(QColor::fromRgbF(r, g, b));
}
