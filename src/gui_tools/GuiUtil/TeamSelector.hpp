#pragma once

#include <string>
#include <vector>

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QKeyEvent>
#include <QListWidget>

#include "gui_tools/widgets/QuizTeam.hpp"


namespace MusicQuiz {
	class TeamSelector : public QDialog
	{
		Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] parent The parent widget.
		 */
		explicit TeamSelector(QWidget* parent = nullptr);

		/**
		 * @brief Destructor
		 */
		virtual ~TeamSelector() = default;

	public slots:
		/**
		 * @brief Handles the key press events.
		 *
		 * @param[in] event The event.
		 */
		void keyPressEvent(QKeyEvent* event);

	private slots:
		/**
		 * @brief Emits the quiz selected signal.
		 */
		void teamSelected();

		/**
		 * @brief Emits the quit signal.
		 */
		void quit();

	signals:
		void quitSignal();
		void teamSelectedSignal(const std::vector<MusicQuiz::QuizTeam*>&);
	protected:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		QLineEdit* _authorText = nullptr;
	};
}
