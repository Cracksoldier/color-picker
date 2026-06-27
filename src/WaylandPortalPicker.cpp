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

    // Pre-compute handle path per XDG portal spec: strip leading ':' from unique name,
    // replace '.' with '_'. Subscribe BEFORE calling the method to avoid missing the
    // Response signal if the compositor delivers it before we connect.
    QString sender = QDBusConnection::sessionBus().baseService();
    QString senderPart = sender.mid(1).replace('.', '_');
    m_handlePath = "/org/freedesktop/portal/desktop/request/" + senderPart + "/" + token;

    bool connected = QDBusConnection::sessionBus().connect(
        "org.freedesktop.portal.Desktop",
        m_handlePath,
        "org.freedesktop.portal.Request",
        "Response",
        this,
        SLOT(onResponse(uint, QVariantMap))
    );

    if (!connected) {
        emit pickCanceled();
        return;
    }

    QDBusReply<QDBusObjectPath> reply = iface.call("PickColor", QString(""), options);
    if (!reply.isValid()) {
        QDBusConnection::sessionBus().disconnect(
            "org.freedesktop.portal.Desktop",
            m_handlePath,
            "org.freedesktop.portal.Request",
            "Response",
            this,
            SLOT(onResponse(uint, QVariantMap))
        );
        emit pickCanceled();
        return;
    }
}

void WaylandPortalPicker::onResponse(uint code, QVariantMap results) {
    // Unsubscribe using the exact path we subscribed with
    QDBusConnection::sessionBus().disconnect(
        "org.freedesktop.portal.Desktop",
        m_handlePath,
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
    const QDBusArgument arg = results["color"].value<QDBusArgument>();
    double r = 0, g = 0, b = 0;
    arg.beginStructure();
    arg >> r >> g >> b;
    arg.endStructure();

    emit colorPicked(QColor::fromRgbF(r, g, b));
}
