#include <QApplication>
#include <oclero/qlementine/icons/QlementineIcons.hpp>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("ColorPicker");
    app.setOrganizationName("local");

    oclero::qlementine::icons::initializeIconTheme();

    app.setStyleSheet(R"(
        QWidget {
            background-color: #1e1e2e;
            color: #cdd6f4;
            font-size: 12px;
        }
        QPushButton {
            background-color: #313244;
            border: 1px solid #45475a;
            border-radius: 6px;
            padding: 4px 12px;
            min-height: 22px;
        }
        QPushButton:hover {
            background-color: #45475a;
        }
        QPushButton:pressed {
            background-color: #585b70;
        }
        QPushButton:disabled {
            color: #585b70;
            background-color: #313244;
            border-color: #313244;
        }
        QFrame[frameShape="4"],
        QFrame[frameShape="5"] {
            color: #45475a;
            background-color: #45475a;
        }
    )");

    MainWindow w;
    w.show();
    return app.exec();
}
