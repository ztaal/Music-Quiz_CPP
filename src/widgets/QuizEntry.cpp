#include "QuizEntry.hpp"

#include <stdexcept>

#include <QMouseEvent>
#include <QSizePolicy>

#include "common/Log.hpp"


MusicQuiz::QuizEntry::QuizEntry(const QString &audioFile, const QString &answer, const size_t points, const size_t startTime, const size_t answerStartTime, const size_t endTime, const size_t answerEndTime, QWidget* parent) :
	QPushButton(parent), _audioFile(audioFile), _answer(answer), _points(points), _startTime(startTime), _answerStartTime(answerStartTime), _endTime(endTime), _answerEndTime(answerEndTime)
{
	/** Set Button Text */
	setText(QString::fromLocal8Bit(std::to_string(_points).c_str()));

	/** Set Start State */
	_state = EntryState::IDLE;

	/** Set Object Name */
	setObjectName("QuizEntry");

	/** Set Size Policy */
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MusicQuiz::QuizEntry::mouseReleaseEvent(QMouseEvent* event)
{
	/** Handle Event */
	if ( event->button() == Qt::LeftButton ) {
		leftClickEvent();
	} else if ( event->button() == Qt::RightButton ) {
		rightClickEvent();
	}

	/** Set Object Name (this changes the color) */
	switch ( _state )
	{
	case MusicQuiz::QuizEntry::EntryState::IDLE:
		setStyleSheet("background-color	: rgb(0, 0, 205)");
		break;
	case MusicQuiz::QuizEntry::EntryState::PLAYING:
		setStyleSheet("background-color	: rgb(0, 0, 139)");
		break;
	case MusicQuiz::QuizEntry::EntryState::PAUSED:
		setStyleSheet("background-color	: rgb(255, 215, 0)");
		break;
	case MusicQuiz::QuizEntry::EntryState::PLAYING_ANSWER:
		setStyleSheet("background-color	: rgb(0, 128, 0)");
		break;
	case MusicQuiz::QuizEntry::EntryState::PLAYED:
		setStyleSheet("background-color	: rgb(0, 0, 205)");
		break;
	default:
		break;
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