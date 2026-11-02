#include "Question.h"
#include "Utils.h"
#include <iostream>
#include <sstream>

Question::Question() : id(0), correctOption('A'), marks(1) {}

Question::Question(int id, const std::string& cat, const std::string& txt,
                   const std::vector<std::string>& opts, char correct,
                   const std::string& exp, int marks)
    : id(id), category(cat), text(txt), options(opts),
      correctOption(correct), explanation(exp), marks(marks) {}

void Question::display(int displayIndex) const {
    std::cout << Utils::BOLD << Utils::CYAN << "Q" << displayIndex << ". [" 
              << category << "] (" << marks << " Mark" << (marks > 1 ? "s" : "") << ")\n" << Utils::RESET;
    std::cout << Utils::BOLD << "   " << text << "\n" << Utils::RESET;
    std::cout << "\n";

    char labels[] = {'A', 'B', 'C', 'D'};
    for (size_t i = 0; i < options.size() && i < 4; ++i) {
        std::cout << "   " << Utils::YELLOW << "[" << labels[i] << "] " << Utils::RESET 
                  << options[i] << "\n";
    }
    std::cout << "\n";
}

std::string Question::serialize() const {
    // Format: id|category|text|optA|optB|optC|optD|correct|explanation|marks
    std::ostringstream oss;
    oss << id << "|"
        << category << "|"
        << text << "|"
        << (options.size() > 0 ? options[0] : "") << "|"
        << (options.size() > 1 ? options[1] : "") << "|"
        << (options.size() > 2 ? options[2] : "") << "|"
        << (options.size() > 3 ? options[3] : "") << "|"
        << correctOption << "|"
        << explanation << "|"
        << marks;
    return oss.str();
}

Question Question::deserialize(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(line);
    while (std::getline(tokenStream, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 10) {
        return Question();
    }

    int id = std::stoi(tokens[0]);
    std::string cat = tokens[1];
    std::string txt = tokens[2];
    std::vector<std::string> opts = {tokens[3], tokens[4], tokens[5], tokens[6]};
    char correct = tokens[7].empty() ? 'A' : tokens[7][0];
    std::string exp = tokens[8];
    int marks = std::stoi(tokens[9]);

    return Question(id, cat, txt, opts, correct, exp, marks);
}
