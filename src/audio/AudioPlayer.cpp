#include "AudioPlayer.hpp"

#include <stdexcept>


#if defined(_WIN32) || defined(WIN32)
#define WINDOWS_INTERFACE true
#else
#define WINDOWS_INTERFACE false
#endif

#if WINDOWS_INTERFACE
#include<windows.h>
#pragma comment(lib, "winmm.lib")
#endif


audio::AudioPlayer::~AudioPlayer()
{
	/** Stop Audio */
#if WINDOWS_INTERFACE
	mciSendString("close audioTrack", NULL, 0, NULL);
#endif
}

void audio::AudioPlayer::play(const std::string& audioFile)
{
	/** Sanity Check */
	if ( audioFile.empty() ) {
		throw std::runtime_error("Audio File Name is empty.");
	}

#if WINDOWS_INTERFACE
	/** Stop audio if any is playing and close file */
	stop();

	/** Open File */
	openFile(audioFile);

	/** Play Audio */
	const std::string playCommand = "play audioTrack";
	mciSendString(playCommand.c_str(), NULL, 0, NULL);
#endif

	/** Set State */
	_state = AudioPlayState::PLAYING;
}

void audio::AudioPlayer::play(const std::string& audioFile, const size_t startTime)
{
	/** Sanity Check */
	if ( audioFile.empty() ) {
		throw std::runtime_error("Audio File Name is empty.");
	}

#if WINDOWS_INTERFACE
	/** Stop audio if any is playing and close file */
	stop();

	/** Open File */
	openFile(audioFile);

	/** Play Audio */
	const std::string playCommand = "play audioTrack from " + std::to_string(startTime);
	mciSendString(playCommand.c_str(), NULL, 0, NULL);
#endif

	/** Set State */
	_state = AudioPlayState::PLAYING;
}

void audio::AudioPlayer::play(const std::string& audioFile, const size_t startTime, const size_t endTime)
{
	/** Sanity Check */
	if ( audioFile.empty() ) {
		throw std::runtime_error("Audio File Name is empty.");
	}

	if ( endTime <= startTime ) {
		throw std::runtime_error("End Time can not be smaller or equal to the start time.");
	}

#if WINDOWS_INTERFACE
	/** Stop audio if any is playing and close file */
	stop();

	/** Open File */
	openFile(audioFile);

	/** Play Audio */
	const std::string playCommand = "play audioTrack from " + std::to_string(startTime) + " to " + std::to_string(endTime);
	mciSendString(playCommand.c_str(), NULL, 0, NULL);
#endif

	/** Set State */
	_state = AudioPlayState::PLAYING;
}

void audio::AudioPlayer::pause()
{
	/** Check State */
	if ( _state != AudioPlayState::PLAYING ) {
		return;
	}

	/** Pause audio */
#if WINDOWS_INTERFACE
	mciSendString("pause audioTrack", NULL, 0, NULL);
#endif

	/** Set State */
	_state = AudioPlayState::PAUSED;
}

void audio::AudioPlayer::resume()
{
	/** Check State */
	if ( _state != AudioPlayState::PAUSED ) {
		return;
	}

	/** Pause audio */
#if WINDOWS_INTERFACE
	const std::string playCommand = "resume audioTrack";
	mciSendString(playCommand.c_str(), NULL, 0, NULL);
#endif

	/** Set State */
	_state = AudioPlayState::PLAYING;
}

void audio::AudioPlayer::stop()
{
	/** Stop audio and close file */
#if WINDOWS_INTERFACE
	mciSendString("close audioTrack", NULL, 0, NULL);
	mciSendString("close all", NULL, 0, NULL);
	mciSendString("clear all", NULL, 0, NULL);
#endif

	/** Set State */
	_state = AudioPlayState::IDLE;
}

void audio::AudioPlayer::openFile(const std::string& audioFile)
{
	/** Get File extension */
	const std::string fileExt = getFileExtension(audioFile);
	if ( fileExt.empty() ) {
		throw std::runtime_error("Audio File Name does not have an extension type.");
	}

#if WINDOWS_INTERFACE
	/** Open Mp3 File */
	std::string openCommand = "open \"" + audioFile;
	if ( fileExt == "mp3" || fileExt == "MP3" ) {
		openCommand += "\" type mpegvideo alias audioTrack";
	} else if ( fileExt == "wav" || fileExt == "WAV" ) {
		openCommand += "\" type waveaudio alias audioTrack";
	} else {
		throw std::runtime_error("Unknown audio type.");
	}
	mciSendString(openCommand.c_str(), NULL, 0, NULL);
#endif
}

std::string audio::AudioPlayer::getFileExtension(const std::string& str) const
{
	size_t idx = str.rfind('.', str.length());
	if ( idx != std::string::npos ) {
		return(str.substr(idx + 1, str.length() - idx));
	}
	return "";
}