
#include <QApplication>
#include <QFile>

#include "JeopardyGUI.hpp"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QFile qss(QString::fromStdString(":/stylesheet_jeopardy.qss"));
	qss.open(QFile::ReadOnly);
	app.setStyleSheet(qss.readAll());
	qss.close();

	jeopardy::JeopardyGUI w;

	w.show();
	app.exec();

	return 0;
}
