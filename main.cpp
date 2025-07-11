#include <QApplication>
#include <QIcon>
#include "MainWindow.h"
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon/perevodchik.ico"));
    MainWindow w;
    w.setAppLanguage("ru");
    w.show();
    return app.exec();
} 