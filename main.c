#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>

#define MAX_ENTRIES 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_WEBSITE_LENGTH 100
#define MAX_USERNAME_LENGTH 50

struct PasswordEntry {
    char website[MAX_WEBSITE_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct PasswordEntry passwordDatabase[MAX_ENTRIES];
int numEntries = 0;

const char* FILENAME = "passwords.dat";
const char* SECRET_KEY = "YourSecretKey"; // Replace with a strong secret key

void addPassword() {
    if (numEntries < MAX_ENTRIES) {
        printf("Enter website: ");
        scanf("%s", passwordDatabase[numEntries].website);
        printf("Enter username: ");
        scanf("%s", passwordDatabase[numEntries].username);
        printf("Enter password: ");
        scanf("%s", passwordDatabase[numEntries].password);
        numEntries++;
        printf("Password added successfully.\n");
    } else {
        printf("Password database is full.\n");
    }
}

void viewPasswords() {
    printf("Stored Passwords:\n");
    for (int i = 0; i < numEntries; i++) {
        printf("Website: %s\n", passwordDatabase[i].website);
        printf("Username: %s\n", passwordDatabase[i].username);
        printf("Password: %s\n", passwordDatabase[i].password);
        printf("----------------------------\n");
    }
}

void savePasswords() {
    FILE* file = fopen(FILENAME, "wb");
    if (file) {
        AES_KEY aesKey;
        AES_set_encrypt_key((const unsigned char*)SECRET_KEY, 128, &aesKey);
        
        for (int i = 0; i < numEntries; i++) {
            struct PasswordEntry entry = passwordDatabase[i];
            AES_encrypt((const unsigned char*)entry.website, (unsigned char*)entry.website, &aesKey);
            AES_encrypt((const unsigned char*)entry.username, (unsigned char*)entry.username, &aesKey);
            AES_encrypt((const unsigned char*)entry.password, (unsigned char*)entry.password, &aesKey);
            
            fwrite(&entry, sizeof(struct PasswordEntry), 1, file);
        }
        
        fclose(file);
        printf("Passwords saved successfully.\n");
    } else {
        printf("Error saving passwords to file.\n");
    }
}

void loadPasswords() {
    FILE* file = fopen(FILENAME, "rb");
    if (file) {
        AES_KEY aesKey;
        AES_set_decrypt_key((const unsigned char*)SECRET_KEY, 128, &aesKey);
        
        struct PasswordEntry entry;
        while (fread(&entry, sizeof(struct PasswordEntry), 1, file)) {
            AES_decrypt((const unsigned char*)entry.website, (unsigned char*)entry.website, &aesKey);
            AES_decrypt((const unsigned char*)entry.username, (unsigned char*)entry.username, &aesKey);
            AES_decrypt((const unsigned char*)entry.password, (unsigned char*)entry.password, &aesKey);
            
            passwordDatabase[numEntries++] = entry;
        }
        
        fclose(file);
        printf("Passwords loaded successfully.\n");
    } else {
        printf("Error loading passwords from file.\n");
    }
}

int main() {
    int choice;
    
    loadPasswords(); // Load passwords from file when starting the program
    
    while (1) {
        printf("\nPassword Manager Menu:\n");
        printf("1. Add Password\n");
        printf("2. View Passwords\n");
        printf("3. Save Passwords\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addPassword();
                break;
            case 2:
                viewPasswords();
                break;
            case 3:
                savePasswords();
                break;
            case 4:
                printf("Exiting Password Manager.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    
    return 0;
}
