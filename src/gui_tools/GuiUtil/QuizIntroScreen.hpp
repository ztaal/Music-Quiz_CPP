#pragma once

#include <string>
#include <vector>

#include <QTimer>
#include <QColor>
#include <QObject>
#include <QWidget>
#include <QDialog>


namespace MusicQuiz {
	class QuizIntroScreen : public QDialog
	{
		Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] parent The parent widget.
		 */
		explicit QuizIntroScreen(QWidget* parent = nullptr);

		/**
		 * @brief Destructor
		 */
		virtual ~QuizIntroScreen();

	public slots:

	private slots:
		/**
		 * @brief Emits the introCompleteSignal signal
		 */
		void introComplete();

	signals:
		void introCompleteSignal();
	protected:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		QTimer _timer;
		const size_t _introDisplayTime = 5000; // in ms
	};
}
