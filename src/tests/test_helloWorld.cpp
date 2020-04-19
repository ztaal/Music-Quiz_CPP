#include <iostream>


int main()
{
	std::cout << "Hello MusicQuiz World!" << std::endl;

#if defined(_WIN32) || defined(WIN32)
	system("pause");
#endif

	return 0;
}
