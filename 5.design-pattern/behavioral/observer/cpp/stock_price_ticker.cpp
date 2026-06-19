/*
=========================================================================
  Observer Pattern — Practical Example: Stock Price Ticker
=========================================================================

Scenario:
You are building a stock price notification system. A `StockExchange` 
subject continuously publishes price updates for various stocks (AAPL, GOOG).
Multiple independent modules need to react to these updates: a `PriceDisplay` 
shows current prices, an `AlertService` notifies traders when prices cross 
thresholds, and a `TradingBot` executes trades based on price movements.

The Problem:
If the `StockExchange` is hardcoded to call the display, alert, and bot 
modules, it becomes tightly coupled. Adding a new module (like a 
`PortfolioTracker`) would require modifying the core exchange logic, 
violating the Open/Closed Principle.

Solution:
Apply the Observer Pattern. The `StockExchange` acts as the Subject. It 
doesn't know anything about displays or bots; it just maintains a list of 
generic `StockObserver` objects. When a price changes, it iterates through 
the list and calls `onPriceUpdate()`. Now, adding a `PortfolioTracker` 
just means creating a new observer class—no changes to the exchange!

=========================================================================
  Component Mapping
=========================================================================

  Subject         → StockExchangeSubject (Interface for managing observers)
  ConcreteSubject → StockExchange (Maintains stock prices)
  Observer        → StockObserver (Interface for receiving updates)
  ConcreteObserver→ PriceDisplay, AlertService, TradingBot
  Client          → main() (Registers the trading systems to the exchange)

=========================================================================
*/

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class StockExchange; // Forward declaration

/*
=========================================================================
  STEP 1: Observer Interface
=========================================================================
Every module that wants to listen to stock prices must implement this.
*/
class StockObserver {
public:
  virtual ~StockObserver() = default;
  virtual void onPriceUpdate(StockExchange *exchange) = 0;
};

/*
=========================================================================
  STEP 2: Subject Interface
=========================================================================
Defines the standard methods for managing subscriptions.
*/
class StockExchangeSubject {
public:
  virtual ~StockExchangeSubject() = default;
  virtual void registerObserver(StockObserver *observer) = 0;
  virtual void removeObserver(StockObserver *observer) = 0;
  virtual void notifyObservers() = 0;
};

/*
=========================================================================
  STEP 3: Concrete Subject
=========================================================================
Holds the state (prices) and notifies all observers when a price changes.
*/
class StockExchange : public StockExchangeSubject {
private:
  map<string, double> prices;
  vector<StockObserver *> observers;
  string lastUpdatedSymbol;

public:
  void registerObserver(StockObserver *observer) override {
    observers.push_back(observer);
  }

  void removeObserver(StockObserver *observer) override {
    observers.erase(remove(observers.begin(), observers.end(), observer),
                    observers.end());
  }

  void notifyObservers() override {
    // We copy the observers list to avoid modification issues during iteration
    for (StockObserver *observer : vector<StockObserver *>(observers)) {
      observer->onPriceUpdate(this);
    }
  }

  void updatePrice(const string &symbol, double price) {
    prices[symbol] = price;
    lastUpdatedSymbol = symbol;
    cout << "\n[EXCHANGE] " << symbol << " updated to $" << price << endl;
    
    // Notify all listeners automatically
    notifyObservers();
  }

  // Getters for observers to pull state
  double getPrice(const string &symbol) const {
    auto it = prices.find(symbol);
    return it != prices.end() ? it->second : 0.0;
  }

  string getLastUpdatedSymbol() const { 
    return lastUpdatedSymbol; 
  }
};

/*
=========================================================================
  STEP 4: Concrete Observers
=========================================================================
Independent modules that react to the state changes in their own way.
*/

class PriceDisplay : public StockObserver {
public:
  void onPriceUpdate(StockExchange *exchange) override {
    string symbol = exchange->getLastUpdatedSymbol();
    cout << "  Display -> " << symbol << ": $" << exchange->getPrice(symbol)
         << endl;
  }
};

class AlertService : public StockObserver {
private:
  map<string, double> thresholds;

public:
  void setAlert(const string &symbol, double threshold) {
    thresholds[symbol] = threshold;
  }

  void onPriceUpdate(StockExchange *exchange) override {
    string symbol = exchange->getLastUpdatedSymbol();
    auto it = thresholds.find(symbol);
    
    if (it != thresholds.end()) {
      double price = exchange->getPrice(symbol);
      if (price >= it->second) {
        cout << "  ALERT   -> " << symbol << " hit $" << price 
             << " (Target: $" << it->second << ")" << endl;
      }
    }
  }
};

class TradingBot : public StockObserver {
private:
  map<string, double> previousPrices;

public:
  void onPriceUpdate(StockExchange *exchange) override {
    string symbol = exchange->getLastUpdatedSymbol();
    double currentPrice = exchange->getPrice(symbol);
    double previousPrice = previousPrices.count(symbol) ? previousPrices[symbol] : currentPrice;

    if (currentPrice > previousPrice) {
      cout << "  Bot     -> " << symbol << " rising ($" << previousPrice 
           << " -> $" << currentPrice << "). HOLD." << endl;
    } else if (currentPrice < previousPrice) {
      cout << "  Bot     -> " << symbol << " dropping ($" << previousPrice 
           << " -> $" << currentPrice << "). BUY DIPS." << endl;
    }

    previousPrices[symbol] = currentPrice;
  }
};

/*
=========================================================================
  STEP 5: Client Code
=========================================================================
*/
int main() {
  cout << "=== Observer Pattern: Stock Ticker ===" << endl;

  StockExchange exchange;

  // Instantiate the modules (observers)
  PriceDisplay display;
  AlertService alerts;
  TradingBot bot;

  // Set up specific logic for the alert service
  alerts.setAlert("AAPL", 180.0);
  alerts.setAlert("GOOG", 140.0);

  // Wire them up (Dynamic subscription)
  exchange.registerObserver(&display);
  exchange.registerObserver(&alerts);
  exchange.registerObserver(&bot);

  // Simulate market activity
  // The exchange just calls updatePrice; it knows nothing about displays/bots!
  exchange.updatePrice("AAPL", 175.50);
  exchange.updatePrice("GOOG", 138.25);
  
  // These updates should trigger the alerts and bot logic
  exchange.updatePrice("AAPL", 182.00);
  exchange.updatePrice("GOOG", 135.00);

  cout << endl;
  return 0;
}