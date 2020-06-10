#include "QuizFactory.hpp"

#include <math.h>
#include <time.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdexcept>

#include <QString>
#include <QMessageBox>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "common/Log.hpp"
#include "common/TimeUtil.hpp"
#include "util/QuizLoader.hpp"
#include "gui_tools/widgets/QuizEntry.hpp"
#include "gui_tools/widgets/QuizCategory.hpp"
#include "gui_tools/QuizCreator/EntryCreator.hpp"
#include "gui_tools/QuizCreator/CategoryCreator.hpp"


MusicQuiz::QuizBoard* MusicQuiz::QuizFactory::createQuiz(const std::string& quizName, const QuizSettings& settings, const media::AudioPlayer::Ptr& audioPlayer,
	const media::VideoPlayer::Ptr& videoPlayer, const std::vector<MusicQuiz::QuizTeam*>& teams, bool preview, QWidget* parent)
{
	/** Get List of Quizzes */
	std::vector<std::string> quizList = MusicQuiz::util::QuizLoader::getListOfQuizzes();
	if ( quizList.empty() ) {
		throw std::runtime_error("No quizzes found in the data folder.");
	}

	/** Check if Quiz Exists */
	size_t idx = 0;
	bool quizExists = false;
	for ( size_t i = 0; i < quizList.size(); ++i ) {
		std::replace(quizList[i].begin(), quizList[i].end(), '\\', '/');
		if ( quizName == quizList[i] ) {
			idx = i;
			quizExists = true;
			break;
		}
	}

	/** Sanity Check */
	if ( !quizExists ) {
		throw std::runtime_error("Quiz does not exists.");
	}

	/** Create Quiz */
	return createQuiz(idx, settings, audioPlayer, videoPlayer, teams, preview, parent);
}

