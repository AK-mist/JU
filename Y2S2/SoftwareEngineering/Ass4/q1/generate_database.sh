#!/bin/bash

if [ ! -d ./database ]; then
    mkdir ./database
    echo "Directory generated: database"
fi

generate_inventory() {
    echo "product_id, product_name, cost_per_item, quantity"
    for((i=1; i<=10; i++)); do
        product_id="$i"
        product_name="P$i"
        cost_per_item=$((RANDOM % 100 + 30))
        quantity=$((RANDOM % 100))
        echo "$product_id, $product_name, $cost_per_item, $quantity"
    done
}

generate_customer() {
    echo "customer_id, number_of_successful_purchases, number_of_failed_purchases, total_spending"
}

generate_purchase_history() {
    echo "customer_id, product_id, purchase_status, purchase_quantity, purchase_cost"
}

if [ ! -f ./database/inventory.csv ]; then
    generate_inventory > ./database/inventory.csv
    echo "File generated: inventory.csv"
fi

if [ ! -f ./database/customers.csv ]; then
    generate_customer > ./database/customers.csv
    echo "File generated: customers.csv"
fi

if [ ! -f ./database/purchase_history.csv ]; then
    generate_purchase_history > ./database/purchase_history.csv
    echo "File generated: purchase_history.csv"
fi