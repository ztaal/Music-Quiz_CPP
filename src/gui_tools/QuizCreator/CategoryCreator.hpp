#pragma once 

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QTableWidget>

#include "audio/AudioPlayer.hpp"
#include "gui_tools/GuiUtil/QExtensions/QTabWidgetExtender.hpp"


namespace MusicQuiz {

	class CategoryCreator : public QWidget
	{
		Q_OBJECT

	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] name The category name.
		 * @param[in] audioPlayer The audio player.
		 * @param[in] parent The parent widget.
		 */
		explicit CategoryCreator(const QString& name, const std::shared_ptr< audio::AudioPlayer >& audioPlayer, QWidget* parent = nullptr);

		/**
		 * @brief Default destructor
		 */
		virtual ~CategoryCreator() = default;

	public slots:
		/**
		 * @brief Updates the category name
		 *
		 * @param[in] name The name.
		 */
		void setName(const QString& name);

	private slots:
		/**
		 * @brief Adds a entry to the category.
		 */
		void addEntry();

		/**
		 * @brief Removes a entry.
		 */
		void removeEntry();

		/**
		 * @brief Updates the name of the tab.
		 *
		 * param[in] str The new name.
		 */
		void updateEntryTabName(const QString& str);

	private:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		QString _categoryName = "";
		QLabel* _categoryNameLabel = nullptr;

		QExtensions::QTabWidgetExtender* _tabWidget = nullptr;

		QTableWidget* _entriesTable = nullptr;

		std::shared_ptr< audio::AudioPlayer > _audioPlayer = nullptr;
	};
}
