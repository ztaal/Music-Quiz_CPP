#pragma once

#include <QString>
#include <QWidget>
#include <QObject>
#include <QMediaPlayer>
#include <QVideoWidget>


namespace media {
	class VideoPlayer : public QWidget
	{
		Q_OBJECT
	public:
		enum class VideoPlayState
		{
			IDLE = 1,		// Default
			PLAYING = 2,
			PAUSED = 3
		};

		/**
		 * @brief Constructor
		 *
		 * @param[in] parent The parent widget.
		 */
		explicit VideoPlayer(QWidget* parent = nullptr);

		/**
		 * @brief destructor
		 */
		virtual ~VideoPlayer();

		/**
		 * @brief Shared Pointer
		 */
		typedef std::shared_ptr< VideoPlayer > Ptr;

		/**
		 * @brief Plays a video.
		 *
		 * @param[in] videoFile The name of the video file to play.
		 * @param[in] muted True if the audio should be muted.
		 */
		void play(const QString& videoFile, bool muted = false);

		/**
		 * @brief Plays a video.
		 *
		 * @param[in] videoFile The name of the video file to play.
		 * @param[in] startTime The time at which to start playing the video file from.
		 * @param[in] muted True if the audio should be muted.
		 */
		void play(const QString& videoFile, size_t startTime, bool muted = false);

		/**
		 * @brief Pauses the video that is currently playing.
		 */
		void pause();

		/**
		 * @brief Resumes the video if it was paused.
		 */
		void resume();

		/**
		 * @brief Stops the video.
		 */
		void stop();

		/**
		 * @brief Resize the widget.
		 *
		 * @param[in] size The new size.
		 */
		void resize(const QSize& size);

	protected:
		/** Variables */
		QMediaPlayer* _player = nullptr;
		QVideoWidget* _videoWidget = nullptr;
		VideoPlayState _state = VideoPlayState::IDLE;
	};
}
