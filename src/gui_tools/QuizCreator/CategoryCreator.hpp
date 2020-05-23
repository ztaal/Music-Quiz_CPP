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


namespace MusicQuiz {
	class CategoryCreator : public QWidget
	{
		Q_OBJECT

	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] parent The parent widget.
		 */
		explicit CategoryCreator(QWidget* parent = nullptr);

		/**
		 * @brief Default destructor
		 */
		virtual ~CategoryCreator() = default;

	public slots:

	private slots:

	private:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
	};
}
