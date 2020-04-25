#pragma once 

#include <QtGui>
#include <QtCore>
#include <QDialog>
#include <QKeyEvent>
#include <QCloseEvent>

#include "ui_MusicQuizGUI.h"


namespace MusicQuiz {
	class MusicQuizGUI : public QDialog, public Ui::Dialog
	{
		Q_OBJECT

	public:
		explicit MusicQuizGUI(QWidget *parent = nullptr);
		virtual ~MusicQuizGUI();

	public slots:

		/** 
		 * @brief Handles the close event.
		 *
		 * @param[in] event The event.
		 */
		void closeEvent(QCloseEvent* event);

		/**
		 * @brief Handles the key press events.
		 *
		 * @param[in] event The event.
		 */
		void keyPressEvent(QKeyEvent* event);

	private slots:
		/**
		 * @brief Closes the window.
		 *
		 * @return True if the window should be closed.
		 */
		bool closeWindow();

	public:


	};
}
