#include "QuizLoader.hpp"

#include <stdexcept>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "common/Log.hpp"


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

	/** Description */
	quizPreview.quizDescription = sub_tree.get<std::string>("QuizDescription");

	/** Categories & Row Categories */
	boost::property_tree::ptree::const_iterator ctrl = sub_tree.begin();
	for ( ; ctrl != sub_tree.end(); ++ctrl ) {
		if ( ctrl->first == "Categories" ) { // Load Categories
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
								if ( it->second.get<bool>("Media.<xmlattr>.video", false) && !quizPreview.includeVideos ) {
									quizPreview.includeVideos = true;
								} else if ( !it->second.get<bool>("Media.<xmlattr>.video", false) && !quizPreview.includeSongs ) {
									quizPreview.includeSongs = true;
								}
							}
						}
					}
				}
			}
		} else if ( ctrl->first == "RowCategories" ) { // Load Row Categories
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