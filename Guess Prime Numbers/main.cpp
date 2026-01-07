#include <iostream>
#include "Random.h"
#include <cmath>
#include <cassert>
#include <functional>
#include <string>
#include <optional>
#include <format>
#include <vector>

bool is_prime(int n)
{
	if (n == 2 || n == 3)
		return true;
	if (n <= 1 || n % 2 == 0 || n % 3 == 0)
		return false;
	
	for (int i{ 5 }; i * i <= n; ++i)
	{
		if (n % i == 0)
			return false;
	}

	return true;
}

int generate_prime_number()
{
	std::random_device re;
	std::mt19937 engine(re());
	std::uniform_int_distribution<int> dist(1, 99999);
	int n{};
	while (!is_prime(n))
	{
		n = Random::get(1,99999);
	}
	return n;
}

void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int input()
{
	int num{};
	while (!(std::cin >> num))
	{
		std::cin.clear();
		ignoreLine();
		std::cout << "Invalid inputs!!! Please enter a number: \n";
	}
	return num;
}

void guess_number(int number)
{
	std::cout << "Guess the number: \n";
	int guess = input();
	while (guess != number)
	{
		std::cout << guess << " is wrong. Try again\n";
		guess = input();
	}
	std::cout << "Well done. \n";
}

std::optional<int> read_number(std::istream& in)
{
	int result{};
	if (in >> result)
	{
		return result;
	}
	in.clear();
	in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return {};
}

void guess_number_or_give_up(int num)
{
	std::cout << "Guess the number or Enter any letter to quit the game: \n";
	std::optional<int> guess;
	while ((guess = read_number(std::cin)))
	{
		if (guess.value() == num)
		{
			std::cout << "Well done!";
			return;
		}
		std::cout << guess.value() << " is wrong. Try again.\n";
	}
	std::cout << "Exit the program. The number was " << num << '\n';
}

void guess_number_with_clues(int number, auto& messages)
{
	std::cout << "Guess the number or Enter any letter to quit. \n";
	std::optional <int> guess;
	while ((guess = read_number(std::cin)))
	{
		if (guess.value() == number)
		{
			std::cout << "Well done.";
			return;
		}
		std::cout << std::format("{:0>5}", (guess.value())) << " is wrong. Try again\n";
		auto clues = (messages[2])(guess.value());
		std::cout << clues;
		for (auto message : messages)
		{
			auto clue = message(guess.value());
			if (clue.length())
			{
				std::cout << clue;
				break;
			}
		}
	}
	std::cout << std::format("Exit the program!!! The number was {:0>5}\n", (number));
}

std::string check_which_digits_correct(int number, int guess)
{
	auto ns = std::format("{:0>5}", (number));
	auto gs = std::format("{:0>5}", (guess));
	std::string matches(5, '.');
	for (size_t i{ 0 }, stop{ gs.length() }; i < stop; ++i)
	{
		char guess_char = gs[i];
		if (guess_char == ns[i] && i < ns.length() )
		{
			matches[i] = '*';
			ns[i] = '*';
		}
	}
	for (size_t i{ 0 }, stop{ gs.length() }; i < stop; ++i)
	{
		char guess_char = gs[i];
		if (matches[i] != '*' && i < ns.length())
		{
			if (size_t idx{ ns.find(guess_char, 0) }; idx != std::string::npos)
			{
				matches[i] = '^';
				ns[idx] = '^';
			}
		}
	}
	return matches;
}

int main()
{
	const int number{ generate_prime_number() };
	auto check_digits = [&number](int guess) {
		return std::format("{}\n", check_which_digits_correct(number, guess));
		};

	auto check_prime = [](int guess) {
		return std::string((is_prime(guess)) ? "" : "Not prime\n");
		};

	auto check_length = [](int guess) {
		return std::string(guess < 100000 ? "" : "Too long\n");
		};

	std::vector<std::function<std::string(int)>> messages{
		check_length,
		check_prime,
		check_digits
	};

	guess_number_with_clues(generate_prime_number(), messages);
	return 0;
}