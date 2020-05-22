#pragma once

#include <vector>

#include <QLabel>
#include <QTimer>
#include <QColor>
#include <QObject>
#include <QWidget>
#include <QDialog>

#include "gui_tools/widgets/QuizTeam.hpp"


namespace MusicQuiz {
	class QuizWinningScreen : public QDialog
	{
		Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] parent The parent widget.
		 */
		explicit QuizWinningScreen(const std::vector<MusicQuiz::QuizTeam*>& winningTeams, QWidget* parent = nullptr);

		/**
		 * @brief Destructor
		 */
		virtual ~QuizWinningScreen();

	public slots:

	private slots:
		/**
		 * @brief Increase the size of the team name.
		 */
		void increaseTextSize();

		/**
		 * @brief Emits the winningScreenCompleteSignal signal
		 */
		void screenComplete();

	signals:
		void winningScreenCompleteSignal();

	protected:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		size_t _textSize = 1;
		size_t _hueCounter = 0;
		QColor _textColor = QColor(255, 255, 0);

		QTimer _timer;
		QTimer _sizeTimer;
		const size_t _winnerDisplayTime = 150000; // in ms

		std::vector< QLabel* > _andLabels;
		std::vector< QLabel* > _winnersLabels;
		std::vector<MusicQuiz::QuizTeam*> _winningTeams;
	};
}
