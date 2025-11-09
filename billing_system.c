#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for each item
struct Item {
    char name[50];
    int quantity;
    float price;
};

// Structure for customer bill
struct Bill {
    char customerName[50];
    char date[20];
    int numItems;
    struct Item items[50];
    float total;
};

// Function declarations
void generateBill(struct Bill bill);
void saveBill(struct Bill bill);
void showAllBills();
void searchBill();

int main() {
    struct Bill bill;
    int choice;
    FILE *fp;

    do {
        printf("\n====== Supermarket Billing System ======\n");
        printf("1. Generate New Bill\n");
        printf("2. Show All Bills\n");
        printf("3. Search Bill by Customer Name\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                // Input customer details
                printf("\nEnter Customer Name: ");
                fgets(bill.customerName, sizeof(bill.customerName), stdin);
                bill.customerName[strcspn(bill.customerName, "\n")] = 0;

                printf("Enter Date (DD/MM/YYYY): ");
                fgets(bill.date, sizeof(bill.date), stdin);
                bill.date[strcspn(bill.date, "\n")] = 0;

                printf("Enter Number of Items: ");
                scanf("%d", &bill.numItems);
                getchar();

                bill.total = 0;
                for (int i = 0; i < bill.numItems; i++) {
                    printf("\nItem %d name: ", i + 1);
                    fgets(bill.items[i].name, sizeof(bill.items[i].name), stdin);
                    bill.items[i].name[strcspn(bill.items[i].name, "\n")] = 0;

                    printf("Quantity: ");
                    scanf("%d", &bill.items[i].quantity);
                    printf("Price per unit: ");
                    scanf("%f", &bill.items[i].price);
                    getchar();

                    bill.total += bill.items[i].quantity * bill.items[i].price;
                }

                // Generate and display the bill
                generateBill(bill);
                saveBill(bill);
                break;

            case 2:
                showAllBills();
                break;

            case 3:
                searchBill();
                break;

            case 4:
                printf("\nExiting program. Thank you!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}

// Function to generate and print the bill
void generateBill(struct Bill bill) {
    float discount = 0, tax, netTotal;

    if (bill.total >= 5000)
        discount = 0.1 * bill.total;
    else if (bill.total >= 2000)
        discount = 0.05 * bill.total;

    tax = 0.18 * (bill.total - discount);
    netTotal = bill.total - discount + tax;

    printf("\n=========== Supermarket Bill ===========\n");
    printf("Customer: %s\nDate: %s\n", bill.customerName, bill.date);
    printf("----------------------------------------\n");
    printf("%-20s %-10s %-10s\n", "Item", "Qty", "Price");
    printf("----------------------------------------\n");

    for (int i = 0; i < bill.numItems; i++) {
        printf("%-20s %-10d %-10.2f\n",
               bill.items[i].name,
               bill.items[i].quantity,
               bill.items[i].price * bill.items[i].quantity);
    }

    printf("----------------------------------------\n");
    printf("Sub Total: %.2f\n", bill.total);
    printf("Discount : %.2f\n", discount);
    printf("Tax (18%%): %.2f\n", tax);
    printf("Grand Total: %.2f\n", netTotal);
    printf("========================================\n");
}

// Function to save the bill to a file
void saveBill(struct Bill bill) {
    FILE *fp = fopen("bills.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "%s,%s,%d,%.2f\n", bill.customerName, bill.date, bill.numItems, bill.total);
    for (int i = 0; i < bill.numItems; i++) {
        fprintf(fp, "%s,%d,%.2f\n", bill.items[i].name, bill.items[i].quantity, bill.items[i].price);
    }
    fprintf(fp, "----------------------------\n");

    fclose(fp);
    printf("\nBill saved successfully!\n");
}

// Function to display all bills from file
void showAllBills() {
    FILE *fp = fopen("bills.txt", "r");
    if (fp == NULL) {
        printf("No bill records found!\n");
        return;
    }

    char line[200];
    printf("\n====== All Saved Bills ======\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

// Function to search for a bill by customer name
void searchBill() {
    FILE *fp = fopen("bills.txt", "r");
    if (fp == NULL) {
        printf("No bills found!\n");
        return;
    }

    char name[50], line[200];
    int found = 0;
    printf("Enter Customer Name to search: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("\n====== Search Results ======\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, name)) {
            printf("%s", line);
            found = 1;
        }
    }
    if (!found)
        printf("No bill found for %s.\n", name);

    fclose(fp);
}