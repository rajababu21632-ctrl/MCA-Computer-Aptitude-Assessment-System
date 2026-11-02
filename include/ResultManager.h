#ifndef RESULT_MANAGER_H
#define RESULT_MANAGER_H

#include <string>
#include <vector>

struct ResultRecord {
    std::string timestamp;
    std::string username;
    std::string studentName;
    std::string studentUid;
    std::string examType;
    int score;
    int totalMarks;
    double percentage;
    std::string grade;
    std::string status; // PASS / FAIL

    std::string serialize() const;
    static ResultRecord deserialize(const std::string& line);
};

class ResultManager {
private:
    std::string filePath;
    std::vector<ResultRecord> records;

    void loadRecords();
    void saveRecords();

public:
    explicit ResultManager(const std::string& path = "data/results.txt");

    void addResult(const ResultRecord& record);
    std::vector<ResultRecord> getAllResults();
    std::vector<ResultRecord> getResultsByUsername(const std::string& username);
    std::vector<ResultRecord> getResultsByUid(const std::string& uid);

    void displayLeaderboard();
    void displayStudentHistory(const std::string& username);
    void displayAllSubmissions();
    void displayBatchAnalytics();
    void searchStudentReport(const std::string& query);

    static std::string calculateGrade(double percentage);
};

#endif // RESULT_MANAGER_H
