#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/*
==========================
Exercise 2: ReportManager
==========================
Refactor ReportManager Class

Problem: You have a ReportManager class that generates report data, formats it
into a specific output (CSV, JSON), and distributes it via email. These are
three distinct responsibilities bundled into one class. Your task is to refactor
it into ReportGenerator, ReportFormatter, and ReportDistributor.

Requirements:

ReportGenerator should gather and return raw report data (a list of records)
ReportFormatter should take raw data and format it as CSV text
ReportDistributor should take a formatted report and "send" it to a recipient
The main workflow should compose these three classes together
*/

// Before: One class doing three unrelated jobs
class ReportManager {
public:
  void createAndSendReport(const string &recipient) {
    // Responsibility 1: Generate report data
    vector<vector<string>> data = {{"Name", "Sales", "Region"},
                                   {"Alice", "15000", "North"},
                                   {"Bob", "22000", "South"},
                                   {"Charlie", "18000", "East"}};

    // Responsibility 2: Format as CSV
    stringstream csv;
    for (const auto &row : data) {
      for (size_t i = 0; i < row.size(); i++) {
        if (i > 0)
          csv << ",";
        csv << row[i];
      }
      csv << "\n";
    }

    // Responsibility 3: Distribute via email
    cout << "Sending report to: " << recipient << endl;
    cout << csv.str();
    cout << "Report sent successfully." << endl;
  }
};

// TODO: Refactor into ReportGenerator, ReportFormatter, and ReportDistributor.

// ReportGenerator gathers and return raw report data (a list of records)
class ReportGenerator {
public:
  vector<vector<string>> generate() {
    // Generate and return the data inside the function.
    // Do not need to store as a static variable of the class because this makes
    // the class slightly more stateful than needed
    vector<vector<string>> data = {{"Name", "Sales", "Region"},
                                   {"Alice", "15000", "North"},
                                   {"Bob", "22000", "South"},
                                   {"Charlie", "18000", "East"}};
    return data;
  }
};

// ReportFormatter takes raw data and format it as CSV text
class ReportFormatter {
public:
  string formatAsCsv(const vector<vector<string>> &data) {
    stringstream csv;
    for (const auto &row : data) {
      for (size_t i = 0; i < row.size(); i++) {
        if (i > 0)
          csv << ",";
        csv << row[i];
      }
      csv << "\n";
    }
    return csv.str();
  }
};

// ReportDistributor takes a formatted report and "send" it to a recipient
class ReportDistributor {
public:
  void distribute(const string &recipientEmail, const string &formattedReport) {
    cout << "Sending report to: " << recipientEmail << endl;
    cout << formattedReport;
    cout << "Report sent successfully." << endl;
  }
};

int main() {
  // Before refactoring
  // ReportManager reportManager;
  // reportManager.createAndSendReport("Soham Das");

  // After refactoring, usage should look like:
  ReportGenerator generator;
  ReportFormatter formatter;
  ReportDistributor distributor;
  auto data = generator.generate();
  auto formatted = formatter.formatAsCsv(data);
  distributor.distribute("manager@company.com", formatted);
  return 0;
}