#pragma once

#include <ctime>
#include <string>


namespace audio {
	class AudioPlayer
	{
	public:
		explicit AudioPlayer() = default;

		virtual ~AudioPlayer();

		enum class AudioPlayState
		{
			IDLE = 1,				// Default
			PLAYING = 2,
			PAUSED = 3
		};

		/**
		 * @brief Plays a song.
		 *
		 * @param[in] audioFile The name of the audio file to play.
		 */
		void play(const std::string& audioFile);

		/**
		 * @brief Plays a song.
		 *
		 * @param[in] audioFile The name of the audio file to play.
		 * @param[in] startTime The time at which to start playing the audio file from.
		 */
		void play(const std::string& audioFile, size_t startTime);

		/**
		 * @brief Plays a song.
		 *
		 * @param[in] audioFile The name of the audio file to play.
		 * @param[in] startTime The time at which to start playing the audio file from.
		 * @param[in] endTime The time at which to end the audio at.
		 */
		void play(const std::string& audioFile, size_t startTime, size_t endTime);

		/**
		 * @brief Pauses the song that is currently playing.
		 */
		void pause();

		/**
		 * @brief Resumes the song if it was paused.
		 */
		void resume();

		/**
		 * @brief Stops the song.
		 */
		void stop();

	protected:
		/**
		 * @brief Opens the audio file.
		 *
		 * @param[in] audioFile The name of the audio file to play.
		 */
		void openFile(const std::string& audioFile);

		/**
		 * @brief Returns the file extension of a string.
		 *
		 * @param[in] str The string in which to find the file extension.
		 * @return The file extension.
		 */
		std::string getFileExtension(const std::string& str) const;

		/** Variables */
		AudioPlayState _state = AudioPlayState::IDLE;
	};
}
