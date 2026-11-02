#include "ExamEngine.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <map>

ExamEngine::ExamEngine(ResultManager& rm, const std::string& path)
    : questionsFilePath(path), resultManager(rm) {
    loadQuestions();
}

void ExamEngine::loadQuestions() {
    questionBank.clear();
    std::ifstream inFile(questionsFilePath);
    if (!inFile.is_open()) return;

    std::string line;
    while (std::getline(inFile, line)) {
        line = Utils::trim(line);
        if (!line.empty() && line[0] != '#') {
            Question q = Question::deserialize(line);
            if (q.getId() > 0) {
                questionBank.push_back(q);
            }
        }
    }
    inFile.close();
}

void ExamEngine::saveQuestions() {
    std::ofstream outFile(questionsFilePath, std::ios::trunc);
    if (!outFile.is_open()) return;

    for (const auto& q : questionBank) {
        outFile << q.serialize() << "\n";
    }
    outFile.close();
}

std::vector<std::string> ExamEngine::getAvailableCategories() const {
    std::vector<std::string> categories;
    for (const auto& q : questionBank) {
        if (std::find(categories.begin(), categories.end(), q.getCategory()) == categories.end()) {
            categories.push_back(q.getCategory());
        }
    }
    return categories;
}

void ExamEngine::listAllQuestions() const {
    Utils::printHeader("QUESTION BANK REPOSITORY");
    if (questionBank.empty()) {
        std::cout << Utils::YELLOW << " No questions currently loaded in the database.\n" << Utils::RESET;
        Utils::pause();
        return;
    }

    std::cout << Utils::GREEN << " Total Questions Available: " << questionBank.size() << "\n\n" << Utils::RESET;
    for (size_t i = 0; i < questionBank.size(); ++i) {
        std::cout << Utils::BOLD << "[" << (i + 1) << "] ID: #" << questionBank[i].getId()
                  << " | Category: " << questionBank[i].getCategory()
                  << " | Marks: " << questionBank[i].getMarks() << Utils::RESET << "\n";
        std::cout << "    Question : " << questionBank[i].getText() << "\n";
        const auto& opts = questionBank[i].getOptions();
        char lbl = 'A';
        for (const auto& opt : opts) {
            std::cout << "    [" << lbl++ << "] " << opt << "\n";
        }
        std::cout << "    Correct  : " << Utils::GREEN << questionBank[i].getCorrectOption() << Utils::RESET << "\n";
        std::cout << "    Explanation: " << Utils::CYAN << questionBank[i].getExplanation() << Utils::RESET << "\n";
        Utils::printDivider('-', 70);
    }
    Utils::pause();
}

void ExamEngine::addQuestionWizard() {
    Utils::printHeader("ADD NEW APTITUDE QUESTION WIZARD");

    int newId = 1;
    for (const auto& q : questionBank) {
        if (q.getId() >= newId) {
            newId = q.getId() + 1;
        }
    }

    std::cout << "Auto-assigned Question ID: " << Utils::BOLD << newId << Utils::RESET << "\n\n";

    std::cout << "Select Domain / Category:\n";
    std::cout << " 1. Computer Fundamentals & Architecture\n";
    std::cout << " 2. C++ & Object-Oriented Programming\n";
    std::cout << " 3. Data Structures & Algorithms\n";
    std::cout << " 4. Operating Systems & System Software\n";
    std::cout << " 5. Database Systems & SQL\n";
    std::cout << " 6. Custom Category\n";
    int catChoice = Utils::getSafeInt(1, 6, "Select option (1-6): ");

    std::string category;
    switch (catChoice) {
        case 1: category = "Computer Fundamentals"; break;
        case 2: category = "C++ OOP Concepts"; break;
        case 3: category = "Data Structures"; break;
        case 4: category = "Operating Systems"; break;
        case 5: category = "Database Management"; break;
        case 6: category = Utils::getSafeString("Enter Custom Category Name: "); break;
    }

    std::string text = Utils::getSafeString("Enter Question Statement: ");

    std::cout << "\nEnter Options:\n";
    std::string optA = Utils::getSafeString(" Option [A]: ");
    std::string optB = Utils::getSafeString(" Option [B]: ");
    std::string optC = Utils::getSafeString(" Option [C]: ");
    std::string optD = Utils::getSafeString(" Option [D]: ");

    char correct = Utils::getSafeChar("ABCD", "Enter Correct Option (A/B/C/D): ");
    std::string explanation = Utils::getSafeString("Enter Brief Technical Explanation: ");
    int marks = Utils::getSafeInt(1, 5, "Enter Marks for this question (1-5): ");

    Question newQ(newId, category, text, {optA, optB, optC, optD}, correct, explanation, marks);
    questionBank.push_back(newQ);
    saveQuestions();

    std::cout << "\n" << Utils::GREEN << " [SUCCESS] Question #" << newId << " added to bank successfully!\n" << Utils::RESET;
    Utils::pause();
}

