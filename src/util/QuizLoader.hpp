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
