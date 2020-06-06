#include "QuizLoader.hpp"

#include <stdexcept>
#include <algorithm>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "common/Log.hpp"

#include "gui_tools/widgets/QuizEntry.hpp"


std::vector<std::string> MusicQuiz::util::QuizLoader::getListOfQuizzes()
{
	/** Check if data folder exists */
	const boost::filesystem::path dataFolder = "./data/";
	if ( !boost::filesystem::is_directory(dataFolder) ) {
		throw std::runtime_error("Data folder does not exists.");
	}

	/** Find Quizzes */
	std::vector<std::string> quizList;
	boost::filesystem::recursive_directory_iterator end;

	for ( boost::filesystem::recursive_directory_iterator file(dataFolder); file != end; ++file ) {
		const std::string fileStr = boost::filesystem::path(*file).string();

		/** Skip non quiz files */
		if ( fileStr.find(".quiz.xml") == std::string::npos ) {
			continue;
		}

		/** Push back quiz */
		quizList.push_back(fileStr);
	}

	return quizList;
}

MusicQuiz::util::QuizLoader::QuizPreview MusicQuiz::util::QuizLoader::getQuizPreview(size_t idx)
{
	/** Get List of Quizzes */
	const std::vector<std::string> quizList = getListOfQuizzes();
	if ( quizList.empty() ) {
		throw std::runtime_error("No quizzes found in the data folder.");
	}

	/** Sanity Check */
	if ( idx >= quizList.size() ) {
		throw std::runtime_error("Index out of range.");
	}

	if ( !boost::filesystem::exists(quizList[idx]) ) {
		throw std::runtime_error("Quiz file does not exists.");
	}

	/** Load preview */
	QuizLoader::QuizPreview quizPreview;
	boost::property_tree::ptree tree;
	boost::property_tree::read_xml(quizList[idx], tree, boost::property_tree::xml_parser::trim_whitespace);
	boost::property_tree::ptree sub_tree = tree.get_child("MusicQuiz");

	/** Name */
	quizPreview.quizName = sub_tree.get<std::string>("QuizName");

	/** Author */
	try {
		quizPreview.quizAuthor = sub_tree.get<std::string>("QuizAuthor");
	} catch ( ... ) {}

	/** Description */
	quizPreview.quizDescription = sub_tree.get<std::string>("QuizDescription");

	/** Guess the Category */
	try {
		quizPreview.guessTheCategory = sub_tree.get<bool>("QuizGuessTheCateotry.<xmlattr>.enabled");
	} catch ( ... ) {}

	/** Categories & Row Categories */
	boost::property_tree::ptree::const_iterator ctrl = sub_tree.begin();
	for ( ; ctrl != sub_tree.end(); ++ctrl ) {
		if ( ctrl->first == "QuizCategories" ) { // Load Categories
			boost::property_tree::ptree categoriesTree = ctrl->second;
			boost::property_tree::ptree::const_iterator sub_ctrl = categoriesTree.begin();
			for ( ; sub_ctrl != categoriesTree.end(); ++sub_ctrl ) {
				if ( sub_ctrl->first == "Category" ) {
					quizPreview.categories.push_back(sub_ctrl->second.get<std::string>("<xmlattr>.name"));

					if ( !quizPreview.includeSongs || !quizPreview.includeVideos ) { // Check if the quiz contains songs / videos.
						boost::property_tree::ptree mediaTree = sub_ctrl->second;
						boost::property_tree::ptree::const_iterator it = mediaTree.begin();
						for ( ; it != mediaTree.end(); ++it ) {
							if ( it->first == "QuizEntry" ) {
								if ( it->second.get<std::string>("<xmlattr>.type") == "song" && !quizPreview.includeSongs ) {
									quizPreview.includeSongs = true;
								} else if ( it->second.get<std::string>("<xmlattr>.type") == "video" && !quizPreview.includeVideos ) {
									quizPreview.includeVideos = true;
								}
							}
						}
					}
				}
			}
		} else if ( ctrl->first == "QuizRowCategories" ) { // Load Row Categories
			boost::property_tree::ptree rowCategoriesTree = ctrl->second;
			boost::property_tree::ptree::const_iterator sub_ctrl = rowCategoriesTree.begin();
			for ( ; sub_ctrl != rowCategoriesTree.end(); ++sub_ctrl ) {
				if ( sub_ctrl->first == "RowCategory" ) {
					quizPreview.rowCategories.push_back(sub_ctrl->second.data());
				}
			}
		}
	}

	return quizPreview;
}