void ExamEngine::deleteQuestionWizard() {
    Utils::printHeader("DELETE QUESTION FROM BANK");
    if (questionBank.empty()) {
        std::cout << Utils::YELLOW << " Question bank is empty.\n" << Utils::RESET;
        Utils::pause();
        return;
    }

    int targetId = Utils::getSafeInt(1, 100000, "Enter Question ID to delete (0 to cancel): ");
    if (targetId == 0) return;

    auto it = std::find_if(questionBank.begin(), questionBank.end(), [targetId](const Question& q) {
        return q.getId() == targetId;
    });

    if (it != questionBank.end()) {
        std::cout << "\nFound Question: " << Utils::YELLOW << it->getText() << Utils::RESET << "\n";
        char confirm = Utils::getSafeChar("YN", "Are you sure you want to delete this question? (Y/N): ");
        if (confirm == 'Y') {
            questionBank.erase(it);
            saveQuestions();
            std::cout << Utils::GREEN << " [SUCCESS] Question deleted successfully.\n" << Utils::RESET;
        } else {
            std::cout << " Deletion cancelled.\n";
        }
    } else {
        std::cout << Utils::RED << " [ERROR] Question ID #" << targetId << " not found in database.\n" << Utils::RESET;
    }
    Utils::pause();
}

