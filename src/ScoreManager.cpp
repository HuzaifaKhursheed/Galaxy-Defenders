#include "ScoreManager.hpp"

ScoreManager::ScoreManager() {
    loadScores(); // Load scores immediately when the game starts
}

// Requirement: Exception Handling
void ScoreManager::loadScores() {
    highScores.clear();
    try {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            throw std::runtime_error("High score file not found. A new one will be created.");
        }

        std::string n;
        int s;
        while (file >> n >> s) {
            highScores.push_back({n, s});
        }
        file.close();
    } 
    catch (const std::exception& e) {
        // If the file doesn't exist yet, we catch the error safely instead of crashing!
        std::cerr << "File IO Exception Caught: " << e.what() << std::endl;
    }
}

void ScoreManager::saveScore(const std::string& name, int score) {
    highScores.push_back({name, score});
    sortScores(); // Sort them before saving

    std::ofstream file(fileName);
    // Save top 10 scores to prevent the file from getting too massive
    int limit = (highScores.size() < 10) ? highScores.size() : 10;
    for (int i = 0; i < limit; i++) {
        file << highScores[i].name << " " << highScores[i].score << "\n";
    }
    file.close();
}

// Requirement: Sorting Algorithm (Selection Sort)
void ScoreManager::sortScores() {
    int n = highScores.size();
    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (highScores[j].score > highScores[max_idx].score) {
                max_idx = j;
            }
        }
        // Swap
        ScoreEntry temp = highScores[max_idx];
        highScores[max_idx] = highScores[i];
        highScores[i] = temp;
    }
}

std::vector<ScoreEntry> ScoreManager::getTopScores(int count) const {
    int limit = (highScores.size() < count) ? highScores.size() : count;
    return std::vector<ScoreEntry>(highScores.begin(), highScores.begin() + limit);
}