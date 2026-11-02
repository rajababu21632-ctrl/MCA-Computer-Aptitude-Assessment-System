#ifndef EXAM_ENGINE_H
#define EXAM_ENGINE_H

#include <string>
#include <vector>
#include "Question.h"
#include "Student.h"
#include "ResultManager.h"

class ExamEngine {
private:
    std::string questionsFilePath;
    std::vector<Question> questionBank;
    ResultManager& resultManager;

    void loadQuestions();
    void saveQuestions();

public:
    explicit ExamEngine(ResultManager& rm, const std::string& path = "data/questions.txt");

    // Question Bank Management
    void addQuestionWizard();
    void deleteQuestionWizard();
    void listAllQuestions() const;
    const std::vector<Question>& getQuestions() const { return questionBank; }
    std::vector<std::string> getAvailableCategories() const;

    // Test Execution
    void startExam(const Student& student, const std::string& categoryFilter = "");
    void showSyllabus() const;
};

#endif // EXAM_ENGINE_H
