#include "ResultManager.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

std::string ResultRecord::serialize() const {
    // Format: timestamp|username|studentName|studentUid|examType|score|totalMarks|percentage|grade|status
    std::ostringstream oss;
    oss << timestamp << "|"
        << username << "|"
        << studentName << "|"
        << studentUid << "|"
        << examType << "|"
        << score << "|"
        << totalMarks << "|"
        << std::fixed << std::setprecision(2) << percentage << "|"
        << grade << "|"
        << status;
    return oss.str();
}

ResultRecord ResultRecord::deserialize(const std::string& line) {
    ResultRecord rec;
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream ss(line);
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }
    if (tokens.size() >= 10) {
        rec.timestamp   = tokens[0];
        rec.username    = tokens[1];
        rec.studentName = tokens[2];
        rec.studentUid  = tokens[3];
        rec.examType    = tokens[4];
        rec.score       = std::stoi(tokens[5]);
        rec.totalMarks  = std::stoi(tokens[6]);
        rec.percentage  = std::stod(tokens[7]);
        rec.grade       = tokens[8];
        rec.status      = tokens[9];
    }
    return rec;
}

ResultManager::ResultManager(const std::string& path) : filePath(path) {
    loadRecords();
}

void ResultManager::loadRecords() {
    records.clear();
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) return;

    std::string line;
    while (std::getline(inFile, line)) {
        line = Utils::trim(line);
        if (!line.empty()) {
            records.push_back(ResultRecord::deserialize(line));
        }
    }
    inFile.close();
}

void ResultManager::saveRecords() {
    std::ofstream outFile(filePath, std::ios::trunc);
    if (!outFile.is_open()) return;

    for (const auto& rec : records) {
        outFile << rec.serialize() << "\n";
    }
    outFile.close();
}

void ResultManager::addResult(const ResultRecord& record) {
    records.push_back(record);
    std::ofstream outFile(filePath, std::ios::app);
    if (outFile.is_open()) {
        outFile << record.serialize() << "\n";
        outFile.close();
    }
}

std::vector<ResultRecord> ResultManager::getAllResults() {
    loadRecords();
    return records;
}

std::vector<ResultRecord> ResultManager::getResultsByUsername(const std::string& username) {
    loadRecords();
    std::vector<ResultRecord> userRecs;
    for (const auto& r : records) {
        if (r.username == username) {
            userRecs.push_back(r);
        }
    }
    return userRecs;
}

std::vector<ResultRecord> ResultManager::getResultsByUid(const std::string& uid) {
    loadRecords();
    std::vector<ResultRecord> userRecs;
    for (const auto& r : records) {
        if (r.studentUid == uid) {
            userRecs.push_back(r);
        }
    }
    return userRecs;
}

std::string ResultManager::calculateGrade(double percentage) {
    if (percentage >= 90.0) return "A+";
    if (percentage >= 80.0) return "A";
    if (percentage >= 70.0) return "B+";
    if (percentage >= 60.0) return "B";
    if (percentage >= 50.0) return "C";
    if (percentage >= 40.0) return "P";
    return "F";
}

