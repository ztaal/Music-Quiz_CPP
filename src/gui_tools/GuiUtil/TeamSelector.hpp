#pragma once

#include <string>
#include <vector>

#include <QColor>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QKeyEvent>
#include <QListWidget>
#include <QPushButton>
#include <QTableWidget>

#include "gui_tools/widgets/QuizTeam.hpp"
#include "gui_tools/GuiUtil/ColorPicker/QHueSlider.hpp"


namespace MusicQuiz {
	class TeamSelector : public QDialog
	{
		Q_OBJECT
	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] parent The parent widget.
		 */
		explicit TeamSelector(QWidget* parent = nullptr);

		/**
		 * @brief Destructor
		 */
		virtual ~TeamSelector() = default;

	public slots:
		/**
		 * @brief Handles the key press events.
		 *
		 * @param[in] event The event.
		 */
		void keyPressEvent(QKeyEvent* event);

	private slots:
		/**
		 * @brief Emits the quiz selected signal.
		 */
		void teamSelected();

		/**
		 * @brief Adds a team to the team table.
		 */
		void addTeam();

		/**
		 * @brief Removes the selected team from the team table.
		 */
		void removeTeam();

		/**
		 * @brief Updates the color indicator.
		 *
		 * @param[in] color The selected color.
		 */
		void teamColorChanged(QColor color);

		/**
		 * @brief Emits the quit signal.
		 */
		void quit();

	signals:
		void quitSignal();
		void teamSelectedSignal(const std::vector<MusicQuiz::QuizTeam*>&);
	protected:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/** Variables */
		QTableWidget* _teamTable = nullptr;
		QLineEdit* _teamNameLineEdit = nullptr;
		QColor _currentColor = QColor(0, 0, 255);

		ColorPicker::QHueSlider* _hueSlider = nullptr;
	};
}