MusicQuiz::QuizBoard* MusicQuiz::QuizFactory::createQuiz(const size_t idx, const QuizSettings& settings, const media::AudioPlayer::Ptr& audioPlayer,
	const media::VideoPlayer::Ptr& videoPlayer, const std::vector<MusicQuiz::QuizTeam*>& teams, bool preview, QWidget* parent)
{
	/** Seed Rand */
	srand(time(NULL));

	/** Create Quiz Board */
	MusicQuiz::QuizBoard* quizBoard = nullptr;

	/** Load Categories */
	std::string loadError;
	std::vector<MusicQuiz::QuizCategory*> categories = MusicQuiz::util::QuizLoader::loadQuizCategories(idx, audioPlayer, videoPlayer, loadError);
	if ( !loadError.empty() ) {
		QMessageBox::information(nullptr, "Info", "Incomplete Quiz:\n\n" + QString::fromStdString(loadError));
	}

	/** Load Row Categories */
	std::vector< QString > rowCategories = MusicQuiz::util::QuizLoader::loadQuizRowCategories(idx);

	/** Hidden Team Score */
	if ( settings.hiddenTeamScore ) {
		for ( size_t i = 0; i < teams.size(); ++i ) {
			teams[i]->setHideScore(true);
		}
	}

	/** Get Number of Entries */
	size_t numberOfEntries = 0;
	for ( size_t i = 0; i < categories.size(); ++i ) {
		numberOfEntries += categories[i]->size();
	}

	std::vector<size_t> entriesIndicies(numberOfEntries);
	for ( size_t i = 0; i < numberOfEntries; ++i ) {
		entriesIndicies[i] = i;
	}

	/** Get Daily Double Entries */
	size_t dailyDoubleCount = std::floor(double(numberOfEntries * settings.dailyDoublePercentage) / 100.0);
	std::vector<size_t> dailyDoubleSelectedElements;
	if ( settings.dailyDouble && !teams.empty() ) {
		/** Ensure that there is atleas one element if the setting is enabled */
		if ( dailyDoubleCount == 0 ) {
			dailyDoubleCount = 1;
		}

		/** Select Elements */
		while ( dailyDoubleSelectedElements.size() < dailyDoubleCount && !entriesIndicies.empty() ) {
			const size_t randomIdx = rand() % entriesIndicies.size();
			dailyDoubleSelectedElements.push_back(entriesIndicies[randomIdx]);
			entriesIndicies.erase(entriesIndicies.begin() + randomIdx);
		}
	}

	/** Get Daily Triple Entries */
	size_t dailyTripleCount = std::floor(double(numberOfEntries * settings.dailyTriplePercentage) / 100.0);
	std::vector<size_t> dailyTripleSelectedElements;
	if ( settings.dailyTriple && !teams.empty() ) {
		/** Ensure that there is atleas one element if the setting is enabled */
		if ( dailyTripleCount == 0 ) {
			dailyTripleCount = 1;
		}

		/** Select Elements */
		while ( dailyTripleSelectedElements.size() < dailyTripleCount && !entriesIndicies.empty() ) {
			const size_t randomIdx = rand() % entriesIndicies.size();
			dailyTripleSelectedElements.push_back(entriesIndicies[randomIdx]);
			entriesIndicies.erase(entriesIndicies.begin() + randomIdx);
		}
	}

	/** Apply Settings */
	size_t counter = 0;
	for ( size_t i = 0; i < categories.size(); ++i ) {
		for ( size_t j = 0; j < categories[i]->size(); ++j ) {
			MusicQuiz::QuizEntry* quizEntry = (*categories[i])[j];
			if ( quizEntry != nullptr ) {
				/** Double Points */
				for ( size_t k = 0; k < dailyDoubleSelectedElements.size(); ++k ) {
					if ( counter == dailyDoubleSelectedElements[k] ) {
						quizEntry->setDoublePointsEnabled(true, settings.dailyDoubleHidden);
					}

				}

				/** Triple Points */
				for ( size_t k = 0; k < dailyTripleSelectedElements.size(); ++k ) {
					if ( counter == dailyTripleSelectedElements[k] ) {
						quizEntry->setTriplePointsEnabled(true, settings.dailyTripleHidden);
					}

				}

				/** Hidden Answers */
				quizEntry->setHiddenAnswer(settings.hiddenAnswers);
			}
			++counter;
		}
	}

	/** Create Quiz */
	quizBoard = new MusicQuiz::QuizBoard(categories, rowCategories, teams, settings, preview, parent);

	/** Set Quiz Name */
	//quizBoard->setQuizName(QString::fromStdString(MusicQuiz::util::QuizLoader::getListOfQuizzes[idx]));

	return quizBoard;
}

