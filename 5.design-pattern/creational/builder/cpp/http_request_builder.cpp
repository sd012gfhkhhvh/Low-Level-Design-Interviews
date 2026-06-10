/*
====================================================
The Problem: Building Complex HttpRequest Objects
===================================================
Imagine you're building a system that needs to configure and create HTTP
requests.

Each HttpRequest can contain a mix of required and optional fields:

- URL (required)
- HTTP Method (e.g., GET, POST, PUT, defaults to GET)
- Headers (optional, multiple key-value pairs)
- Query Parameters (optional, multiple key-value pairs)
- Request Body (optional, typically for POST/PUT)
- Timeout (optional, default to 30 seconds)

At first glance, it seems manageable. But as the number of optional fields
increases, so does the complexity of object construction.
*/

#include <iostream>
#include <map>
using namespace std;

/*
==============================================
Before Applying Builder Pattern
The Naive Approach: Telescoping Constructors
==============================================
A common approach is constructor overloading, often called the telescoping
constructor anti-pattern. You define multiple constructors with increasing
numbers of parameters:
*/

class HttpRequestTelescoping {
private:
  string url;
  string method;
  map<string, string> headers;
  map<string, string> queryParams;
  string body;
  int timeout;

public:
  HttpRequestTelescoping(const string &url)
      : HttpRequestTelescoping(url, "GET") {}

  HttpRequestTelescoping(const string &url, const string &method)
      : HttpRequestTelescoping(url, method, {}) {}

  HttpRequestTelescoping(const string &url, const string &method,
                         const map<string, string> &headers)
      : HttpRequestTelescoping(url, method, headers, {}) {}

  HttpRequestTelescoping(const string &url, const string &method,
                         const map<string, string> &headers,
                         const map<string, string> &queryParams)
      : HttpRequestTelescoping(url, method, headers, queryParams, "") {}

  HttpRequestTelescoping(const string &url, const string &method,
                         const map<string, string> &headers,
                         const map<string, string> &queryParams,
                         const string &body)
      : HttpRequestTelescoping(url, method, headers, queryParams, body, 30000) {
  }

  HttpRequestTelescoping(const string &url, const string &method,
                         const map<string, string> &headers,
                         const map<string, string> &queryParams,
                         const string &body, int timeout)
      : url(url), method(method), headers(headers), queryParams(queryParams),
        body(body), timeout(timeout) {
    cout << "HttpRequest Created: URL=" << url << ", Method=" << method
         << ", Headers=" << headers.size() << ", Params=" << queryParams.size()
         << ", Body=" << (!body.empty()) << ", Timeout=" << timeout << endl;
  }

  // Optional: Getters can be added here but no setters are allowed
  // (immutability)

  //   While it works functionally, this design quickly becomes unwieldy and
  //   error-prone as the object becomes more complex.
};

/*
============================================================
After Applying Builder Pattern - The Main Implementation
============================================================
*/

/*
================================================
The Product (HttpRequest) and the Builder class
================================================
*/

class HttpRequest {
private:
  string url;
  string method;
  map<string, string> headers;
  map<string, string> queryParams;
  string body;
  int timeout;

  // Private constructor
  HttpRequest(const string &url, const string &method,
              const map<string, string> &headers,
              const map<string, string> &queryParams, const string &body,
              int timeout)
      : url(url), method(method), headers(headers), queryParams(queryParams),
        body(body), timeout(timeout) {}

public:
  string getUrl() const { return url; }
  string getMethod() const { return method; }

  void print() const {
    cout << "HttpRequest{url='" << url << "', method='" << method
         << "', headers=" << headers.size()
         << ", queryParams=" << queryParams.size() << ", body='" << body
         << "', timeout=" << timeout << "}" << endl;
  }

  // The builder is only relevant for creating HttpRequest, so nesting it
  // inside HttpRequest keeps the API tightly grouped.
  class Builder {
  private:
    string url;
    string method = "GET";
    map<string, string> headers;
    map<string, string> queryParams;
    string body;
    int timeout = 30000;

  public:
    explicit Builder(const string &url) : url(url) {}

    Builder &setMethod(const string &m) {
      method = m;
      return *this;
    }

    Builder &addHeader(const string &key, const string &value) {
      headers[key] = value;
      return *this;
    }

