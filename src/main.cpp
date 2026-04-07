#include <QApplication>
#include <QIcon>

#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    // 从 Qt 资源中加载图标，避免工作目录变化导致图标失效。
    app.setWindowIcon(QIcon(QStringLiteral(":/icons/Snake.ico")));

    SnakeRewind::MainWindow window;
    window.show();

    return app.exec();
}
