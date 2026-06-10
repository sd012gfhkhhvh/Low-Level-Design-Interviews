/*
==================================================
Simple Factory Pattern
Document Export System
==================================================

Problem:
A reporting service needs to export reports in multiple formats
(PDF, HTML, CSV).

Instead of creating concrete document classes directly inside
client code, we centralize object creation inside a single
factory class.

This is NOT the GoF Factory Method pattern.
This is the commonly used Simple Factory pattern.
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

/*--------------------------------------------------
 Product Interface
--------------------------------------------------*/
class Document {
public:
  virtual string getHeader() = 0;
  virtual string formatRow(const vector<string> &data) = 0;
  virtual string getFooter() = 0;
  virtual string getFileExtension() = 0;

  virtual ~Document() = default;
};

/*--------------------------------------------------
 Concrete Products
--------------------------------------------------*/
class PdfDocument : public Document {
public:
  string getHeader() override { return "--- PDF DOCUMENT START ---"; }

  string formatRow(const vector<string> &data) override {
    string result = "| ";

    for (size_t i = 0; i < data.size(); i++) {
      result += data[i];

      if (i != data.size() - 1) {
        result += " | ";
      }
    }

    result += " |";
    return result;
  }

  string getFooter() override { return "--- PDF DOCUMENT END ---"; }

  string getFileExtension() override { return ".pdf"; }
};

class HtmlDocument : public Document {
public:
  string getHeader() override { return "<html><body><table>"; }

  string formatRow(const vector<string> &data) override {
    string result = "<tr>";

    for (const auto &cell : data) {
      result += "<td>" + cell + "</td>";
    }

    result += "</tr>";

    return result;
  }

  string getFooter() override { return "</table></body></html>"; }

  string getFileExtension() override { return ".html"; }
};

class CsvDocument : public Document {
public:
  string getHeader() override { return ""; }

  string formatRow(const vector<string> &data) override {
    string result;

    for (size_t i = 0; i < data.size(); i++) {
      result += data[i];

      if (i != data.size() - 1) {
        result += ",";
      }
    }

    return result;
  }

  string getFooter() override { return ""; }

  string getFileExtension() override { return ".csv"; }
};

/*--------------------------------------------------
 Document Type
--------------------------------------------------*/
enum class DocumentType { PDF, HTML, CSV };

/*--------------------------------------------------
 Simple Factory
--------------------------------------------------*/
class DocumentFactory {
public:
  static unique_ptr<Document> createDocument(DocumentType type) {
    switch (type) {
    case DocumentType::PDF:
      return make_unique<PdfDocument>();

    case DocumentType::HTML:
      return make_unique<HtmlDocument>();

    case DocumentType::CSV:
      return make_unique<CsvDocument>();

    default:
      throw invalid_argument("Unsupported document type");
    }
  }
};

/*--------------------------------------------------
 Business Service
--------------------------------------------------*/
class ReportExporter {
public:
  void exportData(const vector<vector<string>> &data, DocumentType type) {

    auto document = DocumentFactory::createDocument(type);

    cout << "Exporting to " << document->getFileExtension() << " format...\n";

    string header = document->getHeader();

    if (!header.empty()) {
      cout << header << endl;
    }

    for (const auto &row : data) {
      cout << document->formatRow(row) << endl;
    }

    string footer = document->getFooter();

    if (!footer.empty()) {
      cout << footer << endl;
    }

    cout << "Export Complete.\n\n";
  }
};

/*--------------------------------------------------
 Client
--------------------------------------------------*/
int main() {

  vector<vector<string>> reportData = {{"Name", "Department", "Salary"},
                                       {"Alice", "Engineering", "120000"},
                                       {"Bob", "Marketing", "95000"},
                                       {"Charlie", "Design", "105000"}};

  ReportExporter exporter;

  exporter.exportData(reportData, DocumentType::PDF);
  exporter.exportData(reportData, DocumentType::HTML);
  exporter.exportData(reportData, DocumentType::CSV);

  return 0;
}