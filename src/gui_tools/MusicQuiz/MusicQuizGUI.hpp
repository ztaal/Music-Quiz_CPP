#pragma once 

#include <QtGui>
#include <QtCore>
#include <QDialog>

#include "ui_MusicQuizGUI.h"


namespace MusicQuiz {
	class MusicQuizGUI : public QDialog, public Ui::Dialog
	{
		Q_OBJECT

	public:
		explicit MusicQuizGUI(QWidget *parent = nullptr);
		virtual ~MusicQuizGUI();

	public slots:
	public:
	};
}
