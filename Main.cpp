#include <iostream>
#include <queue>
#include <string>

using namespace std;

enum class OrderType : int { BUY = 1, SELL = 2 };

class Order {
public:
  int client_id;
  string instrument;
  OrderType type;
  int quantity;
  double price;
  string status = "unfilled";
  string reason;

  Order(int client_id, string instrument, OrderType type, int quantity, double price)
      : client_id(client_id), instrument(instrument), type(type), quantity(quantity), price(price) {}
};

class OrderBook {
private:
  string instrument;
  priority_queue<Order, vector<Order>> buy_orders;
  priority_queue<Order, vector<Order>, greater<Order>> sell_orders;

public:
  OrderBook(string instrument) : instrument(instrument) {}

  void addOrder(Order order) {
    if (order.type == OrderType::BUY) {
      buy_orders.push(order);
    } else {
      sell_orders.push(order);
    }
  }

  bool processOrder(Order& new_order) {
    if (new_order.type == OrderType::BUY) {
      while (!sell_orders.empty() && sell_orders.top().price <= new_order.price) {
        Order sell_order = sell_orders.top();

        if (sell_order.quantity >= new_order.quantity) {
          sell_order.quantity -= new_order.quantity;
          new_order.status = "filled";
          sell_orders.pop();

          if (sell_order.quantity > 0) {
            sell_orders.push(sell_order);
          }
          return true;
        } else {
          new_order.status = "partially filled (" + to_string(sell_order.quantity) + " out of " + to_string(new_order.quantity) + ")";
          sell_orders.pop(); 
          return true;
        }
      }
    } else {
      while (!buy_orders.empty() && buy_orders.top().price >= new_order.price) {
        Order buy_order = buy_orders.top();

        if (buy_order.quantity >= new_order.quantity) {
          buy_order.quantity -= new_order.quantity;
          new_order.status = "filled";
          buy_orders.pop();

          if (buy_order.quantity > 0) {
            buy_orders.push(buy_order);
          }
          return true;
        } else {
          new_order.status = "partially filled (" + to_string(buy_order.quantity) + " out of " + to_string(new_order.quantity) + ")";
          buy_orders.pop(); 
          return true;
        }
      }
    }

    new_order.status = "rejected";
    new_order.reason = "No matching orders";
    return false;
  }
};