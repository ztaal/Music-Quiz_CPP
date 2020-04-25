#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QAbstractButton>

#include "common/Log.hpp"
#include "util/QuizLoader.hpp"

#include "MusicQuizController.hpp"

#include "widgets/QuizTeam.hpp"
#include "widgets/QuizBoard.hpp"
#include "widgets/QuizEntry.hpp"
#include "widgets/QuizCategory.hpp"


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
			std::vector<std::string> quizList = MusicQuiz::util::QuizLoader::getListOfQuizzes();
			for ( size_t i = 0; i < quizList.size(); ++i ) {
				LOG_DEBUG("Quiz: " << quizList[i]);
			}
			MusicQuiz::util::QuizLoader::QuizPreview quizPreview = MusicQuiz::util::QuizLoader::getQuizPreview(1);
			LOG_DEBUG(quizPreview);

			MusicQuiz::MusicQuizController w;
			app.exec();
		} catch ( const std::exception& err ) {
			LOG_ERROR("Failed to start Music Quiz. " << err.what());
			errorMessage("Quiz Creator", "Quiz Creator Error. " + std::string(err.what()));
#if WINDOWS_INTERFACE
			system("pause");
#endif
			return 1;
		} catch ( ... ) {
			LOG_ERROR("Failed to start Music Quiz. Unknown Error.");
			errorMessage("Quiz Creator", "Quiz Creator Error. Unknown Error.");
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

			MusicQuiz::QuizCategory *category1 = new MusicQuiz::QuizCategory("Category 1", { &entry11, &entry12, &entry13 }, nullptr);

			MusicQuiz::QuizEntry entry21("C:/Users/Ztaal/Desktop/5.mp3", "Song 1", 100, 0, 30000, -1, -1, nullptr);
			MusicQuiz::QuizEntry entry22("C:/Users/Ztaal/Desktop/test.wav", "Song 2", 200, 0, 100, -1, -1, nullptr);
			MusicQuiz::QuizEntry entry23("C:/Users/Ztaal/Desktop/5.mp3", "Song 3", 300, 0, 50000, -1, -1, nullptr);

			MusicQuiz::QuizCategory* category2 = new MusicQuiz::QuizCategory("Category 2", { &entry21, &entry22, &entry23 }, nullptr);

			MusicQuiz::QuizTeam *team1 = new MusicQuiz::QuizTeam("Team 1", QColor(255, 0, 0), nullptr);
			MusicQuiz::QuizTeam *team2 = new MusicQuiz::QuizTeam("Team 2", QColor(0, 255, 0), nullptr);

			std::vector< QString > rowCategories = { "Row Category 1", "Row Category 2", "Row Category 3" };
			std::vector< MusicQuiz::QuizTeam* > teams = { team1, team2 };
			std::vector< MusicQuiz::QuizCategory* > categories = { category1, category2 };
			MusicQuiz::QuizBoard* board = new MusicQuiz::QuizBoard(categories, rowCategories, teams, nullptr);

			board->exec();
			app.exec();
		} catch ( const std::exception& err ) {
			LOG_ERROR("Failed to start Quiz Creator. " << err.what());
			errorMessage("Quiz Creator", "Quiz Creator Error. " + std::string(err.what()));
#if WINDOWS_INTERFACE
			system("pause");
#endif
			return 1;
		} catch ( ... ) {
			LOG_ERROR("Failed to start Quiz Creator. Unknown Error.");
			errorMessage("Quiz Creator", "Quiz Creator Error. Unknown Error.");
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