#include "MusicQuizGUI.hpp"


MusicQuiz::MusicQuizGUI::MusicQuizGUI(QWidget *this_parent) :
	QDialog(this_parent)
{
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	Ui::Dialog::setupUi(this);
}

MusicQuiz::MusicQuizGUI::~MusicQuizGUI()
{}
