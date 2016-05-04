#include <QApplication>
#include <QWidget>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget w;
    w.show();
    w.setWindowTitle("haha");

    QPushButton button("exit");
    button.setParent(&w);
    button.show();

    QObject::connect(&button, SIGNAL(clicked()), &w, SLOT(close()));

    return app.exec();
}
