#pragma once 

#include <QtGui>
#include <QtCore>
#include <QDialog>

#include "ui_JeopardyGUI.h"


namespace jeopardy {
	class JeopardyGUI : public QDialog, public Ui::Dialog
	{
		Q_OBJECT

	public:
		explicit JeopardyGUI(QWidget *parent = nullptr);
		virtual ~JeopardyGUI();

	public slots:
	public:
	};
}
