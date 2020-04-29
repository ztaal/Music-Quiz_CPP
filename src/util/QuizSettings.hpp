#pragma once

namespace MusicQuiz {
	struct QuizSettings
	{
		/** Daily Double */
		bool dailyDouble = false;
		bool dailyDoubleHidden = false;
		bool dailyDoublePercentage = 0;

		/** Daily Trible */
		bool dailyTriple = false;
		bool dailyTripleHidden = false;
		bool dailyTriplePercentage = 0;

		/** Teams */
		bool hiddenTeamScore = false;

		/** Answers */
		bool hiddenAnswers = false;

		/** Guess the category */
		bool guessTheCategory = false;
		size_t pointsPerCategory = 500;
	};
}
