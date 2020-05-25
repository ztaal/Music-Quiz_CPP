#pragma once 

#include <vector>

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QSpinBox>
#include <QTimeEdit>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QTableWidget>

#include "audio/AudioPlayer.hpp"


namespace MusicQuiz {

	class EntryCreator : public QWidget
	{
		Q_OBJECT

	public:
		/**
		 * @brief Constructor
		 *
		 * @param[in] name The entry name.
		 * @param[in] points The points of the entry.
		 * @param[in] audioPlayer The audio player.
		 * @param[in] parent The parent widget.
		 */
		explicit EntryCreator(const QString& name, size_t points, const std::shared_ptr< audio::AudioPlayer >& audioPlayer, QWidget* parent = nullptr);

		/**
		 * @brief Default destructor
		 */
		virtual ~EntryCreator() = default;

	public slots:
		/**
		 * @brief Updates the category name
		 *
		 * @param[in] name The name.
		 */
		void setName(const QString& name);

	private slots:
		/**
		 * @brief Opens a dialog to browse for a song file.
		 */
		void browseSong();

		/**
		 * @brief Opens a dialog to browse for a video file.
		 */
		void browseVideo();

		/**
		 * @brief Check if the song file name is valid and enables / disables widgets.
		 *
		 * @param[in] fileName The file name.
		 */
		void checkSongFileName(const QString& fileName);

		/**
		 * @brief Check if the video file name is valid and enables / disables widgets.
		 *
		 * @param[in] fileName The file name.
		 */
		void checkVideoFileName(const QString& fileName);

		/**
		 * @brief Plays the song file from the start position defined in the start QTimeEdit.
		 */
		void playSong();

		/**
		 * @brief Plays the answer file from the start position defined in the start QTimeEdit.
		 */
		void playAnswer();

		/**
		 * @brief Pauses the song playing.
		 */
		void pauseSong();

		/**
		 * @brief Stops the song playing.
		 */
		void stopSong();

	private:
		/**
		 * @brief Creates the category layout.
		 */
		void createLayout();

		/**
		 * @brief Creates the song file category layout.
		 */
		QGridLayout* createSongFileLayout();

		/**
		 * @brief Checks if the song file name is valid.
		 *
		 * @param[in] fileName The file name.
		 *
		 * @return True is name is valid.
		 */
		bool isSongFileValid(const QString& fileName);

		/**
		 * @brief Checks if the video file name is valid.
		 *
		 * @param[in] fileName The file name.
		 *
		 * @return True is name is valid.
		 */
		bool isVideoFileValid(const QString& fileName);

		/** Variables */
		size_t _points = 0;

		QString _entryName;
		QLabel* _entryNameLabel = nullptr;

		QSpinBox* _pointsSpinbox = nullptr;
		QLineEdit* _answerLineEdit = nullptr;

		QLineEdit* _songFileLineEdit = nullptr;
		QLineEdit* _videoFileLineEdit = nullptr;

		QTimeEdit* _songEndTimeEdit = nullptr;
		QTimeEdit* _songStartTimeEdit = nullptr;
		QTimeEdit* _answerEndTimeEdit = nullptr;
		QTimeEdit* _answerStartTimeEdit = nullptr;

		QGridLayout* _songSettingsLayout = nullptr;
		QGridLayout* _videoSettingsLayout = nullptr;

		std::shared_ptr< audio::AudioPlayer > _audioPlayer = nullptr;
	};
}
