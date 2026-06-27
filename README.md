# color-picker

A compact desktop color picker built with Qt6 and C++. Pick any pixel from the screen, store two colors, and compare them with WCAG contrast ratio metrics.

![Color Picker UI](https://github.com/Cracksoldier/color-picker/raw/main/preview.png)

## Features

- **Screen color picking** — click anywhere on the desktop to sample a pixel
  - X11: fullscreen overlay with a magnifier loupe
  - Wayland: native compositor picker via `xdg-desktop-portal`
- **Two color slots** — hold Color A and Color B simultaneously
- **WCAG comparison** — contrast ratio and AA/AAA pass/fail for normal and large text
- **Color formats** — hex, RGB, and HSL displayed per slot
- **Multi-monitor aware** — overlay covers the full virtual desktop on X11

## Requirements

- Qt 6.2+
- CMake 3.22+
- A C++20 compiler (GCC or Clang)
- On Wayland: `xdg-desktop-portal` with a backend that implements `org.freedesktop.portal.Screenshot`

## Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

The binary is `build/color-picker`.

## Usage

Run the binary. Click **Pick** on either color slot, then click anywhere on the screen to sample that pixel's color. On X11 a loupe appears under the cursor showing a magnified view; on Wayland the system's native crosshair is used.

Once both slots are filled the **Comparison** panel updates automatically with the WCAG 2.1 contrast ratio and pass/fail badges.

**Swap A ↔ B** exchanges the two colors. **Reset** clears both slots.

To force the X11 overlay on a Wayland+Xwayland session:

```bash
QT_QPA_PLATFORM=xcb ./color-picker
```

## License

[MIT](LICENSE) © 2026 Cracksoldier