void ResultManager::displayLeaderboard() {
    loadRecords();
    Utils::printHeader("GLOBAL MERIT LIST / LEADERBOARD");

    if (records.empty()) {
        std::cout << Utils::YELLOW << " No assessment attempts recorded yet.\n" << Utils::RESET;
        Utils::pause();
        return;
    }

    // Sort by percentage descending, then total score
    std::vector<ResultRecord> sorted = records;
    std::sort(sorted.begin(), sorted.end(), [](const ResultRecord& a, const ResultRecord& b) {
        if (a.percentage != b.percentage)
            return a.percentage > b.percentage;
        return a.score > b.score;
    });

    std::cout << std::left 
              << std::setw(6)  << "Rank"
              << std::setw(15) << "UID"
              << std::setw(20) << "Student Name"
              << std::setw(22) << "Exam Module"
              << std::setw(10) << "Score"
              << std::setw(10) << "%age"
              << std::setw(8)  << "Grade"
              << std::setw(8)  << "Status" << "\n";
    Utils::printDivider('-', 95);

    int rank = 1;
    for (const auto& r : sorted) {
        std::string rankStr = "#" + std::to_string(rank);
        if (rank == 1) rankStr = "[1st]";
        else if (rank == 2) rankStr = "[2nd]";
        else if (rank == 3) rankStr = "[3rd]";

        std::string color = (rank == 1) ? Utils::YELLOW : (rank <= 3 ? Utils::GREEN : Utils::RESET);

        std::cout << color << std::left
                  << std::setw(6)  << rankStr
                  << std::setw(15) << r.studentUid
                  << std::setw(20) << (r.studentName.length() > 18 ? r.studentName.substr(0, 15) + "..." : r.studentName)
                  << std::setw(22) << (r.examType.length() > 20 ? r.examType.substr(0, 18) + "..." : r.examType)
                  << std::setw(10) << (std::to_string(r.score) + "/" + std::to_string(r.totalMarks))
                  << std::setw(10) << (std::to_string((int)r.percentage) + "%")
                  << std::setw(8)  << r.grade
                  << std::setw(8)  << r.status
                  << Utils::RESET << "\n";
        rank++;
    }
    Utils::printDivider('=', 95);
    Utils::pause();
}

void ResultManager::displayStudentHistory(const std::string& username) {
    loadRecords();
    Utils::printHeader("MY ASSESSMENT HISTORY & SCORECARDS");

    std::vector<ResultRecord> myRecs = getResultsByUsername(username);
    if (myRecs.empty()) {
        std::cout << Utils::YELLOW << " You have not appeared for any aptitude tests yet.\n" << Utils::RESET;
        Utils::pause();
        return;
    }

    std::cout << std::left
              << std::setw(22) << "Date & Time"
              << std::setw(22) << "Module / Exam"
              << std::setw(12) << "Score"
              << std::setw(12) << "Percentage"
              << std::setw(8)  << "Grade"
              << std::setw(8)  << "Status" << "\n";
    Utils::printDivider('-', 84);

    for (const auto& r : myRecs) {
        std::string statusColor = (r.status == "PASS") ? Utils::GREEN : Utils::RED;
        std::cout << std::left
                  << std::setw(22) << r.timestamp
                  << std::setw(22) << (r.examType.length() > 20 ? r.examType.substr(0, 18) + "..." : r.examType)
                  << std::setw(12) << (std::to_string(r.score) + "/" + std::to_string(r.totalMarks))
                  << std::setw(12) << (std::to_string((int)r.percentage) + "%")
                  << std::setw(8)  << r.grade
                  << statusColor << std::setw(8) << r.status << Utils::RESET << "\n";
    }
    Utils::printDivider('=', 84);
    Utils::pause();
}

void ResultManager::displayAllSubmissions() {
    loadRecords();
    Utils::printHeader("ALL STUDENT TEST SUBMISSIONS (FACULTY VIEW)");

    if (records.empty()) {
        std::cout << Utils::YELLOW << " No submissions found in the database.\n" << Utils::RESET;
        Utils::pause();
        return;
    }

    std::cout << std::left
              << std::setw(20) << "Date & Time"
              << std::setw(14) << "UID"
              << std::setw(18) << "Student Name"
              << std::setw(20) << "Module"
              << std::setw(10) << "Score"
              << std::setw(10) << "Result"
              << std::setw(6)  << "Grade" << "\n";
    Utils::printDivider('-', 98);

    for (const auto& r : records) {
        std::string statusColor = (r.status == "PASS") ? Utils::GREEN : Utils::RED;
        std::cout << std::left
                  << std::setw(20) << r.timestamp
                  << std::setw(14) << r.studentUid
                  << std::setw(18) << (r.studentName.length() > 16 ? r.studentName.substr(0, 14) + ".." : r.studentName)
                  << std::setw(20) << (r.examType.length() > 18 ? r.examType.substr(0, 16) + ".." : r.examType)
                  << std::setw(10) << (std::to_string(r.score) + "/" + std::to_string(r.totalMarks))
                  << statusColor << std::setw(10) << r.status << Utils::RESET
                  << std::setw(6)  << r.grade << "\n";
    }
    Utils::printDivider('=', 98);
    Utils::pause();
}

