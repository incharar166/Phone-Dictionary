#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 100
#define MAX_CONTACTS 100

// Define a structure for contacts
typedef struct {
    char name[MAX_LENGTH];
    char phone[MAX_LENGTH];
} Contact;

// Function declarations
void signUp();
int login(char *loggedInUsername);
void phoneDictionary(const char *username);
void addNumber(Contact *contacts, int *contactCount);
void deleteNumber(Contact *contacts, int *contactCount);
void editNumber(Contact *contacts, int contactCount);
void displayContacts(Contact *contacts, int contactCount);
void searchContact(Contact *contacts, int contactCount);
void saveContactsToFile(const char *filename, Contact *contacts, int contactCount);
void loadContactsFromFile(const char *filename, Contact *contacts, int *contactCount);
void print_boxed_text(const char *text);
void print_menu_option(const char *text);

int main() {
    int choice;
    char loggedInUsername[MAX_LENGTH] = "";

    while (1) {
        print_boxed_text("--- PHONE DICTIONARY ---");

        print_menu_option("1. Sign Up");
        print_menu_option("2. Login");
        print_menu_option("3. Exit");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                if (login(loggedInUsername)) {
                    phoneDictionary(loggedInUsername);
                }
                break;
            case 3:
                print_boxed_text("Exiting...");
                exit(0);
            default:
                print_boxed_text("Invalid choice. Please try again.");
        }
    }

    return 0;
}

void signUp() {
    char username[MAX_LENGTH], password[MAX_LENGTH];
    FILE *file = fopen("credentials.txt", "a");

    if (file == NULL) {
        print_boxed_text("Error opening credentials file!");
        return;
    }

    printf("Enter a username: ");
    scanf("%s", username);
    printf("Enter a password: ");
    scanf("%s", password);

    fprintf(file, "%s %s\n", username, password);
    fclose(file);

    char filename[MAX_LENGTH];
    sprintf(filename, "%s.txt", username);
    FILE *userFile = fopen(filename, "w");
    if (userFile != NULL) fclose(userFile);

    print_boxed_text("User signed up successfully!");
}

int login(char *loggedInUsername) {
    char username[MAX_LENGTH], password[MAX_LENGTH];
    char fileUsername[MAX_LENGTH], filePassword[MAX_LENGTH];
    FILE *file = fopen("credentials.txt", "r");
    int loginSuccess = 0;

    if (file == NULL) {
        print_boxed_text("No users registered yet. Please sign up first.");
        return 0;
    }

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF) {
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
            loginSuccess = 1;
            strcpy(loggedInUsername, username);
            break;
        }
    }

    fclose(file);

    if (loginSuccess) {
        print_boxed_text("Login successful!");
    } else {
        print_boxed_text("Invalid username or password. Please try again.");
    }

    return loginSuccess;
}

void phoneDictionary(const char *username) {
    int choice;
    char filename[MAX_LENGTH];
    Contact contacts[MAX_CONTACTS];
    int contactCount = 0;

    sprintf(filename, "%s.txt", username);
    loadContactsFromFile(filename, contacts, &contactCount);

    while (1) {
        print_boxed_text("--- Phone Dictionary Menu ---");
        print_menu_option("1. Add Number");
        print_menu_option("2. Delete Number");
        print_menu_option("3. Edit Number");
        print_menu_option("4. Display All Contacts");
        print_menu_option("5. Search Contact");
        print_menu_option("6. Logout");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addNumber(contacts, &contactCount);
                saveContactsToFile(filename, contacts, contactCount);
                break;
            case 2:
                deleteNumber(contacts, &contactCount);
                saveContactsToFile(filename, contacts, contactCount);
                break;
            case 3:
                editNumber(contacts, contactCount);
                saveContactsToFile(filename, contacts, contactCount);
                break;
            case 4:
                displayContacts(contacts, contactCount);
                break;
            case 5:
                searchContact(contacts, contactCount);
                break;
            case 6:
                print_boxed_text("Logging out...\nYou have successfully logged out!!");
                return;
            default:
                print_boxed_text("Invalid choice. Please try again.");
        }
    }
}

