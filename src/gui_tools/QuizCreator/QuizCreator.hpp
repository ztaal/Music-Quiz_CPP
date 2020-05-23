#pragma once

#include <vector>

#include <QObject>
#include <QWidget>
#include <QDialog>


namespace MusicQuiz {
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

	signals:

	protected:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
	};
}
