#pragma once 

#include <atomic>

#include <QtGui>
#include <QTimer>
#include <QtCore>
#include <QWidget>
#include <QKeyEvent>
#include <QCloseEvent>

#include "ui_MusicQuizGUI.h"

#include "widgets/QuizBoard.hpp"
#include "gui_tools/GuiUtil/QuizSelector.hpp"


namespace MusicQuiz {
	class MusicQuizController : public QWidget
	{
		Q_OBJECT

	public:
		enum QuizState
		{
			INIT = 0,
			SELECTING_QUIZ = 1,
			SELECTING_TEAM = 2,
			QUIZ_RUNNING   = 3,
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
		 */
		void quizSelected(size_t quizIdx);

	private:

		/** Variables */
		QTimer _updateTimer;
		const size_t _updateTimerDelay = 25; // ms

		std::atomic<bool> _quizSelected = false;
		std::atomic<bool> _teamSelected = true;
		std::atomic<bool> _gameCompleted = false;
		QuizState _quizState = QuizState::INIT;

		MusicQuiz::QuizBoard* _quizBoard = nullptr;
		MusicQuiz::QuizSelector* _quizSelector = nullptr;

		size_t _selectedQuizIdx = 0;

	};
}