void addNumber(Contact *contacts, int *contactCount) {
    if (*contactCount >= MAX_CONTACTS) {
        print_boxed_text("Contact list is full!");
        return;
    }

    char phone[MAX_LENGTH];

    printf("Enter name: ");
    scanf("%99s", contacts[*contactCount].name);

    while (1) {
        printf("Enter phone number: ");
        scanf("%99s", phone);

        // Validate phone number length
        if (strlen(phone) != 10 || strspn(phone, "0123456789") != 10) {
            print_boxed_text("Phone number must be exactly 10 digits long and contain only digits. Try again.");
        } else {
            break;
        }
    }

    strcpy(contacts[*contactCount].phone, phone);
    (*contactCount)++;
    print_boxed_text("Contact added successfully!");
}

void deleteNumber(Contact *contacts, int *contactCount) {
    char name[MAX_LENGTH];
    int found = 0;

    printf("Enter the name of the contact to delete: ");
    scanf("%s", name);

    for (int i = 0; i < *contactCount; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            found = 1;
            for (int j = i; j < *contactCount - 1; j++) {
                contacts[j] = contacts[j + 1];
            }
            (*contactCount)--;  // Decrease the contact count
            break;
        }
    }

    if (found) {
        print_boxed_text("Contact deleted successfully!");
    } else {
        print_boxed_text("Contact not found!");
    }
}

void editNumber(Contact *contacts, int contactCount) {
    char name[MAX_LENGTH], newNumber[MAX_LENGTH];
    int found = 0;

    printf("Enter the name of the contact to edit: ");
    scanf("%s", name);
    printf("Enter the new phone number: ");
    scanf("%s", newNumber);

    // Validate phone number for 10 digits
    if (strlen(newNumber) != 10 || strspn(newNumber, "0123456789") != 10) {
        print_boxed_text("Invalid phone number. Please enter a 10-digit number.");
        return;
    }

    for (int i = 0; i < contactCount; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            strcpy(contacts[i].phone, newNumber);
            found = 1;
            break;
        }
    }

    if (found) {
        print_boxed_text("Contact updated successfully!");
    } else {
        print_boxed_text("Contact not found!");
    }
}

void displayContacts(Contact *contacts, int contactCount) {
    if (contactCount == 0) {
        print_boxed_text("No contacts to display.");
        return;
    }

    print_boxed_text("--- Contacts List ---");
    for (int i = 0; i < contactCount; i++) {
        printf("Name: %s, Phone: %s\n", contacts[i].name, contacts[i].phone);
    }
}

void searchContact(Contact *contacts, int contactCount) {
    char name[MAX_LENGTH];
    int found = 0;

    printf("Enter the name of the contact to search: ");
    scanf("%s", name);

    for (int i = 0; i < contactCount; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            printf("Contact found!\nName: %s, Phone: %s\n", contacts[i].name, contacts[i].phone);
            found = 1;
            break;
        }
    }

    if (!found) {
        print_boxed_text("Contact not found!");
    }
}

void saveContactsToFile(const char *filename, Contact *contacts, int contactCount) {
    FILE *file = fopen(filename, "w+");

    if (file == NULL) {
        print_boxed_text("Error saving contacts to file!");
        return;
    }

    for (int i = 0; i < contactCount; i++) {
        fprintf(file, "%s %s\n", contacts[i].name, contacts[i].phone);
    }

    fclose(file);
    print_boxed_text("Contacts saved successfully to file.");
}

void loadContactsFromFile(const char *filename, Contact *contacts, int *contactCount) {
    FILE *file = fopen(filename, "r+");
    *contactCount = 0;

    if (file == NULL) {
        print_boxed_text("No contact file found for this user. Starting with an empty contact list.");
        return;
    }

    while (fscanf(file, "%s %s", contacts[*contactCount].name, contacts[*contactCount].phone) != EOF) {
        (*contactCount)++;
    }

    fclose(file);
    print_boxed_text("Contacts loaded from file.");
}

void print_boxed_text(const char *text) {
    int len = strlen(text);
    int padding = (30 - len) / 2;

    // Top border
    printf("+------------------------------+\n");

    // Centered text inside the box
    printf("|%*s%-*s%*s|\n", padding, "", 30 - padding - len, text, padding, "");

    // Bottom border
    printf("+------------------------------+\n");
}

void print_menu_option(const char *text) {
    printf("| %-30s |\n", text);
}