std::vector<MusicQuiz::QuizCategory*> MusicQuiz::util::QuizLoader::loadQuizCategories(const size_t idx, const audio::AudioPlayer::Ptr& audioPlayer, std::string& err)
{
	/** Get List of Quizzes */
	const std::vector<std::string> quizList = getListOfQuizzes();
	if ( quizList.empty() ) {
		throw std::runtime_error("No quizzes found in the data folder.");
	}

	/** Sanity Check */
	if ( idx >= quizList.size() ) {
		throw std::runtime_error("No quiz index requested does not exists.");
	}

	if ( !boost::filesystem::exists(quizList[idx]) ) {
		throw std::runtime_error("Quiz file does not exists.");
	}


	/** Load Categories */
	LOG_INFO("Loading Quiz #" << idx << " '" << quizList[idx] << "'.");

	boost::property_tree::ptree tree;
	boost::property_tree::read_xml(quizList[idx], tree, boost::property_tree::xml_parser::trim_whitespace);
	boost::property_tree::ptree sub_tree = tree.get_child("MusicQuiz");

	std::vector<MusicQuiz::QuizCategory*> categories;
	boost::property_tree::ptree::const_iterator ctrl = sub_tree.begin();
	for ( ; ctrl != sub_tree.end(); ++ctrl ) {
		if ( ctrl->first == "QuizCategories" ) { // Load Categories
			boost::property_tree::ptree categoriesTree = ctrl->second;
			boost::property_tree::ptree::const_iterator sub_ctrl = categoriesTree.begin();
			try {
				for ( ; sub_ctrl != categoriesTree.end(); ++sub_ctrl ) {
					if ( sub_ctrl->first == "Category" ) {

						/** Category Name */
						const QString categoryName = QString::fromStdString(sub_ctrl->second.get<std::string>("<xmlattr>.name"));

						/** Category Entries */
						std::vector<MusicQuiz::QuizEntry*> categorieEntries;
						boost::property_tree::ptree entryTree = sub_ctrl->second;
						boost::property_tree::ptree::const_iterator it = entryTree.begin();
						for ( ; it != entryTree.end(); ++it ) {
							if ( it->first == "QuizEntry" ) {
								const boost::filesystem::path full_path(boost::filesystem::current_path());
								QString songFile = QString::fromStdString(full_path.string() + "/" + it->second.get<std::string>("Media.SongFile"));
								std::replace(songFile.begin(), songFile.end(), '\\', '/');
								const QString answer = QString::fromStdString(it->second.get<std::string>("Answer"));
								const size_t points = it->second.get<size_t>("Points");
								const size_t startTime = it->second.get<size_t>("StartTime");
								const size_t answerStartTime = it->second.get<size_t>("AnswerStartTime");

								/** Check if file exsists */
								if ( !boost::filesystem::exists(songFile.toStdString()) ) {
									err += "Missing song file '" + songFile.toStdString() + "'\n";
								}

								/** Push Back Entry */
								categorieEntries.push_back(new MusicQuiz::QuizEntry(songFile, answer, points, startTime, answerStartTime, audioPlayer));
							}
						}

						categories.push_back(new MusicQuiz::QuizCategory(categoryName, categorieEntries));
					}
				}
			} catch ( const std::exception& err ) {
				LOG_ERROR("Failed to load category. " << err.what());
			} catch ( ... ) {
				LOG_ERROR("Failed to load category.");
			}
			
		}
	}

	return categories;
}

std::vector<QString> MusicQuiz::util::QuizLoader::loadQuizRowCategories(const size_t idx)
{
	/** Get List of Quizzes */
	const std::vector<std::string> quizList = getListOfQuizzes();
	if ( quizList.empty() ) {
		throw std::runtime_error("No quizzes found in the data folder.");
	}

	/** Sanity Check */
	if ( idx < 0 || idx >= quizList.size() ) {
		throw std::runtime_error("No quiz index requested does not exists.");
	}

	if ( !boost::filesystem::exists(quizList[idx]) ) {
		throw std::runtime_error("Quiz file does not exists.");
	}


	/** Load Row Categories */
	boost::property_tree::ptree tree;
	boost::property_tree::read_xml(quizList[idx], tree, boost::property_tree::xml_parser::trim_whitespace);
	boost::property_tree::ptree sub_tree = tree.get_child("MusicQuiz");

	std::vector<QString> rowCategories;
	boost::property_tree::ptree::const_iterator ctrl = sub_tree.begin();
	for ( ; ctrl != sub_tree.end(); ++ctrl ) {
		if ( ctrl->first == "QuizRowCategories" ) { // Load Categories
			boost::property_tree::ptree rowCategoriesTree = ctrl->second;
			boost::property_tree::ptree::const_iterator sub_ctrl = rowCategoriesTree.begin();
			for ( ; sub_ctrl != rowCategoriesTree.end(); ++sub_ctrl ) {
				if ( sub_ctrl->first == "RowCategory" ) {
					rowCategories.push_back(QString::fromStdString(sub_ctrl->second.data()));
				}
			}
		}
	}

	return rowCategories;
}