void MusicQuiz::QuizFactory::saveQuiz(const MusicQuiz::QuizCreator::QuizData& data, QWidget* parent)
{
	/** Tmp Folder */
	std::string mediaDirectoryPath;

	try {
		/** Get Quiz Name */
		const std::string quizName = data.quizName.toStdString();
		if ( quizName.empty() ) {
			return;
		}

		/** Get Quiz Description */
		const std::string quizDescription = data.quizDescription.toStdString();
		if ( quizName.empty() ) {
			return;
		}

		/** Check if quiz already exists */
		boost::system::error_code boost_err;
		const std::string quizPath = "./data/" + quizName;
		if ( boost::filesystem::is_directory(quizPath) ) {
			QMessageBox::StandardButton resBtn = QMessageBox::question(parent, "Overwrite Quiz?", "Quiz already exists, do you want to overwrite existing quiz?",
				QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

			if ( resBtn != QMessageBox::Yes ) {
				return;
			}
		} else if ( !boost::filesystem::is_directory(quizPath) ) {
			/** Create Folder */
			boost::filesystem::create_directory(quizPath, boost_err);
			if ( boost_err ) {
				QMessageBox::warning(parent, "Failed to Save Quiz", "Failed to create directory to save the quiz in.");
				return;
			}
		}

		/** Create Media Folder */
		mediaDirectoryPath = quizPath + "/mediaTmp";
		boost::filesystem::create_directory(mediaDirectoryPath, boost_err);
		if ( boost_err ) {
			QMessageBox::warning(parent, "Failed to Save Quiz", "Failed to create directory to save the media files in.");
			return;
		}

		/** Wirte Quiz to XML Parser */
		boost::property_tree::ptree tree;
		boost::property_tree::ptree& main_tree = tree.put("MusicQuiz", "");
		main_tree.put("<xmlcomment>", std::string("File content written on the ") + common::TimeUtil::getTimeNow());

		/** Quiz Name */
		main_tree.put("QuizName", quizName);

		/** Quiz Author */
		main_tree.put("QuizAuthor", ""); // \todo add this

		/** Quiz Description */
		main_tree.put("QuizDescription", quizDescription);

		/** Guess the Category Setting */
		boost::property_tree::ptree& guessTheCategory_tree = main_tree.add("QuizGuessTheCateotry", 500);
		guessTheCategory_tree.put("<xmlattr>.enabled", data.guessTheCategory);

		/** Categories */
		const size_t numberOfCategories = data.quizCategories.size();
		for ( size_t i = 0; i < numberOfCategories; ++i ) {
			/** Get Category */
			const MusicQuiz::CategoryCreator* category = data.quizCategories[i];

			/** Create Category tree */
			boost::property_tree::ptree& category_tree = main_tree.add("QuizCategories.Category", "");

			/** Category Name */
			const std::string categoryName = category->getName().toStdString();
			if ( categoryName.empty() ) {
				QMessageBox::warning(parent, "Failed to Save Quiz", "Failed to save quiz. All categories must have a name.");
				deleteDirectory(mediaDirectoryPath);
				return;
			}

			/** Check if two categories have the same name */
			for ( size_t j = 0; j < numberOfCategories; ++j ) {
				if ( j != i ) {
					if ( categoryName == data.quizCategories[j]->getName().toStdString() ) {
						QMessageBox::warning(parent, "Failed to Save Quiz", "Failed to save quiz. All categories must have a unique name.");
						deleteDirectory(mediaDirectoryPath);
						return;
					}
				}
			}
			category_tree.put("<xmlattr>.name", categoryName);

			/** Create Category Folder */
			boost::filesystem::create_directory(mediaDirectoryPath + "/" + categoryName, boost_err);
			if ( boost_err ) {
				QMessageBox::warning(parent, "Failed to Save Quiz", "Failed to create directory to save the catrgory files in.");
				deleteDirectory(mediaDirectoryPath);
				return;
			}

			/** Category Entries */
			const std::vector< MusicQuiz::EntryCreator* > entries = category->getEntries();
			const size_t numberOfEntries = entries.size();
			for ( size_t j = 0; j < numberOfEntries; ++j ) {
				/** Get Entry */
				MusicQuiz::EntryCreator* entry = entries[j];

				/** Create Entry tree */
				boost::property_tree::ptree& entry_tree = category_tree.add("QuizEntry", "");

				/** Entry Name */
				const std::string entryName = entry->getName().toStdString();
				if ( entryName.empty() ) {
					QMessageBox::warning(parent, "Failed to Save Quiz", "Failed to save quiz. " + QString::fromStdString(categoryName) + ": All entries needs to have a name.");
					deleteDirectory(mediaDirectoryPath);
					return;
				}

				/** Check if two entries have the same name */
				for ( size_t k = 0; k < numberOfEntries; ++k ) {
					if ( k != j ) {
						if ( entryName == category->getEntries()[k]->getName().toStdString() ) {
							QMessageBox::warning(parent, "Failed to Save Quiz", "Failed to save quiz. " + QString::fromStdString(categoryName) + ": All entires in a category must have a unique name.");
							deleteDirectory(mediaDirectoryPath);
							return;
						}
					}
				}
				entry_tree.put("<xmlattr>.name", entryName);

				/** Entry Type */
				const MusicQuiz::EntryCreator::EntryType type = entry->getType();
				if ( type == MusicQuiz::EntryCreator::EntryType::Song ) {
					entry_tree.put("<xmlattr>.type", "song");
				} else if ( type == MusicQuiz::EntryCreator::EntryType::Video ) {
					entry_tree.put("<xmlattr>.type", "video");
				}

				/** Entry Answer */
				const std::string entryAnswer = entry->getAnswer().toStdString();
				if ( entryAnswer.empty() ) {
					QMessageBox::warning(parent, "Failed to Save Quiz", "Failed to save quiz. " + QString::fromStdString(categoryName) + ": Answers have to be set for all entries.");
					deleteDirectory(mediaDirectoryPath);
					return;
				}
				entry_tree.put("Answer", entryAnswer);

				/** Entry Points */
				entry_tree.put("Points", entry->getPoints());

				/** Media */
				if ( type == MusicQuiz::EntryCreator::EntryType::Song ) { // Song
					/** Entry Song Start Time */
					entry_tree.put("StartTime", entry->getSongStartTime());

					/** Entry Song Answer Start Time */
					entry_tree.put("AnswerStartTime", entry->getAnswerStartTime());

					/** Media File */
					const std::string songFile = entry->getSongFile().toStdString();
					if ( !songFile.empty() ) {
						const std::string songPath = quizPath + "/media/" + categoryName + "/" + entryName + ".mp3";
						boost::property_tree::ptree& media_tree = entry_tree.add("Media", "");
						media_tree.put("SongFile", songPath);

						/** Copy Media File */
						boost::filesystem::copy_file(songFile, mediaDirectoryPath + "/" + categoryName + "/" + entryName + ".mp3", boost::filesystem::copy_option::overwrite_if_exists);
					}
				} else if ( type == MusicQuiz::EntryCreator::EntryType::Video ) { // Video
					/** Entry Video Start Time */
					entry_tree.put("StartTime", entry->getVideoStartTime());

					/** Entry Video Song Start Time */
					entry_tree.put("VideoSongStartTime", entry->getVideoSongStartTime());

					/** Entry Video Answer Start Time */
					entry_tree.put("AnswerStartTime", entry->getVideoAnswerStartTime());

					/** Media File */
					const std::string videoFile = entry->getVideoFile().toStdString();
					const std::string songFile = entry->getVideoSongFile().toStdString();

					if ( !videoFile.empty() && !songFile.empty() ) {
						const std::string videoPath = quizPath + "/media/" + categoryName + "/" + entryName + ".mp4";
						const std::string songPath = quizPath + "/media/" + categoryName + "/" + entryName + ".mp3";
						boost::property_tree::ptree& media_tree = entry_tree.add("Media", "");
						media_tree.put("VideoFile", videoPath);
						media_tree.put("SongFile", songPath);

						/** Copy Media File */
						boost::filesystem::copy_file(videoFile, mediaDirectoryPath + "/" + categoryName + "/" + entryName + ".mp4", boost::filesystem::copy_option::overwrite_if_exists);
						boost::filesystem::copy_file(songFile, mediaDirectoryPath + "/" + categoryName + "/" + entryName + ".mp3", boost::filesystem::copy_option::overwrite_if_exists);
					}
				}
			}
		}

		/** Row Categories */
		const size_t numberOfRowCategories = data.quizRowCategories.size();
		for ( size_t i = 0; i < numberOfRowCategories; ++i ) {
			boost::property_tree::ptree& category_tree = main_tree.add("QuizRowCategories.RowCategory", data.quizRowCategories[i].toStdString());
		}

		/** Delete Previous Folder */
		const boost::filesystem::path target(quizPath + "/media");
		deleteDirectory(target);

		/** Rename tmp folder */
		boost::filesystem::rename(mediaDirectoryPath, target);

		/** Save Quiz */
#if ( BOOST_VERSION >= 105600 )
		boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
#elif
		boost::property_tree::xml_writer_settings<char> settings('\t', 1);
#endif
		boost::property_tree::write_xml(quizPath + "/" + quizName + ".quiz.xml", tree, std::locale(), settings);

		/** Create Cheat Sheet */
		std::ofstream cheatSheet(quizPath + "/" + quizName + ".cheatsheet.txt");
		if ( cheatSheet.is_open() ) {
			cheatSheet << "--------------   CHEATSHEET   --------------\n"
				<< "Quiz: " << quizName << "\n"
				<< "Guess the Category: " << (data.guessTheCategory ? "Enabled" : "Disabled");

			const std::vector< MusicQuiz::CategoryCreator* > categories = data.quizCategories;
			for ( size_t i = 0; i < categories.size(); ++i ) {
				cheatSheet << "\n\n-----  " << categories[i]->getName().toStdString() << "  -----";
				std::vector< MusicQuiz::EntryCreator* > categoryEntries = data.quizCategories[i]->getEntries();
				for ( size_t j = 0; j < categoryEntries.size(); ++j ) {
					cheatSheet << "\n#" << j + 1 << " - " << categoryEntries[j]->getPoints() << " - " << categoryEntries[j]->getAnswer().toStdString();
				}
			}
			cheatSheet.flush();
			cheatSheet.close();
		}

		/** Popup to tell user that the quiz was saved */
		QMessageBox::information(parent, "Info", "Quiz saves successfully.");
	} catch ( const std::exception& err ) {
		QMessageBox::warning(parent, "Failed to Save Quiz", "Failed to save the quiz. " + QString::fromStdString(err.what()));
		deleteDirectory(mediaDirectoryPath);
	} catch ( ... ) {
		QMessageBox::warning(parent, "Failed to Save Quiz", "Failed to save the quiz. Unkown Error.");
		deleteDirectory(mediaDirectoryPath);
	}
}

MusicQuiz::QuizCreator::QuizData MusicQuiz::QuizFactory::loadQuiz(const std::string& quizName, const media::AudioPlayer::Ptr& audioPlayer,
	const media::VideoPlayer::Ptr& videoPlayer, QWidget* parent)
{
	/** Get List of Quizzes */
	std::vector<std::string> quizList = MusicQuiz::util::QuizLoader::getListOfQuizzes();
	if ( quizList.empty() ) {
		throw std::runtime_error("No quizzes found in the data folder.");
	}

	/** Check if Quiz Exists */
	size_t idx = 0;
	bool quizExists = false;
	for ( size_t i = 0; i < quizList.size(); ++i ) {
		std::replace(quizList[i].begin(), quizList[i].end(), '\\', '/');
		if ( quizName == quizList[i] ) {
			idx = i;
			quizExists = true;
			break;
		}
	}

	/** Sanity Check */
	if ( !quizExists ) {
		throw std::runtime_error("Quiz does not exists.");
	}

	/** Quiz Data */
	MusicQuiz::QuizCreator::QuizData data;

	/** Load Categories */
	boost::property_tree::ptree tree;
	boost::property_tree::read_xml(quizList[idx], tree, boost::property_tree::xml_parser::trim_whitespace);
	boost::property_tree::ptree sub_tree = tree.get_child("MusicQuiz");

	/** Quiz Name */
	boost::property_tree::ptree::const_iterator ini_ctrl = tree.begin();
	data.quizName = QString::fromStdString(ini_ctrl->second.get<std::string>("QuizName"));

	/** Quiz Description */
	data.quizDescription = QString::fromStdString(ini_ctrl->second.get<std::string>("QuizDescription"));

	/** Hidden Categories */
	data.guessTheCategory = ini_ctrl->second.get<bool>("QuizGuessTheCateotry.<xmlattr>.enabled");

	/** Categories */
	std::vector< MusicQuiz::CategoryCreator* > categories;
	boost::property_tree::ptree::const_iterator ctrl = sub_tree.begin();
	for ( ; ctrl != sub_tree.end(); ++ctrl ) {
		if ( ctrl->first == "QuizCategories" ) {
			boost::property_tree::ptree categoriesTree = ctrl->second;
			boost::property_tree::ptree::const_iterator sub_ctrl = categoriesTree.begin();
			try {
				for ( ; sub_ctrl != categoriesTree.end(); ++sub_ctrl ) {
					if ( sub_ctrl->first == "Category" ) {
						/** Category Name */
						const QString categoryName = QString::fromStdString(sub_ctrl->second.get<std::string>("<xmlattr>.name"));

						/** Category */
						MusicQuiz::CategoryCreator* category = new MusicQuiz::CategoryCreator(categoryName, audioPlayer, parent);

						/** Category Entries */
						std::vector< MusicQuiz::EntryCreator* > categorieEntries;
						boost::property_tree::ptree entryTree = sub_ctrl->second;
						boost::property_tree::ptree::const_iterator it = entryTree.begin();
						for ( ; it != entryTree.end(); ++it ) {
							try {
								if ( it->first == "QuizEntry" ) {
									const boost::filesystem::path full_path(boost::filesystem::current_path());

									/** Settings */
									const QString entryName = QString::fromStdString(it->second.get<std::string>("<xmlattr>.name"));
									const size_t points = it->second.get<size_t>("Points");

									/** Quiz Entry */
									MusicQuiz::EntryCreator* entry = new MusicQuiz::EntryCreator(entryName, points, audioPlayer, category);
									entry->setAnswer(QString::fromStdString(it->second.get<std::string>("Answer")));

									/** Media Type */
									const std::string type = it->second.get<std::string>("<xmlattr>.type");
									if ( type == "song" ) { // Song
										entry->setType(MusicQuiz::EntryCreator::EntryType::Song);

										/** Start Time */
										try {
											entry->setSongStartTime(it->second.get<size_t>("StartTime"));
										} catch ( ... ) {}

										try {
											entry->setAnswerStartTime(it->second.get<size_t>("AnswerStartTime"));
										} catch ( ... ) {}

										/** Song File */
										try {
											QString songFile = QString::fromStdString(full_path.string() + "/" + it->second.get<std::string>("Media.SongFile"));
											std::replace(songFile.begin(), songFile.end(), '\\', '/');
											entry->setSongFile(songFile);
										} catch ( ... ) {}
									} else if ( type == "video" ) { // Video
										entry->setType(MusicQuiz::EntryCreator::EntryType::Video);

										/** Song File */
										try {
											QString songFile = QString::fromStdString(full_path.string() + "/" + it->second.get<std::string>("Media.SongFile"));
											std::replace(songFile.begin(), songFile.end(), '\\', '/');
											entry->setVideoSongFile(songFile);
										} catch ( ... ) {}

										try {
											entry->setVideoSongStartTime(it->second.get<size_t>("VideoSongStartTime"));
										} catch ( ... ) {}

										/** Video File */
										try {
											QString videoFile = QString::fromStdString(full_path.string() + "/" + it->second.get<std::string>("Media.VideoFile"));
											std::replace(videoFile.begin(), videoFile.end(), '\\', '/');
											entry->setVideoFile(videoFile);
										} catch ( ... ) {}

										try {
											entry->setVideoStartTime(it->second.get<size_t>("StartTime"));
										} catch ( ... ) {}

										try {
											entry->setVideoAnswerStartTime(it->second.get<size_t>("AnswerStartTime"));
										} catch ( ... ) {}
									}
									categorieEntries.push_back(entry);
								}
							} catch ( ... ) {}
						}
						category->setEntries(categorieEntries);
						categories.push_back(category);
					}
				}
			} catch ( const std::exception& err ) {
				LOG_ERROR("Failed to load quiz. " << err.what());
			} catch ( ... ) {
				LOG_ERROR("Failed to load quiz.");
			}
		}
	}
	data.quizCategories = categories;

	/** Load Row Categories */
	data.quizRowCategories = MusicQuiz::util::QuizLoader::loadQuizRowCategories(idx);

	/** Return */
	return data;
}

void MusicQuiz::QuizFactory::deleteDirectory(const boost::filesystem::path& dir)
{
	if ( boost::filesystem::exists(dir) || boost::filesystem::is_directory(dir) ) {
		boost::filesystem::directory_iterator file(dir), end;
		for ( ; file != end; ++file ) {
			boost::filesystem::remove_all(file->path());
		}
		boost::filesystem::remove_all(dir);
	}
}