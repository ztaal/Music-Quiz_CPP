#include "MusicQuizController.hpp"

#include <thread>
#include <string>
#include <chrono>

#include <QMessageBox>

#include "common/Log.hpp"

#include "widgets/QuizTeam.hpp"
#include "widgets/QuizEntry.hpp"
#include "widgets/QuizCategory.hpp"


MusicQuiz::MusicQuizController::MusicQuizController(QWidget* parent) :
	QWidget(parent)
{	
	/** Connect Update Timer */
	connect(&_updateTimer, SIGNAL(timeout()), this, SLOT(executeQuiz()));

	/** Start Timer */
	_updateTimer.start(_updateTimerDelay);
}

MusicQuiz::MusicQuizController::~MusicQuizController()
{
	/** Stop Update Timer */
	if ( _updateTimer.isActive() ) {
		_updateTimer.stop();
	}
}

void MusicQuiz::MusicQuizController::executeQuiz()
{
	switch ( _quizState )
	{
	case MusicQuiz::MusicQuizController::INIT:
	{
		/** Create Quiz Selector */
		_quizSelector = new MusicQuiz::QuizSelector();

		/** Connect Signals */
		connect(_quizSelector, SIGNAL(quitSignal()), this, SLOT(closeWindow()));
		connect(_quizSelector, SIGNAL(quizSelectedSignal(std::string)), this, SLOT(quizSelected(std::string)));

		/** Execute widget */
		_quizSelector->exec();

		/** Go to select quiz state */
		_quizState = SELECTING_QUIZ;
	}
	break;
	case MusicQuiz::MusicQuizController::SELECTING_QUIZ:
	{
		if ( !_quizSelected ) {
			break;
		}

		/** Go to select team state */
		_quizState = SELECTING_TEAM;
	}
	break;
	case MusicQuiz::MusicQuizController::SELECTING_TEAM:
	{
		if ( !_teamSelected ) {
			break;
		}

		// \todo remove
		MusicQuiz::QuizEntry entry11("C:/Users/Ztaal/Desktop/5.mp3", "Song 1", 1000, 0, 30000, -1, -1, nullptr);
		MusicQuiz::QuizEntry entry12("C:/Users/Ztaal/Desktop/test.wav", "Song 2", 2000, 0, 100, -1, -1, nullptr);
		MusicQuiz::QuizEntry entry13("C:/Users/Ztaal/Desktop/5.mp3", "Song 3", 3000, 0, 50000, -1, -1, nullptr);

		MusicQuiz::QuizCategory* category1 = new MusicQuiz::QuizCategory("Category 1", { &entry11, &entry12, &entry13 }, nullptr);

		MusicQuiz::QuizEntry entry21("C:/Users/Ztaal/Desktop/5.mp3", "Song 1", 100, 0, 30000, -1, -1, nullptr);
		MusicQuiz::QuizEntry entry22("C:/Users/Ztaal/Desktop/test.wav", "Song 2", 200, 0, 100, -1, -1, nullptr);
		MusicQuiz::QuizEntry entry23("C:/Users/Ztaal/Desktop/5.mp3", "Song 3", 300, 0, 50000, -1, -1, nullptr);

		MusicQuiz::QuizCategory* category2 = new MusicQuiz::QuizCategory("Category 2", { &entry21, &entry22, &entry23 }, nullptr);

		MusicQuiz::QuizTeam* team1 = new MusicQuiz::QuizTeam("Team 1", QColor(255, 0, 0), nullptr);
		MusicQuiz::QuizTeam* team2 = new MusicQuiz::QuizTeam("Team 2", QColor(0, 255, 0), nullptr);

		std::vector< QString > rowCategories = { "Row Category 1", "Row Category 2", "Row Category 3" };
		std::vector< MusicQuiz::QuizTeam* > teams = { team1, team2 };
		std::vector< MusicQuiz::QuizCategory* > categories = { category1, category2 };

		_quizBoard = new MusicQuiz::QuizBoard(categories, rowCategories, teams);
		connect(_quizBoard, SIGNAL(quitSignal()), this, SLOT(closeWindow()));
		_quizBoard->exec();

		/** Go to quiz running state */
		_quizState = QUIZ_RUNNING;
	}
	break;
	case MusicQuiz::MusicQuizController::QUIZ_RUNNING:
	{
		if ( !_gameCompleted ) {
			break;
		}

		/** Go to victory screen state */
		_quizState = VICTORY_SCREEN;
	}
	break;
	case MusicQuiz::MusicQuizController::VICTORY_SCREEN:
		break;
	default:
		break;
	}
}


void MusicQuiz::MusicQuizController::closeEvent(QCloseEvent* event)
{
	if ( closeWindow() ) {
		event->accept();
	} else {
		event->ignore();
	}
}

bool MusicQuiz::MusicQuizController::closeWindow()
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Close Music Quiz?", "Are you sure you want to close the application?",
		QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

	if ( resBtn == QMessageBox::Yes ) {
		/** Call Destructor */
		QApplication::quit();

		return true;
	}

	return false;
}

void MusicQuiz::MusicQuizController::keyPressEvent(QKeyEvent* event)
{
	switch ( event->key() )
	{
	case Qt::Key_Escape:
		closeWindow();
		break;
	default:
		QWidget::keyPressEvent(event);
		break;
	}
}

void MusicQuiz::MusicQuizController::quizSelected(const std::string &quiz)
{
	/** Set Quiz Selected */
	_quizSelected = true;
	LOG_INFO("Quiz Selected: " << quiz);

	/** Remove Quiz Selector */
	_quizSelector->hide();
	delete _quizSelector;
}