void ResultManager::displayBatchAnalytics() {
    loadRecords();
    Utils::printHeader("MCA BATCH PERFORMANCE ANALYTICS & STATS");

    if (records.empty()) {
        std::cout << Utils::YELLOW << " No test data available to generate statistics.\n" << Utils::RESET;
        Utils::pause();
        return;
    }

    int totalAttempts = records.size();
    double totalPercent = 0.0;
    double maxPercent = -1.0;
    double minPercent = 999.0;
    std::string topStudent = "";
    int passedCount = 0;

    for (const auto& r : records) {
        totalPercent += r.percentage;
        if (r.percentage > maxPercent) {
            maxPercent = r.percentage;
            topStudent = r.studentName + " (" + r.studentUid + ")";
        }
        if (r.percentage < minPercent) {
            minPercent = r.percentage;
        }
        if (r.status == "PASS") {
            passedCount++;
        }
    }

    double avgPercent = totalPercent / totalAttempts;
    double passRate = (static_cast<double>(passedCount) / totalAttempts) * 100.0;

    std::cout << Utils::BOLD << " Summary of MCA Computer Aptitude Performance:\n" << Utils::RESET;
    Utils::printDivider('-', 60);
    std::cout << " Total Test Attempts Logged  : " << Utils::CYAN << totalAttempts << Utils::RESET << "\n";
    std::cout << " Overall Batch Average Score : " << Utils::YELLOW << std::fixed << std::setprecision(2) << avgPercent << "%" << Utils::RESET << "\n";
    std::cout << " Batch Highest Percentage    : " << Utils::GREEN << maxPercent << "%" << Utils::RESET << "\n";
    std::cout << " Topper / Highest Achiever   : " << Utils::BOLD << topStudent << Utils::RESET << "\n";
    std::cout << " Lowest Percentage Recorded  : " << Utils::RED << minPercent << "%" << Utils::RESET << "\n";
    std::cout << " Overall Batch Pass Rate     : " << (passRate >= 60 ? Utils::GREEN : Utils::RED)
              << passRate << "% (" << passedCount << "/" << totalAttempts << ")" << Utils::RESET << "\n";
    Utils::printDivider('=', 60);
    Utils::pause();
}

void ResultManager::searchStudentReport(const std::string& query) {
    loadRecords();
    Utils::printHeader("STUDENT INDIVIDUAL PERFORMANCE REPORT");

    std::vector<ResultRecord> found;
    for (const auto& r : records) {
        if (r.username == query || r.studentUid == query || r.studentName.find(query) != std::string::npos) {
            found.push_back(r);
        }
    }

    if (found.empty()) {
        std::cout << Utils::RED << " No records found matching query: " << query << "\n" << Utils::RESET;
        Utils::pause();
        return;
    }

    std::cout << Utils::GREEN << " Found " << found.size() << " test attempt(s) for query: " << query << "\n\n" << Utils::RESET;
    std::cout << std::left
              << std::setw(20) << "Date"
              << std::setw(15) << "UID"
              << std::setw(18) << "Name"
              << std::setw(20) << "Exam"
              << std::setw(10) << "Score"
              << std::setw(10) << "%age"
              << std::setw(8)  << "Grade" << "\n";
    Utils::printDivider('-', 91);

    for (const auto& r : found) {
        std::cout << std::left
                  << std::setw(20) << r.timestamp
                  << std::setw(15) << r.studentUid
                  << std::setw(18) << r.studentName
                  << std::setw(20) << r.examType
                  << std::setw(10) << (std::to_string(r.score) + "/" + std::to_string(r.totalMarks))
                  << std::setw(10) << (std::to_string((int)r.percentage) + "%")
                  << std::setw(8)  << r.grade << "\n";
    }
    Utils::printDivider('=', 91);
    Utils::pause();
}
