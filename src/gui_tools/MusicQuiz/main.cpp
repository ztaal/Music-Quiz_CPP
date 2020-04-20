#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QAbstractButton>

#include "common/Log.hpp"
#include "MusicQuizGUI.hpp"
#include "widgets/QuizEntry.hpp"
#include "widgets/QuizCategory.hpp"


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

		/** Test Code \todo remove this */
		try {
			MusicQuiz::QuizEntry entry11("C:/Users/Ztaal/Desktop/5.mp3", "Song 1", 1000, 0, 30000, -1, -1, nullptr);
			MusicQuiz::QuizEntry entry12("C:/Users/Ztaal/Desktop/test.wav", "Song 2", 2000, 0, 100, -1, -1, nullptr);
			MusicQuiz::QuizEntry entry13("C:/Users/Ztaal/Desktop/5.mp3", "Song 3", 3000, 0, 50000, -1, -1, nullptr);

			MusicQuiz::QuizCategory category1("Category 1", { &entry11, &entry12, &entry13 }, nullptr);

			MusicQuiz::QuizEntry entry21("C:/Users/Ztaal/Desktop/5.mp3", "Song 1", 1000, 0, 30000, -1, -1, nullptr);
			MusicQuiz::QuizEntry entry22("C:/Users/Ztaal/Desktop/test.wav", "Song 2", 2000, 0, 100, -1, -1, nullptr);
			MusicQuiz::QuizEntry entry23("C:/Users/Ztaal/Desktop/5.mp3", "Song 3", 3000, 0, 50000, -1, -1, nullptr);

			MusicQuiz::QuizCategory category2("Category 1", { &entry21, &entry22, &entry23 }, nullptr);

			MusicQuiz::MusicQuizGUI w;
			w.layout()->addWidget(&category1);
			w.layout()->addWidget(&category2);
			w.show();
			app.exec();
		} catch ( const std::exception& err ) {
			LOG_ERROR("Failed to start Quiz Creator. " << err.what());
#if WINDOWS_INTERFACE
			system("pause");
#endif
			return 1;
		} catch ( ... ) {
			LOG_ERROR("Failed to start Quiz Creator. Unknown Error.");
#if WINDOWS_INTERFACE
			system("pause");
#endif
			return 1;
		}
	} else {
		/** Close Program */
		LOG_INFO("Exit Program Selected.");
	}

	system("pause");

	return 0;
}