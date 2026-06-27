#pragma once
#include <QObject>
#include <QColor>

class WaylandPortalPicker : public QObject {
    Q_OBJECT
public:
    explicit WaylandPortalPicker(QObject* parent = nullptr);
    void startPick();

signals:
    void colorPicked(QColor color);
    void pickCanceled();

private slots:
    void onResponse(uint code, QVariantMap results);

private:
    QString m_handlePath;
};
