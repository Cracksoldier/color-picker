#pragma once
#include <QColor>
#include <QString>
#include <cmath>

namespace ColorMath {

[[nodiscard]] inline double linearize(double c) {
    return c <= 0.04045 ? c / 12.92 : std::pow((c + 0.055) / 1.055, 2.4);
}

[[nodiscard]] inline double luminance(QColor col) {
    return 0.2126 * linearize(col.redF())
         + 0.7152 * linearize(col.greenF())
         + 0.0722 * linearize(col.blueF());
}

[[nodiscard]] inline double contrastRatio(QColor a, QColor b) {
    double L1 = luminance(a), L2 = luminance(b);
    if (L1 < L2) std::swap(L1, L2);
    return (L1 + 0.05) / (L2 + 0.05);
}

[[nodiscard]] inline QString toHex(QColor c) {
    return c.name(QColor::HexRgb).toUpper();
}

[[nodiscard]] inline QString toRgbStr(QColor c) {
    return QString("rgb(%1, %2, %3)").arg(c.red()).arg(c.green()).arg(c.blue());
}

[[nodiscard]] inline QString toHslStr(QColor c) {
    QColor h = c.toHsl();
    // hslHue() returns -1 for achromatic colors (greys, black, white); clamp to 0.
    int hue = qMax(0, h.hslHue());
    return QString("hsl(%1, %2%, %3%)")
        .arg(hue)
        .arg(qRound(h.hslSaturationF() * 100))
        .arg(qRound(h.lightnessF() * 100));
}

} // namespace ColorMath
