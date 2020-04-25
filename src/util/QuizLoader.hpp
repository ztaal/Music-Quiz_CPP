#pragma once

#include <string>
#include <vector>

#include <boost/filesystem.hpp>


namespace MusicQuiz {
	namespace util {
		class QuizLoader
		{
		public:
			struct QuizPreview
			{
				std::string quizName = "";
				bool includeSongs = false;
				bool includeVideos = false;
				std::string quizDescription = "";
				std::vector<std::string> categories;
			};

			/**
			 * @brief Deleted constructor.
			 */
			QuizLoader() = delete;

			/**
			* @brief Deleted Destructor.
			*/
			~QuizLoader() = delete;

			/**
			* @brief Returns a list of quizzez stored in the data folder.
			*
			* @return The list of quizzes.
			*/
			static std::vector<std::string> getListOfQuizzes();

		protected:
			/** Variables */
		};


	}
}
