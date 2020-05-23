#include "QuizCreator.hpp"

#include <QLabel>
#include <QString>
#include <QGridLayout>


MusicQuiz::QuizCreator::QuizCreator(QWidget* parent) :
	QDialog(parent)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::Window | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Set Window Title */
	setWindowTitle("Quiz Creator");

	/** Set Object Name */
	setObjectName("QuizCreator");

	/** Create Layout */
	createLayout();
}

void MusicQuiz::QuizCreator::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;

	/** Set Layout */
	setLayout(mainlayout);
	setContentsMargins(20, 20, 20, 20);
}