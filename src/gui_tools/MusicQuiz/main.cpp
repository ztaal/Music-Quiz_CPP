
#include <QApplication>
#include <QFile>

#include "MusicQuizGUI.hpp"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QFile qss(QString::fromStdString("../src/gui_tools/MusicQuiz/stylesheet_musicQuiz.qss"));
	qss.open(QFile::ReadOnly);
	app.setStyleSheet(qss.readAll());
	qss.close();

	MusicQuiz::MusicQuizGUI w;

	w.show();
	app.exec();

	return 0;
}
