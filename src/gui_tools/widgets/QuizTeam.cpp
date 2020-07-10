#include "QuizTeam.hpp"

#include <sstream>
#include <stdlib.h>
#include <stdexcept>

#include <QMouseEvent>

#include "common/Log.hpp"


MusicQuiz::QuizTeam::QuizTeam(const QString& name, const QColor& color, QWidget* parent) :
	QPushButton(parent), _name(name), _score(0), _color(color), _newPoints(0),
	_scoreCntRate(1), _scoreTimerDelayMs(25), _hideScore(false)
{
	/** Sanity Check */
	if ( _name.isEmpty() ) {
		throw std::runtime_error("Cannot create team without a name.");
	}

	/** Set Team Text */
	QString str = _name + ": " + QString::fromLocal8Bit(std::to_string(_score).c_str()) + "$";
	setText(str);

	/** Set Background Color */
	std::stringstream ss;
	ss << "background-color	: rgb(" << _color.red() << ", " << _color.green() << ", " << _color.blue() << ");";

	/** Set Text Color to inverted button color */
	ss << "color : rgb(" << 255 - _color.red() << ", " << 255 - _color.green() << ", " << 255 - _color.blue() << ");";
	setStyleSheet(QString::fromStdString(ss.str()));

	/** Set Object Name */
	setObjectName("TeamEntry");

	/** Connect timer */
	connect(&_scoreCntTimer, SIGNAL(timeout()), this, SLOT(accumulateScore()));
}

MusicQuiz::QuizTeam::~QuizTeam()
{
	/** Stop Timer */
	if ( !_scoreCntTimer.isActive() ) {
		_scoreCntTimer.stop();
	}
}

void MusicQuiz::QuizTeam::setHideScore(bool hide)
{
	_hideScore = hide;

	/** Update Name */
	QString str = _name + (_hideScore ? "" : ": " + QString::fromLocal8Bit(std::to_string(_score).c_str()) + "$");
	setText(str);
}

void MusicQuiz::QuizTeam::addPoints(size_t points)
{
	/** Update Score */
	if ( !_hideScore ) {
		_newPoints += points;
		_scoreCntRate = _newPoints / _scoreTimerDelayMs;
	} else {
		_score += points;
	}

	if ( !_scoreCntTimer.isActive() ) {
		_scoreCntTimer.start(_scoreTimerDelayMs);
	}
}

void MusicQuiz::QuizTeam::accumulateScore()
{
	/** Get random number to add to score */
	size_t val = rand() % _scoreCntRate + 1;
	if ( val > _newPoints ) {
		val = _newPoints;
	}

	/** Add Score */
	_score += val;
	_newPoints -= val;

	/** Update Text */
	QString str = _name + (_hideScore ? "" : ": " + QString::fromLocal8Bit(std::to_string(_score).c_str()) + "$");
	setText(str);

	/** Check if points have been added */
	if ( _newPoints <= 0 ) {
		_scoreCntTimer.stop();
	}
}

QString MusicQuiz::QuizTeam::getName() const
{
	return _name;
}

size_t MusicQuiz::QuizTeam::getScore() const
{
	return _score + _newPoints;
}

QColor MusicQuiz::QuizTeam::getColor() const
{
	return _color;
}