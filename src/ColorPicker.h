#pragma once
#include <QObject>
#include <QColor>

class PickerOverlay;
class WaylandPortalPicker;

class ColorPicker : public QObject {
    Q_OBJECT
public:
    explicit ColorPicker(QObject* parent = nullptr);
    void startPick();

signals:
    void colorPicked(QColor color);
    void pickCanceled();

private:
    [[nodiscard]] bool isWayland() const;
};
