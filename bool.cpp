bool processOrder(Order& new_order) {
    if (new_order.type == OrderType::BUY) {
        // Logic for processing a BUY order
        while (!sell_orders.empty() && sell_orders.top().price <= new_order.price) {
            // Loop through sell orders while there are matching orders and prices are compatible
            Order sell_order = sell_orders.top(); // Get the top (lowest price) sell order

            if (sell_order.quantity >= new_order.quantity) {
                // If the sell order has enough quantity to fulfill the buy order
                sell_order.quantity -= new_order.quantity; // Reduce the sell order's quantity
                new_order.status = "filled"; // Set the buy order status to "filled"
                sell_orders.pop(); // Remove the sell order from the queue

                if (sell_order.quantity > 0) {
                    sell_orders.push(sell_order); // If there's remaining quantity, add the modified sell order back
                }
                return true; // Return true to indicate that the order has been fully or partially filled
            } else {
                // If the sell order has less quantity than the buy order
                new_order.status = "partially filled (" + to_string(sell_order.quantity) + " out of " + to_string(new_order.quantity) + ")";
                sell_orders.pop(); // Remove the sell order from the queue
                return true; // Return true to indicate that the order has been fully or partially filled
            }
        }
    } else {
        // Logic for processing a SELL order
        while (!buy_orders.empty() && buy_orders.top().price >= new_order.price) {
            // Loop through buy orders while there are matching orders and prices are compatible
            Order buy_order = buy_orders.top(); // Get the top (highest price) buy order

            if (buy_order.quantity >= new_order.quantity) {
                // If the buy order has enough quantity to fulfill the sell order
                buy_order.quantity -= new_order.quantity; // Reduce the buy order's quantity
                new_order.status = "filled"; // Set the sell order status to "filled"
                buy_orders.pop(); // Remove the buy order from the queue

                if (buy_order.quantity > 0) {
                    buy_orders.push(buy_order); // If there's remaining quantity, add the modified buy order back
                }
                return true; // Return true to indicate that the order has been fully or partially filled
            } else {
                // If the buy order has less quantity than the sell order
                new_order.status = "partially filled (" + to_string(buy_order.quantity) + " out of " + to_string(new_order.quantity) + ")";
                buy_orders.pop(); // Remove the buy order from the queue
                return true; // Return true to indicate that the order has been fully or partially filled
            }
        }
    }

    // If no matching orders are found, mark the order as rejected
    new_order.status = "rejected";
    new_order.reason = "No matching orders";
    return false; // Return false to indicate that the order has been rejected
}