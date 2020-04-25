#pragma once

#include <string>
#include <vector>

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QListWidget>

#include "util/QuizLoader.hpp"

namespace MusicQuiz {
	class QuizSelector : public QWidget
	{
		Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] parent The parent widget.
		 */
		explicit QuizSelector(QWidget* parent = nullptr);

		/**
		 * @brief Destructor
		 */
		virtual ~QuizSelector() = default;

	public slots:

	private slots:
		/**
		 * @brief Updates the displayed information based on the quiz that is clicked.
		 *
		 * @param[in] idx The quiz index.
		 */
		void selectionClicked();

		/**
		 * @brief Emits the quiz selected signal.
		 */
		void quizSelected();

		/**
		 * @brief Emits the quit signal.
		 */
		void quit();

	signals:
		void quitSignal();
		void quizSelectedSignal(const std::string& quiz);
	protected:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		QTextEdit* _descriptionText = nullptr;
		QListWidget* _quizSelectionList = nullptr;

		std::vector<std::string> _quizList;
		std::vector<MusicQuiz::util::QuizLoader::QuizPreview> _quizPreviews;
	};
}
