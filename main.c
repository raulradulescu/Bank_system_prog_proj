#include "bank_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Account *accounts = NULL;
    int accountCount = loadAccounts(&accounts);
    int loggedInAccountIndex = -1; // -1 indicates that no user is logged in

    printWelcome();

    // Main interaction loop
    char command[16];
    while (1) {

        printCommands(loggedInAccountIndex != -1);
        scanf("%15s", command);

        if (strcmp(command, "login") == 0 && loggedInAccountIndex == -1) {
            char name[100], surname[100];
            printf("Enter name: ");
            scanf("%99s", name);
            printf("Enter surname: ");
            scanf("%99s", surname);
            loggedInAccountIndex = login(name, surname, &accounts, accountCount);
            if (loggedInAccountIndex != -1) {
                printf("Login successful!\n");
            } else {
                printf("Login failed. Please try again.\n");
            }
        } else if (strcmp(command, "logout") == 0 && loggedInAccountIndex != -1) {
            loggedInAccountIndex = -1; // Reset login to log out
            printf("Logout successful.\n");
        } else if (strcmp(command, "view") == 0 && loggedInAccountIndex != -1) {
            viewAccount(&accounts[loggedInAccountIndex]);
        } else if (strcmp(command, "edit") == 0 && loggedInAccountIndex != -1) {
            editAccount(&accounts[loggedInAccountIndex]);
            saveAccounts(accounts, accountCount); // Save changes
        } else if (strcmp(command, "delete") == 0 && loggedInAccountIndex != -1) {
            deleteAccount(accounts, &accountCount, loggedInAccountIndex);
            loggedInAccountIndex = -1; // Reset login after account deletion
            saveAccounts(accounts, accountCount); // Save changes
        } else if (strcmp(command, "transaction") == 0) {
            if (loggedInAccountIndex == -1) {
                printf("Please login to perform a transaction.\n");
            } else {
                char destIban[34];
                double amount;
                printf("Enter destination IBAN: ");
                scanf("%33s", destIban);
                printf("Enter amount to transfer: ");
                scanf("%lf", &amount);
                performTransaction(accounts, accountCount, accounts[loggedInAccountIndex].iban, destIban, amount);
                saveAccounts(accounts, accountCount); // Save changes
            }
        } else if (strcmp(command, "signup") == 0){
            createAccount(&accounts, &accountCount);
            saveAccounts(accounts, accountCount); // Save new account to CSV
        }else if (strcmp(command, "exit") == 0) {
            break; // Exit the program
        } else {
            printf("Invalid command or you need to login first.\n");
        }
    }

    // Cleanup and exit
    free(accounts);
    return 0;
}