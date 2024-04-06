#include "bank_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Load accounts from CSV file
int loadAccounts(Account **accounts) {
    FILE *file = fopen(ACCOUNTS_CSV, "r");
    if (!file) {
        perror("Error opening accounts file");
        return 0;
    }

    char line[256];
    int count = 0;
    *accounts = malloc(MAX_ACCOUNTS * sizeof(Account));

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^;];%[^;];%[^;];%[^;];%lf\n",
               (*accounts)[count].iban,
               (*accounts)[count].name,
               (*accounts)[count].surname,
               (*accounts)[count].coin,
               &(*accounts)[count].amount);
        count++;
    }

    fclose(file);
    return count;
}

// Save accounts to CSV file
void saveAccounts(Account *accounts, int count) {
    FILE *file = fopen(ACCOUNTS_CSV, "w");
    if (!file) {
        perror("Error opening accounts file for writing");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s;%s;%s;%s;%lf\n",
                accounts[i].iban,
                accounts[i].name,
                accounts[i].surname,
                accounts[i].coin,
                accounts[i].amount);
    }

    fclose(file);
}
// User login function
int login(char *name, char *surname, Account **accounts, int count) {
    for (int i = 0; i < count; ++i) {
        if (strcmp((*accounts)[i].name, name) == 0 && strcmp((*accounts)[i].surname, surname) == 0) {
            //printf("%d",i);
            return i;  // Return account index if found

        }
    }
    return -1; // Return -1 if account not found
}
void createAccount(Account **accounts, int *count) {
    if (*count >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached.\n");
        return;
    }

    printf("Creating a new account:\n");
    printf("Enter IBAN: ");
    scanf("%33s", (*accounts)[*count].iban);
    printf("Enter name: ");
    scanf("%99s", (*accounts)[*count].name);
    printf("Enter surname: ");
    scanf("%99s", (*accounts)[*count].surname);
    printf("Enter currency code (3 letters): ");
    scanf("%3s", (*accounts)[*count].coin);
    printf("Enter initial amount: ");
    scanf("%lf", &(*accounts)[*count].amount);

    (*count)++;
    printf("Account created successfully.\n");
}

// View account details function
void viewAccount(Account *account) {
    printf("Account IBAN: %s\n", account->iban);
    printf("Account Owner: %s %s\n", account->name, account->surname);
    printf("Currency: %s\n", account->coin);
    printf("Balance: %.2f\n", account->amount);
}

// Edit account details function
void editAccount(Account *account) {
    printf("Editing Account of %s %s\n", account->name, account->surname);
    printf("Enter new IBAN (current: %s): ", account->iban);
    scanf("%99s", account->iban);
    printf("Enter new currency (current: %s): ", account->coin);
    scanf("%3s", account->coin);
    printf("Enter new amount (current: %.2f): ", account->amount);
    scanf("%lf", &account->amount);
}

// Delete an account function
void deleteAccount(Account *accounts, int *count, int index) {
    for (int i = index; i < *count - 1; ++i) {
        accounts[i] = accounts[i + 1];
    }
    *count = *count - 1;
}

// Perform a transaction between two accounts
void performTransaction(Account *accounts, int count, char *sourceIban, char *destIban, double amount) {
    if (amount <= 0) {
        printf("The amount must be greater than 0.\n");
        return;
    }
    int sourceIndex = -1, destIndex = -1;

    // Find source and destination accounts by IBAN
    for (int i = 0; i < count; ++i) {
        if (strcmp(accounts[i].iban, sourceIban) == 0) {
            sourceIndex = i;
        } else if (strcmp(accounts[i].iban, destIban) == 0) {
            destIndex = i;
        }
    }

    // Check if both accounts were found
    if (sourceIndex == -1) {
        printf("Transaction failed: Source account not found.\n");
        return;
    }
    if (destIndex == -1) {
        printf("Transaction failed: Destination account not found.\n");
        return;
    }

    // Check if source and destination accounts have the same currency
    if (strcmp(accounts[sourceIndex].coin, accounts[destIndex].coin) != 0) {
        printf("Transaction failed: Currency mismatch between accounts.\n");
        return;
    }

    // Check if source account has enough balance
    if (accounts[sourceIndex].amount < amount) {
        printf("Transaction failed: Insufficient funds in source account.\n");
        return;
    }

    // Perform the transaction
    accounts[sourceIndex].amount -= amount;
    accounts[destIndex].amount += amount;
    printf("Transaction successful: Amount %.2f transferred from %s to %s.\n", amount, sourceIban, destIban);
}
void printWelcome() {
    printf("/====================================================\\\n");
    printf("|         Welcome to the Bank Management System      |\n");
    printf("\\====================================================/\n");
}
void printCommands(int isLoggedIn) {

    if (isLoggedIn) {
        printf("/==============================================================\\\n");
        printf("| Commands: view | edit | delete | transaction | logout | exit |\n");
        printf("\\==============================================================/\n");
    } else {
        printf("/==================================\\\n");
        printf("| Commands: login | sign up | exit |\n");
        printf("\\==================================/\n");
    }

    printf("Enter command: ");
}