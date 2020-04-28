#pragma once

#include <string>
#include <vector>

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QCheckBox>

#include "widgets/QuizFactory.hpp"

namespace MusicQuiz {
	class QuizSettingsDialog : public QDialog
	{
		Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] settings The quiz settings.
		 * @param[in] parent The parent widget.
		 */
		explicit QuizSettingsDialog(MusicQuiz::QuizFactory::QuizSettings settings, QWidget* parent = nullptr);

		/**
		 * @brief Destructor
		 */
		virtual ~QuizSettingsDialog() = default;

	public slots:
		/**
		 * @brief Handles the key press events.
		 *
		 * @param[in] event The event.
		 */
		void keyPressEvent(QKeyEvent* event);

	private slots:
		/**
		 * @brief Emits the quit signal.
		 */
		void quit();

		/**
		 * @brief Show a popup information message box.
		 *
		 * @param[in] info The information to be displayed within the message box.
		 */
		void informationMessageBox(const QString& info);

		/**
		 * @brief Display Information boxes for the different settings.
		 */
		void showHiddenTeamsInfo();
		void showHiddenAnswersInfo();

	signals:
		void quitSignal();
	protected:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		MusicQuiz::QuizFactory::QuizSettings _settings;

		/** Hidden Teams */
		QCheckBox* _hiddenTeam = nullptr;
		QCheckBox* _hiddenAnswers = nullptr;
	};
}
