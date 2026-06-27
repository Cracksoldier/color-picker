# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
./color-picker
```

Release build: add `-DCMAKE_BUILD_TYPE=Release`. The binary is always at `build/color-picker`. There are no tests and no lint step.

Force X11 backend (useful to test the overlay picker on a Wayland session with Xwayland):
```bash
QT_QPA_PLATFORM=xcb ./color-picker
```

## Architecture

**Platform split** is the central design concern. `ColorPicker::isWayland()` checks `QGuiApplication::platformName()` **and** the `WAYLAND_DISPLAY` env var, then dispatches to one of two implementations:

- **X11** (pure X11, no `WAYLAND_DISPLAY`) → `PickerOverlay`: a frameless fullscreen `QWidget` that grabs the desktop with `QScreen::grabWindow(WId(0))` before showing, then samples `QImage::pixelColor` on click. Includes a magnifier loupe painted in `paintEvent`.
- **Wayland / XWayland** → `WaylandPortalPicker`: calls `org.freedesktop.portal.Screenshot.PickColor` over DBus. The compositor draws its own crosshair; no loupe is possible due to Wayland's security model. Requires `xdg-desktop-portal` to be running.

The `WAYLAND_DISPLAY` check is critical for AppImages: they bundle no Qt Wayland platform plugin, so Qt falls back to xcb even on Wayland sessions. On XWayland, `grabWindow(root)` returns a black image because the Wayland compositor never renders to the X11 root window — so we must route to the portal instead. The portal only needs QtDBus, not the Qt Wayland platform plugin.

Both emit `colorPicked(QColor)` / `pickCanceled()` and are owned/deleted by `ColorPicker`.

**Data flow:**
```
ColorSlot::pickRequested → MainWindow sets PickTarget enum
  → ColorPicker::startPick → platform picker emits colorPicked
  → MainWindow::onColorPicked → ColorSlot::setColor + ComparisonPanel::setColorA/B
```

**`ColorMath.h`** is header-only with no Qt signals — add new color math here. All comparison metrics (WCAG contrast ratio, pass/fail thresholds) live here. WCAG thresholds: AA normal ≥4.5, AA large ≥3.0, AAA normal ≥7.0, AAA large ≥4.5.

**UI** is a single fixed-width (340 px) `QMainWindow` with a Catppuccin Mocha stylesheet applied in `main.cpp`. Swatch colors are set via `QLabel::setStyleSheet("background-color: #RRGGBB")`.
