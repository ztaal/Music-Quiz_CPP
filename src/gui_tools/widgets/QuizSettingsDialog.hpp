#pragma once

#include <string>
#include <vector>

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QCheckBox>
#include <QKeyEvent>

#include "util/QuizSettings.hpp"


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
		explicit QuizSettingsDialog(const MusicQuiz::QuizSettings& settings, QWidget* parent = nullptr);

		/**
		 * @brief Destructor
		 */
		virtual ~QuizSettingsDialog() = default;

		/**
		 * @brief Deleted the copy and assignment constructor.
		 */
		QuizSettingsDialog(const QuizSettingsDialog&) = delete;
		QuizSettingsDialog& operator=(const QuizSettingsDialog&) = delete;

	public slots:
		/**
		 * @brief Handles the key press events.
		 *
		 * @param[in] event The event.
		 */
		void keyPressEvent(QKeyEvent* event);

	private slots:
		/**
		 * @brief Closes the dialog and emits a signal with the settings.
		 */
		void saveSettings();
		
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
		void showDailyDoubleInfo();
		void showDailyTripleInfo();
		void showDailyDoubleHiddenInfo();
		void showDailyTripleHiddenInfo();

	signals:
		void quitSignal();
		void settingsUpdated(MusicQuiz::QuizSettings settings);
	protected:
		/**
		 * @brief Creates the category layout.
		 *
		 * @param[in] settings The currently set settings.
		 */
		void createLayout(const MusicQuiz::QuizSettings& settings);

		/** Variables */
		QCheckBox* _hiddenTeam = nullptr;
		QCheckBox* _hiddenAnswers = nullptr;
		QCheckBox* _dailyDouble = nullptr;
		QCheckBox* _dailyTriple = nullptr;
		QCheckBox* _dailyDoubleHidden = nullptr;
		QCheckBox* _dailyTripleHidden = nullptr;
	};
}
