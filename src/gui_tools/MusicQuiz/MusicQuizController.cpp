#include "MusicQuizController.hpp"

#include <thread>
#include <string>
#include <chrono>

#include <QMessageBox>

#include "common/Log.hpp"

#include "widgets/QuizTeam.hpp"
#include "widgets/QuizEntry.hpp"
#include "widgets/QuizFactory.hpp"
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

	/** Close Quiz Creator */
	if ( _quizSelector != nullptr ) {
		_quizSelector->close();
	}

	/** Close Quiz Board */
	if ( _quizBoard != nullptr ) {
		_quizBoard->close();
	}
}

void MusicQuiz::MusicQuizController::executeQuiz()
{
	switch ( _quizState )
	{
	case MusicQuiz::MusicQuizController::SELECT_QUIZ:
	{
		/** Create Quiz Selector */
		_quizSelector = new MusicQuiz::QuizSelector();

		/** Connect Signals */
		connect(_quizSelector, SIGNAL(quitSignal()), this, SLOT(closeWindow()));
		connect(_quizSelector, SIGNAL(quizSelectedSignal(size_t)), this, SLOT(quizSelected(size_t)));

		/** Show widget */
		_quizSelector->exec();

		/** Go to select team state */
		_quizState = SELECT_TEAM;
	}
	break;
	case MusicQuiz::MusicQuizController::SELECT_TEAM:
	{
		/** Wait for quiz selection to complete */
		if ( !_quizSelected ) {
			break;
		}

		/** Create Team Selector */
		MusicQuiz::QuizTeam* team1 = new MusicQuiz::QuizTeam("Team 1", QColor(255, 0, 0), nullptr);
		MusicQuiz::QuizTeam* team2 = new MusicQuiz::QuizTeam("Team 2", QColor(0, 255, 0), nullptr);
		_teams = { team1, team2 };

		/** Connect Signals */

		/** Show Widget */

		/** Go to select quiz intro screen state */
		_quizState = QUIZ_INTRO_SCREEN;
	}
	break;
	case MusicQuiz::MusicQuizController::QUIZ_INTRO_SCREEN:
	{
		/** Wait for team selection to complete */
		if ( !_teamSelected ) {
			break;
		}

		/** Show Intro Screen */

		/** Go to select team state */
		_quizState = RUN_QUIZ;
	}
	break;
	case MusicQuiz::MusicQuizController::RUN_QUIZ:
	{
		/** Wait for intro scree to be done showing */
		if ( !_introScreenDone ) {
			break;
		}

		/** Create Quiz Board */
		_quizBoard = MusicQuiz::QuizFactory::createQuiz(_quizSelected, _settings, _teams);

		/** Connect Signals */
		connect(_quizBoard, SIGNAL(quitSignal()), this, SLOT(closeWindow()));

		/** Show Widget */
		_quizBoard->exec();

		/** Go to quiz running state */
		_quizState = VICTORY_SCREEN;
	}
	break;
	case MusicQuiz::MusicQuizController::VICTORY_SCREEN:
	{
		/** Wait for Quiz to complete */
		if ( !_gameCompleted ) {
			break;
		}

		/** Show Victory Screen */

		/** Go to victory screen state */
		_quizState = VICTORY_SCREEN;
	}
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
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Close Music Quiz?", "Are you sure you want to close the quiz?",
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

void MusicQuiz::MusicQuizController::quizSelected(const size_t quizIdx)
{
	/** Set Quiz Selected */
	_quizSelected = true;
	_selectedQuizIdx = quizIdx;
	LOG_INFO("Quiz Selected #" << _selectedQuizIdx);

	/** Remove Quiz Selector */
	_quizSelector->hide();
	delete _quizSelector;
}