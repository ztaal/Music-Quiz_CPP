#include "QuizEntry.hpp"

#include <stdexcept>

#include <QMouseEvent>

#include "common/Log.hpp"


MusicQuiz::QuizEntry::QuizEntry(std::string audioFile, QString answer, size_t points, size_t startTime, size_t answerStartTime, size_t endTime, size_t answerEndTime, QWidget* parent) :
	_audioFile(audioFile), _answer(answer), _points(points), _startTime(startTime), _answerStartTime(answerStartTime), _endTime(endTime), _answerEndTime(answerEndTime), QPushButton(parent)
{
	/** Set Button Text */
	setText(QString(_points));

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
	LOG_DEBUG("Left Click " << _answer.toStdString() << " " << _points);

	switch ( _state )
	{
	case EntryState::IDLE: // Start Song
		LOG_DEBUG("IDLE State.");
		_state = EntryState::PLAYING;
		if ( _endTime == -1 ) {
			_audioPlayer.play(_audioFile, _startTime);
		} else {
			_audioPlayer.play(_audioFile, _startTime, _endTime);
		}
		break;
	case EntryState::PLAYING: // Pause Song
		LOG_DEBUG("PLAYING State.");
		_state = EntryState::PAUSED;
		_audioPlayer.pause();
		break;
	case EntryState::PAUSED: // Play Answer
		LOG_DEBUG("PAUSED State.");
		_state = EntryState::PLAYING_ANSWER;
		if ( _answerEndTime == -1 ) {
			_audioPlayer.play(_audioFile, _answerStartTime);
		} else {
			_audioPlayer.play(_audioFile, _answerStartTime, _answerEndTime);
		}
		break;
	case EntryState::PLAYING_ANSWER: // Entry Answered
		LOG_DEBUG("PLAYING_ANSWER State.");
		_state = EntryState::PLAYED;
		_audioPlayer.stop();
		setText(_answer);
		break;
	case QuizEntry::EntryState::PLAYED: // Play Answer Again
		LOG_DEBUG("PLAYED State.");
		if ( _answerEndTime == -1 ) {
			_audioPlayer.play(_audioFile, _answerStartTime);
		} else {
			_audioPlayer.play(_audioFile, _answerStartTime, _answerEndTime);
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
	LOG_DEBUG("Right Click " << _answer.toStdString() << " " << _points);

	switch ( _state )
	{
	case EntryState::IDLE:
		LOG_DEBUG("IDLE State.");
		// \todo
		break;
	case EntryState::PLAYING: // Back to initial state
		LOG_DEBUG("PLAYING State.");
		_state = EntryState::IDLE;
		_audioPlayer.pause();
		break;
	case EntryState::PAUSED: // Continue playing
		LOG_DEBUG("PAUSED State.");
		_state = EntryState::PLAYING;
		_audioPlayer.resume();
		break;
	case EntryState::PLAYING_ANSWER: // Pause Song
		LOG_DEBUG("PLAYING_ANSWER State.");
		_state = EntryState::PAUSED;
		_audioPlayer.pause();
		setText(_answer);
		break;
	case QuizEntry::EntryState::PLAYED: // Back to playing answer
		LOG_DEBUG("PLAYED State.");
		_state = EntryState::PLAYING_ANSWER;
		if ( _answerEndTime == -1 ) {
			_audioPlayer.play(_audioFile, _answerStartTime);
		} else {
			_audioPlayer.play(_audioFile, _answerStartTime, _answerEndTime);
		}
		setText(QString(_points));
		break;
	default:
		throw std::runtime_error("Unknown Quiz Entry State Encountered.");
		break;
	}
}