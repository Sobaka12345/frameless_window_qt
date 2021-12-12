#include "main_window.h"

#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	{
		QFile styleSheet(":/stylesheet.qss");
        styleSheet.open(QIODevice::ReadOnly);
        a.setStyleSheet(styleSheet.readAll());
        styleSheet.close();
    }

    MainWindow w;
    w.show();
    return a.exec();
}
