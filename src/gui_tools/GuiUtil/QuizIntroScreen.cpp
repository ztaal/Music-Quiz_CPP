#include "QuizIntroScreen.hpp"

#include <QColor>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>


MusicQuiz::QuizIntroScreen::QuizIntroScreen(QWidget* parent) :
	QDialog(parent)
{
	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);

	/** Set Object Name */
	setObjectName("QuizIntroScreen");

	/** Create Layout */
	createLayout();

	/** Set Fullscreen */
	showFullScreen();

	/** Connect timer */
	connect(&_timer, SIGNAL(timeout()), this, SLOT(introComplete()));

	/** Start Timer */
	if ( !_timer.isActive() ) {
		_timer.start(_introDisplayTime);
	}
}

MusicQuiz::QuizIntroScreen::~QuizIntroScreen()
{
	/** Stop Timer */
	if ( !_timer.isActive() ) {
		_timer.stop();
	}
}

void MusicQuiz::QuizIntroScreen::createLayout()
{
	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;

	/** Quiz Label */
	QLabel* quizLabel = new QLabel("Music Quiz"); 
	quizLabel->setObjectName("quizIntroLabel");
	mainlayout->addWidget(quizLabel, 0, 0, Qt::AlignCenter);

	/** Set Layout */
	setLayout(mainlayout);
	setContentsMargins(20, 20, 20, 20);
}

void MusicQuiz::QuizIntroScreen::introComplete()
{
	emit introCompleteSignal();
}