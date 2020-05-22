#pragma once 

#include <string>
#include <atomic>
#include <memory>

#include <QtGui>
#include <QTimer>
#include <QtCore>
#include <QWidget>
#include <QKeyEvent>
#include <QCloseEvent>

#include "ui_MusicQuizGUI.h"

#include "util/QuizSettings.hpp"
#include "audio/AudioPlayer.hpp"

#include "gui_tools/widgets/QuizTeam.hpp"
#include "gui_tools/widgets/QuizEntry.hpp"
#include "gui_tools/widgets/QuizBoard.hpp"


namespace MusicQuiz {
	class QuizBoard;
	class QuizFactory;
	class QuizSelector;
	class TeamSelector;
	class QuizIntroScreen;
	class QuizWinningScreen;

	class MusicQuizController : public QWidget
	{
		Q_OBJECT

	public:
		enum QuizState
		{
			SELECT_QUIZ = 0,
			SELECT_TEAM = 1,
			QUIZ_INTRO_SCREEN = 2,
			RUN_QUIZ = 3,
			VICTORY_SCREEN = 4
		};

		/**
		 * @brief Constructor
		 *
		 * @param[in] parent The parent widget.
		 */
		explicit MusicQuizController(QWidget *parent = nullptr);

		/**
		 * @brief Default destructor
		 */
		virtual ~MusicQuizController();

	public slots:

	private slots:
		/**
		 * @brief Handles the quiz states.
		 */
		void executeQuiz();

		/**
		 * @brief Quizs the quiz.
		 */
		void quitQuiz();

		/**
		 * @brief Handles quiz selected.
		 *
		 * @param[in] quizIdx The selected quiz index.
		 * @param[in] settings The quiz settings.
		 */
		void quizSelected(size_t quizIdx, const MusicQuiz::QuizSettings& settings);

		/**
		 * @brief Handles team selected.
		 *
		 * @param[in] teams The list of teams.
		 */
		void teamSelected(const std::vector<MusicQuiz::QuizTeam*>& teams);

		/**
		 * @brief Handles the intro complete event.
		 */
		void introComplete();

		/**
		 * @brief Handles the quiz complete .
		 */
		void quizCompleted(std::vector<MusicQuiz::QuizTeam*> winningTeam);

	private:

		/** Variables */
		std::string _themeSongFile = "./data/default/theme_song.mp3";
		std::string _vicatorySongFile = "./data/default/victory_song.mp3";

		MusicQuiz::QuizBoard* _quizBoard = nullptr;
		std::vector< MusicQuiz::QuizTeam* > _teams;
		MusicQuiz::QuizSelector* _quizSelector = nullptr;
		MusicQuiz::TeamSelector* _teamSelector = nullptr;
		MusicQuiz::QuizIntroScreen* _quizIntro = nullptr;
		MusicQuiz::QuizWinningScreen* _quizWinningScreen = nullptr;

		std::vector<MusicQuiz::QuizTeam*> _winningTeams;

		/** Update Timer */
		QTimer _updateTimer;
		const size_t _updateTimerDelay = 25; // ms

		/** State Variables */
		std::atomic<bool> _quizSelected = false;
		std::atomic<bool> _teamSelected = false;
		std::atomic<bool> _introScreenDone = false;
		std::atomic<bool> _gameCompleted = false;
		QuizState _quizState = QuizState::SELECT_QUIZ;

		/** Quiz Settings */
		size_t _selectedQuizIdx = 0;
		MusicQuiz::QuizSettings _settings;

		/** Audio Player */
		audio::AudioPlayer _audioPlayer;
	};
}
