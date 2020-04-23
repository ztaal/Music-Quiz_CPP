#pragma once

#include <vector>
#include <string>
#include <memory>

#include <QColor>
#include <QString>
#include <QObject>
#include <QWidget>


namespace MusicQuiz {
	class QuizTeam;
	class QuizCategory;
	class QuizBoard : public QWidget
	{
		Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] categories The categories.
		 * @param[in] rowCategories The row categories.
		 * @param[in] teams The list of teams.
		 * @param[in] parent The parent widget.
		 */
		explicit QuizBoard(const std::vector<MusicQuiz::QuizCategory*> &categories, const std::vector<QString> &rowCategories,
			const std::vector<MusicQuiz::QuizTeam*> &teams, QWidget* parent = nullptr);

		/**
		 * @brief Default Destructor
		 */
		virtual ~QuizBoard() = default;

	public slots:
		/**
		 * @brief Handle answer.
		 *
		 * @param[in] points The points from the entry.
		 */
		void handleAnswer(size_t points);
	protected:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		QString _name = "";

		std::vector<QuizTeam*> _teams;
		std::vector<QString> _rowCategories;
		std::vector<MusicQuiz::QuizCategory*> _categories;
	};
}
