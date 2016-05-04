#include <QApplication>
#include <QWidget>
#include <QCompleter>
#include <QLineEdit>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget w;

    QLineEdit edit;
    edit.setParent(&w);
    edit.setEchoMode(QLineEdit::Password);
    edit.text();
    edit.setPlaceholderText("...");
    edit.show();

    QCompleter completer(QStringList() << "aab" << "123" << "998" );
    completer.setFilterMode(Qt::MatchContains);
    edit.setCompleter(&completer);

    w.show();
    return app.exec();
}
