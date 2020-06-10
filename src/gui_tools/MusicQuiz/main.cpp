#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QAbstractButton>

#include "common/Log.hpp"

#include "MusicQuizController.hpp"
#include "gui_tools/QuizCreator/QuizCreator.hpp"


#if defined(_WIN32) || defined(WIN32)
#define WINDOWS_INTERFACE true
#else
#define WINDOWS_INTERFACE false
#endif


void errorMessage(const std::string& title, const std::string& errorMsg)
{
	QMessageBox::critical(nullptr, QString::fromLocal8Bit(title.c_str()), QString::fromLocal8Bit(errorMsg.c_str()), QMessageBox::Ok);
}

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
	QAbstractButton* quizCreatorButton = msgBox.addButton("Quiz Creator", QMessageBox::YesRole);
	QAbstractButton* exitButton = msgBox.addButton("Exit", QMessageBox::NoRole);
	msgBox.setStyleSheet(qss.readAll());
	qss.close();

	msgBox.exec();

	/** Start Selected Program */
	if ( msgBox.clickedButton() == musicQuizButton ) {
		/** Start Music Quiz */
		LOG_INFO("Music Quiz Selected.");

		try {
			MusicQuiz::MusicQuizController w;
			app.exec();
		} catch ( const std::exception& err ) {
			LOG_ERROR("Failed to start Music Quiz. " << err.what());
			errorMessage("Quiz Creator", "Quiz Creator Error. " + std::string(err.what()));
			return 1;
		} catch ( ... ) {
			LOG_ERROR("Failed to start Music Quiz. Unknown Error.");
			errorMessage("Quiz Creator", "Quiz Creator Error. Unknown Error.");
			return 1;
		}
	} else if ( msgBox.clickedButton() == quizCreatorButton ) {
		/** Start Quiz Creator */
		LOG_INFO("Quiz Creator Selected.");

		try {
			MusicQuiz::QuizCreator w;
			w.show();
			app.exec();
		} catch ( const std::exception& err ) {
			LOG_ERROR("Failed to start Quiz Creator. " << err.what());
			errorMessage("Quiz Creator", "Quiz Creator Error. " + std::string(err.what()));
			return 1;
		} catch ( ... ) {
			LOG_ERROR("Failed to start Quiz Creator. Unknown Error.");
			errorMessage("Quiz Creator", "Quiz Creator Error. Unknown Error.");
			return 1;
		}
	} else {
		/** Close Program */
		LOG_INFO("Exit Program Selected.");
	}

	return 0;
}


/** TODO LIST
 * Quiz Author
	- Add author to quiz creator.
	- Add author and quiz name to quiz intro screen.

 * Add guess the category in music quiz

 * Make settings menu pretty

 * Update popup boxes cosmetics
*/