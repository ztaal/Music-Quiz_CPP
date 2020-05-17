#include "MusicQuizController.hpp"

#include <thread>
#include <string>
#include <chrono>

#include <QMessageBox>

#include "common/Log.hpp"

#include "gui_tools/widgets/QuizTeam.hpp"
#include "gui_tools/widgets/QuizEntry.hpp"
#include "gui_tools/widgets/QuizCategory.hpp"


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
		_quizSelector = nullptr;
	}

	/** Close Quiz Board */
	if ( _quizBoard != nullptr ) {
		_quizBoard->close();
		_quizBoard = nullptr;
	}
}

void MusicQuiz::MusicQuizController::executeQuiz()
{
	switch ( _quizState )
	{
	case MusicQuiz::MusicQuizController::SELECT_QUIZ:
	{
		/** Create Quiz Selector */
		_quizSelector = new MusicQuiz::QuizSelector;

		/** Connect Signals */
		connect(_quizSelector, SIGNAL(quitSignal()), this, SLOT(quitQuiz()));
		connect(_quizSelector, SIGNAL(quizSelectedSignal(size_t, const MusicQuiz::QuizSettings&)), this, SLOT(quizSelected(size_t, const MusicQuiz::QuizSettings&)));

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
		_teamSelector = new MusicQuiz::TeamSelector;

		/** Connect Signals */
		connect(_teamSelector, SIGNAL(quitSignal()), this, SLOT(quitQuiz()));
		connect(_teamSelector, SIGNAL(teamSelectedSignal(const std::vector<MusicQuiz::QuizTeam*>&)), this, SLOT(teamSelected(const std::vector<MusicQuiz::QuizTeam*>&)));

		/** Show Widget */
		_teamSelector->exec();

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
		_quizBoard = MusicQuiz::QuizFactory::createQuiz(_selectedQuizIdx, _settings, std::make_shared<audio::AudioPlayer>(_audioPlayer), _teams);

		/** Connect Signals */
		connect(_quizBoard, SIGNAL(quitSignal()), this, SLOT(quitQuiz()));

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

void MusicQuiz::MusicQuizController::quitQuiz()
{
	/** Call Destructor */
	QApplication::quit();
}

void MusicQuiz::MusicQuizController::quizSelected(const size_t quizIdx, const MusicQuiz::QuizSettings& settings)
{
	/** Set Quiz Selected */
	_quizSelected = true;
	_selectedQuizIdx = quizIdx;
	LOG_INFO("Quiz Selected #" << _selectedQuizIdx);

	/** Set Settings */
	_settings = settings;

	/** Remove Quiz Selector */
	_quizSelector->hide();
	delete _quizSelector;
	_quizSelector = nullptr;
}

void MusicQuiz::MusicQuizController::teamSelected(const std::vector<MusicQuiz::QuizTeam*>& teams)
{
	/** Set Quiz Selected */
	_teamSelected = true;
	if ( !teams.empty() ) {
		LOG_INFO("Selected teams:");
		for ( size_t i = 0; i < teams.size(); ++i ) {
			LOG_INFO("\t" << teams[i]->getName().toStdString());
		}
	} else {
		LOG_INFO("Playing Quiz wihtout teams");
	}

	/** Set Teams */
	_teams = teams;

	/** Remove Team Selector */
	_teamSelector->hide();
	delete _teamSelector;
	_teamSelector = nullptr;
}