void ExamEngine::startExam(const Student& student, const std::string& categoryFilter) {
    std::vector<Question> activeQuestions;
    for (const auto& q : questionBank) {
        if (categoryFilter.empty() || q.getCategory() == categoryFilter) {
            activeQuestions.push_back(q);
        }
    }

    std::string examTitle = categoryFilter.empty() ? "Comprehensive MCA Computer Aptitude Assessment" 
                                                   : ("Module Test: " + categoryFilter);

    Utils::printHeader(examTitle);

    if (activeQuestions.empty()) {
        std::cout << Utils::RED << " No questions available for the selected assessment module.\n" << Utils::RESET;
        Utils::pause();
        return;
    }

    std::cout << Utils::BOLD << "Candidate : " << student.getFullName() << " (UID: " << student.getUid() << ")\n" << Utils::RESET;
    std::cout << "Questions : " << activeQuestions.size() << " Questions\n";
    std::cout << "Format    : 4 Options (A/B/C/D) or 'S' to Skip\n";
    std::cout << "Grading   : Full marks for correct, 0 for skipped/wrong.\n\n";
    char ready = Utils::getSafeChar("YN", "Are you ready to begin? (Y/N): ");
    if (ready != 'Y') {
        std::cout << " Assessment cancelled.\n";
        Utils::pause();
        return;
    }

    int totalScore = 0;
    int maxPossibleMarks = 0;
    int correctCount = 0;
    int incorrectCount = 0;
    int skippedCount = 0;

    std::vector<char> userAnswers;
    std::map<std::string, std::pair<int, int>> categoryStats; // Category -> {correct, total}

    for (size_t i = 0; i < activeQuestions.size(); ++i) {
        const Question& q = activeQuestions[i];
        maxPossibleMarks += q.getMarks();
        categoryStats[q.getCategory()].second += q.getMarks();

        Utils::printHeader(examTitle + " | Progress: " + std::to_string(i + 1) + "/" + std::to_string(activeQuestions.size()));
        q.display(i + 1);

        char ans = Utils::getSafeChar("ABCDS", "Your Answer (A/B/C/D) or [S] to Skip: ");
        userAnswers.push_back(ans);

        if (ans == 'S') {
            skippedCount++;
        } else if (ans == q.getCorrectOption()) {
            correctCount++;
            totalScore += q.getMarks();
            categoryStats[q.getCategory()].first += q.getMarks();
        } else {
            incorrectCount++;
        }
    }

    double percentage = (maxPossibleMarks > 0) 
        ? ((static_cast<double>(totalScore) / maxPossibleMarks) * 100.0) 
        : 0.0;
    std::string grade = ResultManager::calculateGrade(percentage);
    std::string status = (percentage >= 40.0) ? "PASS" : "FAIL";

    // Record Result
    ResultRecord rec;
    rec.timestamp   = Utils::getCurrentDateTime();
    rec.username    = student.getUsername();
    rec.studentName = student.getFullName();
    rec.studentUid  = student.getUid();
    rec.examType    = categoryFilter.empty() ? "Full Assessment" : categoryFilter;
    rec.score       = totalScore;
    rec.totalMarks  = maxPossibleMarks;
    rec.percentage  = percentage;
    rec.grade       = grade;
    rec.status      = status;
    resultManager.addResult(rec);

    // Scorecard Display
    Utils::printHeader("OFFICIAL ASSESSMENT SCORECARD - MCA GENERAL");
    std::cout << Utils::BOLD << " Student Name : " << student.getFullName() << "\n";
    std::cout << " Student UID  : " << student.getUid() << "\n";
    std::cout << " Date & Time  : " << rec.timestamp << "\n";
    std::cout << " Exam Module  : " << rec.examType << "\n" << Utils::RESET;
    Utils::printDivider('-', 74);

    std::cout << std::left
              << std::setw(25) << " Total Questions" << ": " << activeQuestions.size() << "\n"
              << std::setw(25) << " Questions Attempted" << ": " << (correctCount + incorrectCount) << "\n"
              << std::setw(25) << " Correct Answers" << ": " << Utils::GREEN << correctCount << Utils::RESET << "\n"
              << std::setw(25) << " Incorrect Answers" << ": " << Utils::RED << incorrectCount << Utils::RESET << "\n"
              << std::setw(25) << " Skipped Questions" << ": " << Utils::YELLOW << skippedCount << Utils::RESET << "\n";
    Utils::printDivider('-', 74);

    std::cout << std::left
              << std::setw(25) << " Final Score" << ": " << Utils::BOLD << totalScore << " / " << maxPossibleMarks << Utils::RESET << "\n"
              << std::setw(25) << " Percentage" << ": " << Utils::BOLD << std::fixed << std::setprecision(2) << percentage << "%" << Utils::RESET << "\n"
              << std::setw(25) << " Assigned Grade" << ": " << Utils::BOLD << Utils::CYAN << grade << Utils::RESET << "\n"
              << std::setw(25) << " Evaluation Status" << ": " 
              << (status == "PASS" ? Utils::GREEN : Utils::RED) << Utils::BOLD << status << Utils::RESET << "\n";
    Utils::printDivider('=', 74);

    // Module-wise Diagnostic Analysis
    std::cout << Utils::BOLD << "\n>> TOPIC-WISE COMPETENCY BREAKDOWN:\n" << Utils::RESET;
    for (const auto& pair : categoryStats) {
        double catPercent = (pair.second.second > 0) 
            ? ((static_cast<double>(pair.second.first) / pair.second.second) * 100.0) 
            : 0.0;
        std::cout << "  * " << std::left << std::setw(32) << pair.first << ": "
                  << pair.second.first << "/" << pair.second.second << " marks (" 
                  << std::setw(5) << (int)catPercent << "%) ";

        if (catPercent >= 80.0) std::cout << Utils::GREEN << "[Proficient / Strong Area]" << Utils::RESET << "\n";
        else if (catPercent >= 50.0) std::cout << Utils::YELLOW << "[Moderate / Good]" << Utils::RESET << "\n";
        else std::cout << Utils::RED << "[Needs Revision]" << Utils::RESET << "\n";
    }
    std::cout << "\n";

    // Answer Review Option
    char reviewChoice = Utils::getSafeChar("YN", "Would you like to review answer keys & technical explanations? (Y/N): ");
    if (reviewChoice == 'Y') {
        Utils::printHeader("DETAILED EXAM ANSWER KEY & TECHNICAL EXPLANATIONS");
        for (size_t i = 0; i < activeQuestions.size(); ++i) {
            const Question& q = activeQuestions[i];
            char userAns = userAnswers[i];
            bool isCorrect = (userAns == q.getCorrectOption());

            std::cout << Utils::BOLD << "Q" << (i + 1) << ". " << q.getText() << Utils::RESET << "\n";
            std::cout << "   Your Answer : ";
            if (userAns == 'S') {
                std::cout << Utils::YELLOW << "[Skipped]" << Utils::RESET;
            } else if (isCorrect) {
                std::cout << Utils::GREEN << "[" << userAns << "] (Correct)" << Utils::RESET;
            } else {
                std::cout << Utils::RED << "[" << userAns << "] (Incorrect)" << Utils::RESET;
            }
            std::cout << " | Correct Answer: " << Utils::GREEN << "[" << q.getCorrectOption() << "]" << Utils::RESET << "\n";
            std::cout << "   " << Utils::CYAN << "Explanation: " << q.getExplanation() << Utils::RESET << "\n";
            Utils::printDivider('-', 74);
        }
    }

    std::cout << "\n" << Utils::GREEN << " Result has been recorded to university grade database." << Utils::RESET << "\n";
    Utils::pause();
}

