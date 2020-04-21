#include "QuizEntry.hpp"

#include <stdexcept>

#include <QMouseEvent>

#include "common/Log.hpp"


MusicQuiz::QuizEntry::QuizEntry(QString audioFile, QString answer, size_t points, size_t startTime, size_t answerStartTime, size_t endTime, size_t answerEndTime, QWidget* parent) :
	_audioFile(audioFile), _answer(answer), _points(points), _startTime(startTime), _answerStartTime(answerStartTime), _endTime(endTime), _answerEndTime(answerEndTime), QPushButton(parent)
{
	/** Set Button Text */
	setText(QString::fromLocal8Bit(std::to_string(_points).c_str()));

	/** Set Start State */
	_state = EntryState::IDLE;

	/** Set Object Name */
	setObjectName("quizEntry");
}

void MusicQuiz::QuizEntry::mouseReleaseEvent(QMouseEvent* event)
{
	if ( event->button() == Qt::LeftButton ) {
		leftClickEvent();
	} else if ( event->button() == Qt::RightButton ) {
		rightClickEvent();
	}
}

void MusicQuiz::QuizEntry::leftClickEvent()
{
	switch ( _state )
	{
	case EntryState::IDLE: // Start Song
		_state = EntryState::PLAYING;
		if ( _endTime == -1 ) {
			_audioPlayer.play(_audioFile.toStdString(), _startTime);
		} else {
			_audioPlayer.play(_audioFile.toStdString(), _startTime, _endTime);
		}
		break;
	case EntryState::PLAYING: // Pause Song
		_state = EntryState::PAUSED;
		_audioPlayer.pause();
		break;
	case EntryState::PAUSED: // Play Answer
		_state = EntryState::PLAYING_ANSWER;
		if ( _answerEndTime == -1 ) {
			_audioPlayer.play(_audioFile.toStdString(), _answerStartTime);
		} else {
			_audioPlayer.play(_audioFile.toStdString(), _answerStartTime, _answerEndTime);
		}
		setText(QString::fromLocal8Bit(_answer.toStdString().c_str()));
		break;
	case EntryState::PLAYING_ANSWER: // Entry Answered
		_state = EntryState::PLAYED;
		_audioPlayer.stop();
		break;
	case QuizEntry::EntryState::PLAYED: // Play Answer Again
		if ( _answerEndTime == -1 ) {
			_audioPlayer.play(_audioFile.toStdString(), _answerStartTime);
		} else {
			_audioPlayer.play(_audioFile.toStdString(), _answerStartTime, _answerEndTime);
		}
		_state = EntryState::PLAYING_ANSWER;
		break;
	default:
		throw std::runtime_error("Unknown Quiz Entry State Encountered.");
		break;
	}
}

void MusicQuiz::QuizEntry::rightClickEvent()
{
	switch ( _state )
	{
	case EntryState::IDLE:
		break;
	case EntryState::PLAYING: // Back to initial state
		_state = EntryState::IDLE;
		_audioPlayer.pause();
		break;
	case EntryState::PAUSED: // Continue playing
		_state = EntryState::PLAYING;
		_audioPlayer.resume();
		break;
	case EntryState::PLAYING_ANSWER: // Pause Song
		_state = EntryState::PAUSED;
		_audioPlayer.pause();
		setText(QString::fromLocal8Bit(_answer.toStdString().c_str()));
		break;
	case QuizEntry::EntryState::PLAYED: // Back to idle
		_state = EntryState::IDLE;
		setText(QString::fromLocal8Bit(std::to_string(_points).c_str()));
		break;
	default:
		throw std::runtime_error("Unknown Quiz Entry State Encountered.");
		break;
	}
}