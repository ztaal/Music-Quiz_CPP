#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QAbstractButton>

#include "common/Log.hpp"
#include "MusicQuizGUI.hpp"


#if defined(_WIN32) || defined(WIN32)
#define WINDOWS_INTERFACE true
#else
#define WINDOWS_INTERFACE false
#endif


int main(int argc, char* argv[])
{
	/** Create QApplication */
	QApplication app(argc, argv);

	/** Set Stylesheet */
	QFile qss(QString::fromStdString("../src/gui_tools/MusicQuiz/stylesheet_musicQuiz.qss"));
	qss.open(QFile::ReadOnly);
	app.setStyleSheet(qss.readAll());

	/** Pop to select program (MusicQuiz or QuizCreator) */
	QMessageBox msgBox(QMessageBox::Question, "Select Program", "Select Program", QMessageBox::NoButton, nullptr, Qt::WindowStaysOnTopHint);
	QAbstractButton* musicQuizButton = msgBox.addButton("Music Quiz", QMessageBox::YesRole);
	QAbstractButton* musicCreatorButton = msgBox.addButton("Quiz Creator", QMessageBox::YesRole);
	QAbstractButton* exitButton = msgBox.addButton("Exit", QMessageBox::NoRole);
	msgBox.setStyleSheet(qss.readAll());
	qss.close();

	msgBox.exec();

	/** Start Selected Program */
	if ( msgBox.clickedButton() == musicQuizButton ) {
		/** Start Music Quiz */
		LOG_INFO("Music Quiz Selected.");

		try {
			MusicQuiz::MusicQuizGUI w;
			w.show();
			app.exec();
		} catch ( const std::exception& err ) {
			LOG_ERROR("Failed to start Music Quiz. " << err.what());
#if WINDOWS_INTERFACE
			system("pause");
#endif
			return 1;
		} catch ( ... ) {
			LOG_ERROR("Failed to start Music Quiz. Unknown Error.");
#if WINDOWS_INTERFACE
			system("pause");
#endif
			return 1;
		}
	} else if ( msgBox.clickedButton() == musicCreatorButton ) {
		/** Start Quiz Creator */
		LOG_INFO("Quiz Creator Selected.");
	} else {
		/** Close Program */
		LOG_INFO("Exit Program Selected.");
	}

	return 0;
}
