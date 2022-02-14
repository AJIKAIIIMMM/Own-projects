#ifndef VORTEX_H
#define VORTEX_H
#include <random>

class PRNG {
	std::mt19937 engine;
public:
	PRNG(){}
	void initGenerator(PRNG& generator) {
		const unsigned seed = unsigned(std::time(nullptr));
		generator.engine.seed(seed);
	}

	size_t getRandomIndex(PRNG& generator, size_t size) {
		std::uniform_int_distribution<size_t> distribution(0, size - 1);
		return static_cast<int>(distribution(generator.engine));
	}
};

#endif