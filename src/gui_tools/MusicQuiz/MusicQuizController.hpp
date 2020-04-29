#pragma once 

#include <atomic>

#include <QtGui>
#include <QTimer>
#include <QtCore>
#include <QWidget>
#include <QKeyEvent>
#include <QCloseEvent>

#include "ui_MusicQuizGUI.h"

#include "util/QuizSettings.hpp"
#include "widgets/QuizBoard.hpp"
#include "widgets/QuizFactory.hpp"
#include "gui_tools/GuiUtil/QuizSelector.hpp"


namespace MusicQuiz {
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
		/** 
		 * @brief Handles the close event.
		 *
		 * @param[in] event The event.
		 */
		void closeEvent(QCloseEvent* event);

		/**
		 * @brief Handles the key press events.
		 *
		 * @param[in] event The event.
		 */
		void keyPressEvent(QKeyEvent* event);

	private slots:
		/**
		 * @brief Handles the quiz states.
		 */
		void executeQuiz();

		/**
		 * @brief Closes the window.
		 *
		 * @return True if the window should be closed.
		 */
		bool closeWindow();

		/**
		 * @brief Handles quiz selected.
		 *
		 * @param[in] quizIdx The selected quiz index.
		 * @param[in] settings The quiz settings.
		 */
		void quizSelected(size_t quizIdx, const MusicQuiz::QuizSettings& settings);

	private:

		/** Variables */
		MusicQuiz::QuizBoard* _quizBoard = nullptr;
		std::vector< MusicQuiz::QuizTeam* > _teams;
		MusicQuiz::QuizSelector* _quizSelector = nullptr;

		/** Update Timer */
		QTimer _updateTimer;
		const size_t _updateTimerDelay = 25; // ms

		/** State Variables */
		std::atomic<bool> _quizSelected = false;
		std::atomic<bool> _teamSelected = true;
		std::atomic<bool> _introScreenDone = true;
		std::atomic<bool> _gameCompleted = false;
		QuizState _quizState = QuizState::SELECT_QUIZ;

		/** Quiz Settings */
		size_t _selectedQuizIdx = 0;
		MusicQuiz::QuizSettings _settings;
	};
}
