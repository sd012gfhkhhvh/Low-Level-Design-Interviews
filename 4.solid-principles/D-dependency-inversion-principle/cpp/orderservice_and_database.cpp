/*
=============================================
          OrderService and Database
=============================================
Refactor OrderService

Problem: An OrderService directly creates and uses a MySQLDatabase object to
save orders. If you want to switch to PostgreSQL or MongoDB, you would have to
rewrite OrderService. Refactor by introducing a Database interface so
OrderService depends on the abstraction, not the concrete database. Then add a
PostgresDatabase implementation and show both databases working with the same
OrderService.

Requirements:

- Create a Database interface with insert(table, data) and query(table, id)
methods
- Make MySQLDatabase implement the Database interface
- Create a PostgresDatabase implementation that prints "PostgreSQL: ..." instead
of "MySQL:
..."
- Refactor OrderService to accept a Database through its constructor instead
of creating one internally
- Show OrderService working with both databases without
changing OrderService

*/

#include <iostream>
#include <memory>
#include <string>

using namespace std;

/*
==================
Before DIP
==================
*/

// // Before: OrderService is tightly coupled to MySQLDatabase
// class MySQLDatabase {
// public:
//   void insert(const string &table, const string &data) {
//     cout << "MySQL: Inserting into " << table << " -> " << data << endl;
//   }

//   string query(const string &table, const string &id) {
//     cout << "MySQL: Querying " << table << " for id " << id << endl;
//     return "{ id: " + id + ", item: 'Widget' }";
//   }
// };

// class OrderService {
// private:
//   MySQLDatabase database; // Direct dependency!

// public:
//   void placeOrder(const string &orderId, const string &orderData) {
//     cout << "Placing order: " << orderId << endl;
//     database.insert("orders", orderData);
//     cout << "Order placed successfully." << endl;
//   }

//   string getOrder(const string &orderId) {
//     return database.query("orders", orderId);
//   }
// };

/*
================
After DIP
================
*/

class Database {
public:
  virtual ~Database() = default;
  virtual void insert(const string &table, const string &data) = 0;
  virtual string query(const string &table, const string &id) = 0;
};

/*
=========================================================
                CONCRETE Inplementations
=========================================================
*/

class MySQLDatabase : public Database {
public:
  void insert(const string &table, const string &data) override {
    cout << "MySQL: Inserting into " << table << " -> " << data << endl;
  }

  string query(const string &table, const string &id) override {
    cout << "MySQL: Querying " << table << " for id " << id << endl;
    return "{ id: " + id + ", item: 'Widget' }";
  }
};

class PostgresDatabase : public Database {
public:
  void insert(const string &table, const string &data) override {
    cout << "PostgreSQL: Inserting into " << table << " -> " << data << endl;
  }

  string query(const string &table, const string &id) override {
    cout << "PostgreSQL: Querying " << table << " for id " << id << endl;
    return "{ id: " + id + ", item: 'Widget' }";
  }
};

class OrderService {
private:
  unique_ptr<Database> database;

public:
  OrderService(unique_ptr<Database> db) : database(move(db)) {}

  void placeOrder(const string &orderId, const string &orderData) {
    cout << "Placing order: " << orderId << endl;
    database->insert("orders", orderData);
    cout << "Order placed successfully." << endl;
  }

  string getOrder(const string &orderId) {
    return database->query("orders", orderId);
  }
};

/*
=========================================================
                Driver Code
=========================================================
*/

int main() {
  // Before DIP
  //   OrderService service;
  //   service.placeOrder("ORD-001", "{ item: 'Widget', qty: 3 }");
  //   string order = service.getOrder("ORD-001");
  //   cout << "Order: " << order << endl;

  // After DIP
  cout << "--- MySQL ---" << endl;
  OrderService mysqlService(make_unique<MySQLDatabase>());
  mysqlService.placeOrder("ORD-001", "{ item: 'Widget', qty: 3 }");
  string order1 = mysqlService.getOrder("ORD-001");
  cout << "Order: " << order1 << endl;

  cout << endl;
  cout << "--- PostgreSQL ---" << endl;
  OrderService pgService(make_unique<PostgresDatabase>());
  pgService.placeOrder("ORD-001", "{ item: 'Widget', qty: 3 }");
  string order2 = pgService.getOrder("ORD-001");
  cout << "Order: " << order2 << endl;

  return 0;
}