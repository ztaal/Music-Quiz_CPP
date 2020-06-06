#pragma once

#include <vector>

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QTabWidget>
#include <QTableWidget>

#include "media/AudioPlayer.hpp"
#include "media/VideoPlayer.hpp"


namespace MusicQuiz {
	class QuizBoard;
	class CategoryCreator;

	class QuizCreator : public QDialog
	{
		Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] parent The parent widget.
		 */
		explicit QuizCreator(QWidget* parent = nullptr);

		/**
		 * @brief Default destructor
		 */
		virtual ~QuizCreator() = default;

	public slots:

	private slots:
		/**
		 * @brief Adds a category to the quiz.
		 */
		void addCategory();

		/**
		 * @brief Goes to the category tab.
		 */
		void editCategory();

		/**
		 * @brief Adds a row category to the quiz.
		 */
		void addRowCategory();

		/**
		 * @brief Removes a category.
		 */
		void removeCategory();

		/**
		 * @brief Removes a row category.
		 */
		void removeRowCategory();
		
		/**
		 * @brief Updates the name of the tab.
		 *
		 * param[in] str The new name.
		 */
		void updateCategoryTabName(const QString& str);

		/**
		 * @brief Saves the quiz.
		 */
		void saveQuiz();

		/**
		 * @brief Previews the quiz.
		 */
		void previewQuiz();

		/**
		 * @brief Stops the quiz preview.
		 */
		void stopQuizPreview();

		/**
		 * @brief Quits the quiz creator.
		 */
		void quitCreator();

	signals:

	protected:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		std::string quizSavedName = "";

		QTabWidget* _tabWidget = nullptr;

		QLineEdit* _quizNameLineEdit = nullptr;
		QTextEdit* _quizDescriptionTextEdit = nullptr;
		
		QCheckBox* _hiddenCategoriesCheckbox = nullptr;

		QTableWidget* _categoriesTable = nullptr;
		QTableWidget* _rowCategoriesTable = nullptr;

		std::vector< MusicQuiz::CategoryCreator* > _categories;

		/** Audio Player */
		std::shared_ptr<media::AudioPlayer> _audioPlayer = nullptr;
		std::shared_ptr<media::VideoPlayer> _videoPlayer = nullptr;

		/** Preview Quiz Board */
		MusicQuiz::QuizBoard* _previewQuizBoard = nullptr;
	};
}
