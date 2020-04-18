#include "JeopardyGUI.hpp"


jeopardy::JeopardyGUI::JeopardyGUI(QWidget *this_parent) :
	QDialog(this_parent)
{
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	Ui::Dialog::setupUi(this);
}

jeopardy::JeopardyGUI::~JeopardyGUI()
{}
