#include "AudioPlayer.hpp"

#include <stdexcept>

#include <QVBoxLayout>
#include <QMediaContent>

#include "common/Log.hpp"


media::AudioPlayer::AudioPlayer(QWidget* parent) :
	QWidget(parent)
{
	/** Create Media Player */
	_player = new QMediaPlayer(this);
	_player->setVolume(100);
}

media::AudioPlayer::~AudioPlayer()
{
	/** Stop Audio */
	_player->stop();
}

void media::AudioPlayer::play(const QString& audioFile)
{
	/** Sanity Check */
	if ( audioFile.isEmpty() ) {
		throw std::runtime_error("Video File Name is empty.");
	}

	/** Stop audio if any is playing and close file */
	stop();

	/** Set Audio File */
	_player->setMedia(QUrl::fromLocalFile(audioFile));

	/** Play Video */
	_player->play();

	/** Set State */
	_state = AudioPlayState::PLAYING;
}

void media::AudioPlayer::play(const QString& audioFile, const size_t startTime)
{
	/** Sanity Check */
	if ( audioFile.isEmpty() ) {
		throw std::runtime_error("Video File Name is empty.");
	}

	/** Stop audio if any is playing and close file */
	stop();

	/** Set Audio File */
	_player->setMedia(QUrl::fromLocalFile(audioFile));

	/** Set Start Time */
	_player->setPosition(startTime);

	/** Play Audio */
	_player->play();

	/** Set State */
	_state = AudioPlayState::PLAYING;
}

void media::AudioPlayer::pause()
{
	/** Check State */
	if ( _state != AudioPlayState::PLAYING ) {
		return;
	}

	/** Pause Video */
	_player->pause();

	/** Set State */
	_state = AudioPlayState::PAUSED;
}

void media::AudioPlayer::resume()
{
	/** Check State */
	if ( _state != AudioPlayState::PAUSED ) {
		return;
	}

	/** Resume Audio */
	_player->play();

	/** Set State */
	_state = AudioPlayState::PLAYING;
}

void media::AudioPlayer::stop()
{
	/** Stop Video */
	_player->stop();
	_player->setMedia(QMediaContent());

	/** Set State */
	_state = AudioPlayState::IDLE;
}