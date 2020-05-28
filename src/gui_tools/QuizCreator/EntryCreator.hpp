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
#include <QPushButton>
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
		 * @brief Updates the entry name.
		 *
		 * @param[in] name The name.
		 */
		void setName(const QString& name);

		/**
		 * @brief Gets the entry name.
		 *
		 * @return The name.
		 */
		const QString getName() const;

		/**
		 * @brief Gets the entry answer.
		 *
		 * @return The answer.
		 */
		const QString getAnswer() const;

		/**
		 * @brief Gets the points.
		 *
		 * @return The points.
		 */
		const size_t getPoints() const;

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
		 * @brief Pauses the song playing.
		 */
		void pauseSong();

		/**
		 * @brief Stops the song playing.
		 */
		void stopSong();

		/**
		 * @brief Sets the entry type [0 = song, 1 = video].
		 *
		 * @param[in] index The type index.
		 */
		void setEntryType(int index);

		void pointsChanged(int points);

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
		 * @brief Creates the video file category layout.
		 */
		QGridLayout* createVideoFileLayout();
		
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

		/**
		 * @brief Gets the time in msec from a QTime.
		 * @note The Qtime is used so the hh::mm corresponds to mm::ss.
		 *
		 * @param[in] time The time.
		 *
		 * @return The time in miliseconds.
		 */
		size_t getMSec(const QTime& time);

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

		QTimeEdit* _videoEndTimeEdit = nullptr;
		QTimeEdit* _videoStartTimeEdit = nullptr;
		QTimeEdit* _videoAnswerEndTimeEdit = nullptr;
		QTimeEdit* _videoAnswerStartTimeEdit = nullptr;

		QWidget* _songSettings = nullptr;
		QWidget* _videoSettings = nullptr;
		QPushButton* _browseVideoBtn = nullptr;

		std::shared_ptr< audio::AudioPlayer > _audioPlayer = nullptr;
	};
}
