#include "header.h"
#include <QApplication>
int main(int argc, char *argv[]) {
    // Создание приложения
    QApplication a(argc, argv);

    // Создание основного окна
    MainWindow w;

    // Отображение окна
    w.show();

    // Запуск основного цикла приложения
    return a.exec();
}
