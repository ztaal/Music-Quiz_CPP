#include "VideoPlayer.hpp"

#include <stdexcept>

#include <QVBoxLayout>

#include "common/Log.hpp"


media::VideoPlayer::VideoPlayer(QWidget* parent) :
		QWidget(parent)
{
	/** Create Video Widget */
	QVBoxLayout* layout = new QVBoxLayout;
	_videoWidget = new QVideoWidget(this);
	layout->addWidget(_videoWidget);
	_videoWidget->show();
	
	/** Create Media Player */
	_player = new QMediaPlayer(this);
	_player->setVideoOutput(_videoWidget);
	_player->setVolume(100);
}

media::VideoPlayer::~VideoPlayer()
{
	/** Stop Video */
	_player->stop();
}

void media::VideoPlayer::play(const QString& videoFile, bool muted)
{
	/** Sanity Check */
	if ( videoFile.isEmpty() ) {
		throw std::runtime_error("Video File Name is empty.");
	}

	/** Stop audio if any is playing and close file */
	stop();

	/** Set Video File */
	_player->setMedia(QUrl::fromLocalFile(videoFile));

	/** Set Volume */
	if ( muted ) {
		_player->setVolume(0);
	} else {
		_player->setVolume(100);
	}

	/** Play Video */
	_player->play();

	/** Set State */
	_state = VideoPlayState::PLAYING;
}

void media::VideoPlayer::play(const QString& videoFile, const size_t startTime, bool muted)
{
	/** Sanity Check */
	if ( videoFile.isEmpty() ) {
		throw std::runtime_error("Video File Name is empty.");
	}

	/** Stop audio if any is playing and close file */
	stop();

	/** Set Video File */
	_player->setMedia(QUrl::fromLocalFile(videoFile));

	/** Set Volume */
	if ( muted ) {
		_player->setVolume(0);
	} else {
		_player->setVolume(100);
}

	/** Set Start Time */
	_player->setPosition(startTime);

	/** Play Video */
	_player->play();

	/** Set State */
	_state = VideoPlayState::PLAYING;
}

void media::VideoPlayer::pause()
{
	/** Check State */
	if ( _state != VideoPlayState::PLAYING ) {
		return;
	}

	/** Pause Video */
	_player->pause();

	/** Set State */
	_state = VideoPlayState::PAUSED;
}

void media::VideoPlayer::resume()
{
	/** Check State */
	if ( _state != VideoPlayState::PAUSED ) {
		return;
	}

	/** Resume audio */
	_player->play();

	/** Set State */
	_state = VideoPlayState::PLAYING;
}

void media::VideoPlayer::stop()
{
	/** Stop Video */
	_player->stop();

	/** Set State */
	_state = VideoPlayState::IDLE;
}

void media::VideoPlayer::resize(const QSize& size)
{
	/** Resize the video widget */
	if ( _videoWidget != nullptr ) {
		_videoWidget->resize(size);
	}
}

void media::VideoPlayer::mousePressEvent(QMouseEvent* event)
{
	if ( _mouseEventCallback ) {
		_mouseEventCallback(event);
	}
	event->accept();
}

void media::VideoPlayer::keyPressEvent(QKeyEvent* event)
{
	/** Hide Video if esc is pressed */
	if ( event->key() == Qt::Key_Escape ) {
		hide();
	}

	event->accept();
}

void media::VideoPlayer::setMouseEventCallbackFunction(const std::function< void(QMouseEvent*) > mouseEventCallback)
{
	_mouseEventCallback = mouseEventCallback;
}