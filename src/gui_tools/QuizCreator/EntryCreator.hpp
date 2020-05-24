#pragma once 

#include <vector>

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QSpinBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QTableWidget>


namespace MusicQuiz {

	class EntryCreator : public QWidget
	{
		Q_OBJECT

	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] name The entry name.
		 * @param[in] points The points of the entry.
		 * @param[in] parent The parent widget.
		 */
		explicit EntryCreator(const QString& name, size_t points, QWidget* parent = nullptr);

		/**
		 * @brief Default destructor
		 */
		virtual ~EntryCreator() = default;

	public slots:
		/**
		 * @brief Updates the category name
		 *
		 * @param[in] name The name.
		 */
		void setName(const QString& name);

	private slots:
		/**
		 * @brief Opens a dialog to browse for a song file;
		 */
		void browseSong();

		/**
		 * @brief Opens a dialog to browse for a video file;
		 */
		void browseVideo();

	private:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		size_t _points = 0;

		QString _entryName;
		QLabel* _entryNameLabel = nullptr;

		QSpinBox* _pointsSpinbox = nullptr;
		QLineEdit* _answerLineEdit = nullptr;

		QLineEdit* _songFileLineEdit = nullptr;
		QLineEdit* _videoFileLineEdit = nullptr;
	};
}
