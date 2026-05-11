#ifndef SCORE_MANAGER_HPP
#define SCORE_MANAGER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept> // For Exception Handling

struct ScoreEntry {
    std::string name;
    int score;
};

class ScoreManager {
private:
    std::vector<ScoreEntry> highScores;
    const std::string fileName = "highscores.txt";

    // Requirement: Sorting Algorithm
    void sortScores();

public:
    ScoreManager();
    void loadScores();
    void saveScore(const std::string& name, int score);
    std::vector<ScoreEntry> getTopScores(int count) const;
};

#endif