    Builder &addQueryParam(const string &key, const string &value) {
      queryParams[key] = value;
      return *this;
    }

    Builder &setBody(const string &b) {
      body = b;
      return *this;
    }

    Builder &setTimeout(int t) {
      timeout = t;
      return *this;
    }

    HttpRequest build() const {
      return HttpRequest(url, method, headers, queryParams, body, timeout);
    }
  };
};

/*
================================================
The Director Class: Abstracting the Build Process
================================================
So far, the client has been calling builder methods directly. But what
happens when multiple parts of your codebase need to create the same type
of request? You would end up duplicating the same builder calls across
different classes, making your codebase harder to maintain. This is where
the director comes in.

The Director solves this by encapsulating common construction sequences into
named methods. Instead of every client knowing how to configure a builder, the
Director provides pre-built recipes.
*/

class HttpRequestDirector {
public:
  HttpRequest buildSimpleGet(const string &url) {
    return HttpRequest::Builder(url).setMethod("GET").setTimeout(30000).build();
  }

  HttpRequest buildAuthenticatedPost(const string &url, const string &token,
                                     const string &body) {
    return HttpRequest::Builder(url)
        .setMethod("POST")
        .addHeader("Authorization", "Bearer " + token)
        .addHeader("Content-Type", "application/json")
        .setBody(body)
        .setTimeout(10000)
        .build();
  }

  HttpRequest buildInternalServiceCall(const string &url) {
    return HttpRequest::Builder(url)
        .setMethod("GET")
        .addHeader("X-Internal-Service", "true")
        .setTimeout(5000)
        .build();
  }
};

// Client Code
int main() {
  /*
    ==================================
    Before Applying Builder Pattern
    ==================================
  */
  cout << "===============================================" << endl;
  cout << "Before Applying Builder Pattern (Telescoping Constructors)" << endl;
  cout << "===============================================" << endl;

  HttpRequestTelescoping req1("https://api.example.com/data");

  HttpRequestTelescoping req2("https://api.example.com/submit", "POST", {}, {},
                              "{\"key\":\"value\"}");

  HttpRequestTelescoping req3("https://api.example.com/config", "PUT",
                              {{"X-API-Key", "secret"}}, {}, "config_data",
                              5000);

  /*
  ========================
  After Applying Builder
  =======================
  */
  cout << endl << "===============================================" << endl;
  cout << "After Applying Builder Pattern" << endl;
  cout << "===============================================" << endl;

  // Simple GET request
  HttpRequest get =
      HttpRequest::Builder("https://api.example.com/users").build();

  // POST with body and custom timeout
  HttpRequest post =
      HttpRequest::Builder("https://api.example.com/users")
          .setMethod("POST")
          .addHeader("Content-Type", "application/json")
          .setBody("{\"name\":\"Alice\",\"email\":\"alice@example.com\"}")
          .setTimeout(5000)
          .build();

  // Authenticated PUT with query parameters
  HttpRequest put = HttpRequest::Builder("https://api.example.com/config")
                        .setMethod("PUT")
                        .addHeader("Authorization", "Bearer token123")
                        .addHeader("Content-Type", "application/json")
                        .addQueryParam("env", "production")
                        .addQueryParam("version", "2")
                        .setBody("{\"feature_flag\":true}")
                        .setTimeout(10000)
                        .build();

  get.print();
  post.print();
  put.print();

  /*
  ========================
  Applying Director
  =======================
  */
  cout << endl << "===============================================" << endl;
  cout << "After Applying Director Pattern" << endl;
  cout << "===============================================" << endl;

  HttpRequestDirector director;

  // Simple GET request
  HttpRequest getDirector =
      director.buildSimpleGet("https://api.example.com/users");

  // POST with body and custom timeout
  HttpRequest postDirector = director.buildAuthenticatedPost(
      "https://api.example.com/users", "token123",
      "{\"name\":\"Alice\",\"email\":\"[EMAIL_ADDRESS]\"}");

  // Authenticated PUT with query parameters
  HttpRequest internalDirector = director.buildInternalServiceCall(
      "https://internal.api.example.com/validate");

  getDirector.print();
  postDirector.print();
  internalDirector.print();

  return 0;
}