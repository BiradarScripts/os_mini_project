#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_BOOK_TITLE_LENGTH 50

void book_management(int choice, FILE *members, FILE *books) {
    // Implementation of book management based on the received choice
}

int main() {
    struct sockaddr_in serv, cli;
    int sd;
    char buf[100];
    char choice[10];
    int ret;
    FILE *members = NULL;
    FILE *books = NULL;

    // Create socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address setup
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(5000);

    // Bind socket to address
    if (bind(sd, (struct sockaddr *)&serv, sizeof(serv)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sd, 5) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int nsd;
        int len = sizeof(cli);
        nsd = accept(sd, (struct sockaddr *)&cli, &len);

        if (fork() == 0) { // Child process
            close(sd); // Close the listening socket in the child process

            // Receive user type from client
            ret = read(nsd, buf, sizeof(buf) - 1);
            if (ret == -1) {
                perror("read error");
                exit(EXIT_FAILURE);
            }
            buf[ret] = '\0'; // Null-terminate the received string
            printf("User type received from client: %s\n", buf);
            char received_usertype[20];
            strcpy(received_usertype, buf);

            // Receive username from client
            ret = read(nsd, buf, sizeof(buf) - 1);
            if (ret == -1) {
                perror("read error");
                exit(EXIT_FAILURE);
            }
            buf[ret] = '\0'; // Null-terminate the received string
            char received_username[20];
            strcpy(received_username, buf);

            // Receive password from client
            ret = read(nsd, buf, sizeof(buf) - 1);
            if (ret == -1) {
                perror("read error");
                exit(EXIT_FAILURE);
            }
            buf[ret] = '\0'; // Null-terminate the received string
            char received_password[20];
            strcpy(received_password, buf);

            // Open the appropriate file based on user type
            FILE *file = NULL;
            if (strcmp(received_usertype, "admin") == 0) {
                file = fopen("admins.txt", "r+");
            } else if (strcmp(received_usertype, "member") == 0) {
                file = fopen("members.txt", "r+");
            } else {
                perror("Invalid user type!\n");
                exit(EXIT_FAILURE);
            }

            if (file == NULL) {
                perror("Error opening file!\n");
                exit(EXIT_FAILURE);
            }

            int found = 0;
            char admin_username[20] = "admin";
            char admin_password[20] = "admin123";
            while (fscanf(file, "%s %s", admin_username, admin_password) != EOF) {
                if (strcmp(received_username, admin_username) == 0 && strcmp(received_password, admin_password) == 0) {
                    found = 1;
                    char success_message[100];
                    sprintf(success_message, "%s authentication successfully!\n", received_usertype);
                    write(nsd, success_message, strlen(success_message));
                    break;
                }
            }

            rewind(file);

            if (!found) {
                fprintf(file, "%s %s\n", received_username, received_password);
                char success_message[100];
                sprintf(success_message, "New %s added successfully!\n", received_usertype);
                write(nsd, success_message, strlen(success_message));
            }

            fclose(file);

            if (strcmp(received_usertype, "admin") == 0) {
                printf("%s is managing the system\n", received_username);
                // int choice = 0;
                do {
                    // Receive the choice from the client
                    ret = read(nsd, buf, sizeof(buf) - 1);
                    if (ret == -1) {
                        perror("read error");
                        exit(EXIT_FAILURE);
                    }
                    buf[ret] = '\0';
                    strcpy(choice, buf);

                    if(strcmp(choice,"one")==0){
                        members = fopen("members.txt", "a");
                        if (members == NULL) {
                            printf("Error opening members file!\n");
                            return 1;
                        }

                        //input from client
                        ret = read(nsd, buf, sizeof(buf) - 1);
                        if (ret == -1) {
                            perror("read error");
                            exit(EXIT_FAILURE);
                        }
                        buf[ret] = '\0';
                        printf("Received username to be added: %s\n", buf);

                        
                        fprintf(members, "%s\n", buf);
                        fclose(members);

                        //success message to the client
                        write(nsd,"Member added successfully!\n", strlen("Member added successfully!\n"));
                        
                        //success message in the server
                        printf("Member added successfully!\n");
                    }
                    else if(strcmp(choice,"two")==0){
                        FILE *members = fopen("members.txt", "r");
                        if (members == NULL) {
                            printf("Error opening members file!\n");
                            return 1;
                        }

                        // Input from client
                        ret = read(nsd, buf, sizeof(buf) - 1);
                        if (ret == -1) {
                            perror("read error");
                            exit(EXIT_FAILURE);
                        }
                        buf[ret] = '\0';
                        printf("Received username to be deleted: %s\n", buf);
                        
                        char temp_member[20];
                        char delete_username[20];
                        
                        strcpy(delete_username, buf);

                        FILE *temp = fopen("temp.txt", "w");
                        if (temp == NULL) {
                            printf("Error creating temporary file!\n");
                            fclose(members);
                            return 1;
                        }
                        
                        int member_found = 0;
                        while (fgets(temp_member, sizeof(temp_member), members)) {
                            // Remove newline character from fgets result
                            size_t len = strlen(temp_member);
                            if (temp_member[len - 1] == '\n') {
                                temp_member[len - 1] = '\0';
                            }

                            if (strcmp(temp_member, delete_username) != 0) {
                                fputs(temp_member, temp);
                                fputs("\n", temp); // Add newline after each username
                            } else {
                                member_found = 1;
                            }
                        }
                        
                        fclose(members);
                        fclose(temp);
                        
                        remove("members.txt");
                        rename("temp.txt", "members.txt");
                        
                        if (member_found) {
                            // Success message to the client
                            write(nsd, "Member deleted successfully!\n", strlen("Member deleted successfully!\n"));
                            
                            // Success message in the server
                            printf("Member deleted successfully!\n");
                        } else {
                            printf("Member not found!\n");
                        }
                    }

                    else if(strcmp(choice,"three")==0){
                        books = fopen("books.txt", "a");
                        if (books == NULL) {
                            printf("Error opening books file!\n");
                            return 1;
                        }

                        ret = read(nsd, buf, sizeof(buf) - 1);
                        if (ret == -1) {
                            perror("read error");
                            exit(EXIT_FAILURE);
                        }
                        buf[ret] = '\0';
                        printf("Received book title to add: %s\n", buf);

                    
                        strcpy(buf, buf);

                        fprintf(books, "%s\n", buf);
                        fclose(books);
                        //success message to the client
                        write(nsd,"Book added successfully!\n", strlen("Book added successfully!\n"));
                        
                        //success message in the server
                        printf("Book added successfully!\n");
                    }
                    else if(strcmp(choice,"four")==0){
                        FILE *books = fopen("books.txt", "r");
                        if (books == NULL) {
                            printf("Error opening books file!\n");
                            return 1;
                        }
                        
                        char temp_book[20];
                        char delete_title[20];

                        ret = read(nsd, buf, sizeof(buf) - 1);
                        if (ret == -1) {
                            perror("read error");
                            exit(EXIT_FAILURE);
                        }
                        buf[ret] = '\0';
                        printf("Received book title to delete: %s\n", buf);

                        strcpy(delete_title, buf);
                        
                        FILE *temp = fopen("temp.txt", "w");
                        if (temp == NULL) {
                            printf("Error creating temporary file!\n");
                            fclose(books);
                            return 1;
                        }
                        
                        int book_found = 0;
                        while (fgets(temp_book, sizeof(temp_book), books)) {
                            // Remove newline character from fgets result
                            size_t len = strlen(temp_book);
                            if (temp_book[len - 1] == '\n') {
                                temp_book[len - 1] = '\0';
                            }

                            if (strcmp(temp_book, delete_title) != 0) {
                                fputs(temp_book, temp);
                                fputs("\n", temp); // Add newline after each book title
                            } else {
                                book_found = 1;
                            }
                        }
                        
                        fclose(books);
                        fclose(temp);
                        
                        remove("books.txt");
                        rename("temp.txt", "books.txt");
                        
                        if (book_found) {
                            // Success message to the client
                            write(nsd,"Book deleted successfully!\n", strlen("Book deleted successfully!\n"));
                            
                            // Success message in the server
                            printf("Book deleted successfully!\n");
                        } else {
                            printf("Book not found!\n");
                        }
                    }

                   else if (strcmp(choice, "five") == 0) {


                        char book_title[MAX_BOOK_TITLE_LENGTH];
                        char modified_title[MAX_BOOK_TITLE_LENGTH];

                        // Receive book titles from client
                        ret = read(nsd, book_title, sizeof(book_title) - 1);
                        if (ret == -1) {
                            perror("read error");
                            exit(EXIT_FAILURE);
                        }
                        book_title[ret] = '\0';

                        ret = read(nsd, modified_title, sizeof(modified_title) - 1);
                        if (ret == -1) {
                            perror("read error");
                            exit(EXIT_FAILURE);
                        }
                        modified_title[ret] = '\0';

                        FILE *books = fopen("books.txt", "r+");
                        if (books == NULL) {
                            printf("Error opening books file!\n");
                            return 1;
                        }

                        char temp_book[MAX_BOOK_TITLE_LENGTH];
                        int book_found = 0;

                        FILE *temp = fopen("temp.txt", "w");
                        if (temp == NULL) {
                            printf("Error creating temporary file!\n");
                            fclose(books);
                            return 1;
                        }

                        // Modify book title
                        while (fgets(temp_book, sizeof(temp_book), books)) {
                            temp_book[strcspn(temp_book, "\n")] = '\0'; // Remove newline character
                            if (strcmp(temp_book, book_title) == 0) {
                                fprintf(temp, "%s\n", modified_title); // Write modified title
                                book_found = 1;
                            } else {
                                fprintf(temp, "%s\n", temp_book); // Copy unchanged titles
                            }
                        }

                        fclose(books);
                        fclose(temp);

                        remove("books.txt");
                        rename("temp.txt", "books.txt");

                        if (book_found) {
                            // Success message to the client
                            write(nsd, "Book modified successfully!\n", strlen("Book modified successfully!\n"));

                            // Success message in the server
                            printf("Book modified successfully!\n");
                        } else {
                            printf("Book not found!\n");
                        }
                    }



                   else if(strcmp(choice,"six")==0){
                        FILE *books = fopen("books.txt", "r");
                        if (books == NULL) {
                            printf("Error opening books file!\n");
                            return 1;
                        }
                        
                        char temp_book[20];
                        char search_title[20];
                        
                        ret = read(nsd, buf, sizeof(buf) - 1);
                        if (ret == -1) {
                            perror("read error");
                            exit(EXIT_FAILURE);
                        }
                        buf[ret] = '\0';
                        printf("Received book title to search: %s\n", buf);

                        strcpy(search_title, buf);
                        
                        int book_found = 0;
                        while (fgets(temp_book, sizeof(temp_book), books)) {
                            // Remove the newline character from the line read from the file
                            temp_book[strcspn(temp_book, "\n")] = '\0';
                            if (strcmp(temp_book, search_title) == 0) {
                                // Book found
                                book_found = 1;
                                // Success message to the client
                                write(nsd, temp_book, strlen(temp_book));
                                break;
                            }
                        }
                        
                        fclose(books);

                        if (!book_found) {
                            // Book not found
                            write(nsd,"Book not found!\n", strlen("Book not found!\n"));
                            printf("Book not found!\n");
                        }
                    }

                    else if(strcmp(choice,"seven")==0){
                        close(nsd);
                        exit(0);
                    }
                    else{
                        printf("Invalid choice\n");
                    }
                } while (strcmp(choice,"seven")!=0);
            } 
            else {
                printf("%s is a member\n", received_username);
                // int choice = 0;
                do {
                    books = fopen("books.txt", "r");
                    if (books == NULL) {
                        printf("Error opening books file!\n");
                        return 1;
                    }
                    
                    char temp_book[20];
                    char search_title[20];
                    // Receive the choice from the client
                    ret = read(nsd, buf, sizeof(buf) - 1);
                    if (ret == -1) {
                        perror("read error");
                        exit(EXIT_FAILURE);
                    }
                    buf[ret] = '\0';
                    strcpy(choice, buf);
                    if(strcmp(choice,"one")==0){
                        // Open books file
                        books = fopen("books.txt", "r");
                        if (books == NULL) {
                            printf("Error opening books file!\n");
                            return 1;
                        }
                        // Read book title to search from client
                        ret = read(nsd, buf, sizeof(buf) - 1);
                        if (ret == -1) {
                            perror("read error");
                            exit(EXIT_FAILURE);
                        }
                        buf[ret] = '\0';
                        printf("Reading %s\n", buf);

                        // Remove newline character if present
                        if (buf[ret - 1] == '\n') {
                            buf[ret - 1] = '\0';
                        }

                        // Copy search title
                        strcpy(search_title, buf);

                        // Search for book
                        int book_found = 0;
                        while (fgets(temp_book, sizeof(temp_book), books) != NULL) {
                            // Remove newline character from book title
                            if (temp_book[strlen(temp_book) - 1] == '\n') {
                                temp_book[strlen(temp_book) - 1] = '\0';
                            }
                            // Compare book titles
                            if (strcmp(temp_book, search_title) == 0) {
                                // Book found
                                book_found = 1;
                                // Send book title to client
                                char success_message[100];
                                sprintf(success_message, "enjoy reading %s\n", temp_book);
                                write(nsd, success_message, strlen(success_message));
                                break;
                            }
                        }

                        fclose(books);

                        if (!book_found) {
                            write(nsd, "Book not found!\n", strlen("Book not found!\n"));
                            printf("Book not found!\n");
                        }

                    }
                    else if(strcmp(choice,"two")==0){
                        close(nsd);
                        exit(0);
                    }
                    else{
                        printf("Invalid choice\n");
                    }
                } while (strcmp(choice,"three")!=0);
            }

            close(nsd);
            exit(0);
        }

        close(nsd); // Close the socket in the parent process
    }

    close(sd);

    return 0;
}
