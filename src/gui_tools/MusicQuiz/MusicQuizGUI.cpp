#include "MusicQuizGUI.hpp"

#include <QMessageBox>


MusicQuiz::MusicQuizGUI::MusicQuizGUI(QWidget *this_parent) :
	QDialog(this_parent)
{
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	Ui::Dialog::setupUi(this);
}

MusicQuiz::MusicQuizGUI::~MusicQuizGUI()
{}


void MusicQuiz::MusicQuizGUI::closeEvent(QCloseEvent* event)
{
	if ( closeWindow() ) {
		event->accept();
	} else {
		event->ignore();
	}
}

bool MusicQuiz::MusicQuizGUI::closeWindow()
{
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Close Music Quiz?", "Are you sure you want to close the application?",
		QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

	if ( resBtn == QMessageBox::Yes ) {
		/** Call Destructor */
		QApplication::quit();

		return true;
	}

	return false;
}

void MusicQuiz::MusicQuizGUI::keyPressEvent(QKeyEvent* event)
{
	switch ( event->key() )
	{
	case Qt::Key_Escape:
		closeWindow();
		break;
	default:
		QWidget::keyPressEvent(event);
		break;
	}
}