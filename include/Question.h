#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

class Question {
private:
    int id;
    std::string category;
    std::string text;
    std::vector<std::string> options; // 4 options
    char correctOption;               // 'A', 'B', 'C', 'D'
    std::string explanation;
    int marks;

public:
    Question();
    Question(int id, const std::string& cat, const std::string& txt,
             const std::vector<std::string>& opts, char correct,
             const std::string& exp, int marks = 1);

    // Getters
    int getId() const { return id; }
    std::string getCategory() const { return category; }
    std::string getText() const { return text; }
    std::vector<std::string> getOptions() const { return options; }
    char getCorrectOption() const { return correctOption; }
    std::string getExplanation() const { return explanation; }
    int getMarks() const { return marks; }

    // Setters
    void setId(int newId) { id = newId; }
    void setCategory(const std::string& cat) { category = cat; }
    void setText(const std::string& txt) { text = txt; }
    void setOptions(const std::vector<std::string>& opts) { options = opts; }
    void setCorrectOption(char opt) { correctOption = opt; }
    void setExplanation(const std::string& exp) { explanation = exp; }
    void setMarks(int m) { marks = m; }

    // Helpers
    void display(int displayIndex) const;
    std::string serialize() const;
    static Question deserialize(const std::string& line);
};

#endif // QUESTION_H
