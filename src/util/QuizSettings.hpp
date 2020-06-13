#pragma once

namespace MusicQuiz {
	struct QuizSettings
	{
		/** Daily Double */
		bool dailyDouble = false;
		bool dailyDoubleHidden = true;
		size_t dailyDoublePercentage = 15;

		/** Daily Trible */
		bool dailyTriple = false;
		bool dailyTripleHidden = true;
		size_t dailyTriplePercentage = 5;

		/** Teams */
		bool hiddenTeamScore = false;

		/** Answers */
		bool hiddenAnswers = false;

		/** Guess the category */
		bool guessTheCategory = false;
		size_t pointsPerCategory = 500;
	};
}