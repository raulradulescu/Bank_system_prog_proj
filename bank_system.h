// bank_system.h
#ifndef BANK_SYSTEM_H  // Include guard
#define BANK_SYSTEM_H

// Account structure definition
typedef struct {
    char iban[34];
    char name[100];
    char surname[100];
    char coin[4];  // Assuming currency codes are 3 letters long.
    double amount;
} Account;

// Constants
#define ACCOUNTS_CSV "C:/Users/acer/CLionProjects/programming_proj/accounts_compatible.csv"
#define MAX_ACCOUNTS 1000

// Function prototypes
int loadAccounts(Account **accounts);
void saveAccounts(Account *accounts, int count);
int login(char *name, char *surname, Account **accounts, int count);
void createAccount(Account **accounts, int *count);
void viewAccount(Account *account);
void editAccount(Account *account);
void deleteAccount(Account *accounts, int *count, int index);
void performTransaction(Account *accounts, int count, char *sourceIban, char *destIban, double amount);
void printWelcome();
void printCommands(int isLoggedIn);

#endif // BANK_SYSTEM_H
