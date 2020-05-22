#include "QuizWinningScreen.hpp"

#include <QColor>
#include <QLabel>
#include <QWidget>
#include <QString>
#include <QGridLayout>


MusicQuiz::QuizWinningScreen::QuizWinningScreen(const std::vector<MusicQuiz::QuizTeam*>& winningTeams, QWidget* parent) :
	QDialog(parent), _winningTeams(winningTeams)
{
	/** Sanity Check */
	if ( _winningTeams.empty() ) {
		screenComplete();
	}

	/** Set Window Flags */
	setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::MSWindowsFixedSizeDialogHint);

	/** Set Object Name */
	setObjectName("QuizWinningScreen"); 

	/** Create Layout */
	createLayout();

	/** Set Fullscreen */
	showFullScreen();

	/** Set Fixed Size */
	setFixedSize(this->size());

	/** Connect timer */
	connect(&_timer, SIGNAL(timeout()), this, SLOT(screenComplete()));
	connect(&_sizeTimer, SIGNAL(timeout()), this, SLOT(increaseTextSize()));

	/** Start Timer */
	if ( !_timer.isActive() ) {
		_timer.start(_winnerDisplayTime);
	}

	if ( !_sizeTimer.isActive() ) {
		_sizeTimer.start(20);
	}
}

MusicQuiz::QuizWinningScreen::~QuizWinningScreen()
{
	/** Stop Timer */
	if ( !_timer.isActive() ) {
		_timer.stop();
	}
}

void MusicQuiz::QuizWinningScreen::createLayout()
{

	/** Layout */
	QGridLayout* mainlayout = new QGridLayout;
	mainlayout->setRowStretch(0, 1);
	mainlayout->setRowStretch(1, 5);
	size_t row = 0;

	/** Text Label */
	QLabel* textLabel = new QLabel("The Winner!");
	textLabel->setObjectName("winningScreenTextLabel");
	mainlayout->addWidget(textLabel, 0, 0, Qt::AlignCenter);

	/** Winners Label */
	_winnersLabel = new QLabel;
	QString winnersStr = _winningTeams[0]->getName();
	for ( size_t i = 1; i < _winningTeams.size(); ++i ) {
		winnersStr += " & " + _winningTeams[i]->getName();
	}
	_winnersLabel->setText(winnersStr);
	mainlayout->addWidget(_winnersLabel, 1, 0, Qt::AlignCenter);

	/** Set Layout */	
	setLayout(mainlayout);
	setContentsMargins(20, 20, 20, 20);
}

void MusicQuiz::QuizWinningScreen::increaseTextSize()
{
	/** Sanity Check */
	if ( _winnersLabel == nullptr ) {
		return;
	}

	/** Increase Size */
	if ( _textSize < 1200 ) {
		++_textSize;
	}

	/** Choose Random Color */
	_textColor.setHsv(_hueCounter, _textColor.saturation(), _textColor.value());
	++_hueCounter;
	if ( _hueCounter > 200 && _hueCounter < 270 ) { // Skip Blue
		_hueCounter = 270;
	} else if ( _hueCounter >= 359 ) { // Loop Color
		_hueCounter = 0;
	}

	/** Set Stylesheet */
	_winnersLabel->setStyleSheet("font-size: " + QString::number(_textSize / 4) + "px; font-weight: bold;" +
		"color: rgb(" + QString::number(_textColor.red()) + "," + QString::number(_textColor.green()) + "," + QString::number(_textColor.blue()) + ");");
}

void MusicQuiz::QuizWinningScreen::screenComplete()
{
	emit winningScreenCompleteSignal();
}