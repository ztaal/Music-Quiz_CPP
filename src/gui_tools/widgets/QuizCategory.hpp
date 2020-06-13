#pragma once

#include <vector>
#include <memory>

#include <QString>
#include <QObject>
#include <QWidget>


namespace MusicQuiz {
	class QuizEntry;
	class QuizCategory : public QWidget
	{
		Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] name The category name.
		 * @param[in] entries The category entries.
		 * @param[in] parent The parent widget.
		 */
		explicit QuizCategory(QString name, const std::vector<MusicQuiz::QuizEntry*>& entries, QWidget* parent = nullptr);

		/**
		 * @brief Default Destructor
		 */
		virtual ~QuizCategory() = default;

		/**
		 * @brief Deleted the copy and assignment constructor.
		 */
		QuizCategory(const QuizCategory&) = delete;
		QuizCategory& operator=(const QuizCategory&) = delete;

		/**
		 * @brief Gets the size of the category.
		 *
		 * @return The size of the category.
		 */
		size_t size();

		/** Overload [] operator */
		MusicQuiz::QuizEntry*& operator[](int index)
		{
			return _entries[index];
		};
	public slots:

	protected:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		QString _name = "";
		std::vector<MusicQuiz::QuizEntry*> _entries;
	};
}