void ExamEngine::showSyllabus() const {
    Utils::printHeader("COMPUTER APTITUDE ASSESSMENT SYLLABUS (CU MCA)");
    std::cout << Utils::BOLD << "Course Modules Covered in Examination System:\n" << Utils::RESET;
    std::cout << " 1. Computer Fundamentals & Digital Logic:\n";
    std::cout << "    - Binary, Hexadecimal, 2's Complement representations\n";
    std::cout << "    - CPU registers, cache hierarchy, Von Neumann architecture\n\n";

    std::cout << " 2. C++ & Object-Oriented Programming (OOP):\n";
    std::cout << "    - Classes, Objects, Access specifiers, Constructors/Destructors\n";
    std::cout << "    - Inheritance, Virtual functions, Polymorphism, Abstract classes\n";
    std::cout << "    - Memory management, pointers, references, templates\n\n";

    std::cout << " 3. Data Structures & Algorithms:\n";
    std::cout << "    - Arrays, Linked Lists, Stacks, Queues, Trees, Binary Search Trees\n";
    std::cout << "    - Time & Space Complexity analysis (Big-O notation)\n";
    std::cout << "    - Sorting algorithms (Merge sort, Quick sort, Heap sort)\n\n";

    std::cout << " 4. Operating Systems & Networking:\n";
    std::cout << "    - Process management, threads, deadlock conditions, virtual memory\n";
    std::cout << "    - OSI 7-layer model, TCP/IP, IP addressing, socket basics\n\n";

    std::cout << " 5. Database Management Systems (DBMS):\n";
    std::cout << "    - Relational model, Keys (Primary, Foreign, Candidate)\n";
    std::cout << "    - Normalization (1NF, 2NF, 3NF, BCNF), ACID properties, SQL\n\n";

    Utils::printDivider('=', 74);
    std::cout << Utils::CYAN << "Grading Scale: A+ (>=90%), A (>=80%), B+ (>=70%), B (>=60%), C (>=50%), P (>=40%), F (<40%)\n" << Utils::RESET;
    Utils::pause();
}
