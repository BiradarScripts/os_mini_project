#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/types.h>

#define MAX_USER_TYPE_LENGTH 10
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_MEMBER_USERNAME_LENGTH 20
#define MAX_BOOK_TITLE_LENGTH 50
#define MAX_NUMBER_OF_CHOICES 10

int main() {
    struct sockaddr_in serv;
    int sd;
    char user_type[MAX_USER_TYPE_LENGTH];
    char entered_username[MAX_USERNAME_LENGTH];
    char entered_password[MAX_PASSWORD_LENGTH];
    char member_username[MAX_MEMBER_USERNAME_LENGTH];
    char book_title[MAX_BOOK_TITLE_LENGTH], search_title[MAX_BOOK_TITLE_LENGTH];
    char buf[100];
    int ret;
    char choice[MAX_NUMBER_OF_CHOICES];

    // Create socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Server address setup
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change this to your server IP
    serv.sin_port = htons(5000); // Change this to your server port

    // Connect to the server
    if (connect(sd, (struct sockaddr *)&serv, sizeof(serv)) == -1) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }
    
    // Authentication
    printf("Welcome to the online library\n");

    printf("Are you an admin or a member? (admin/member): ");
    scanf("%9s", user_type);

    write(sd, user_type, strlen(user_type));

    printf("Enter your username: ");
    scanf("%19s", entered_username);
    write(sd, entered_username, strlen(entered_username));
    
    printf("Enter your password: ");
    scanf("%19s", entered_password);
    write(sd, entered_password, strlen(entered_password));

    // Read response from server
    while(getchar() != '\n'); // Clear input buffer
    getchar(); 
    ret = read(sd, buf, sizeof(buf));
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    write(1, buf, ret);

    // Main menu loop
    while(1) {
        if (strcmp(user_type, "admin") == 0) {
            printf("\nAdmin Menu:\n");
            printf("1. Add member\n");
            printf("2. Delete member\n");
            printf("3. Add book\n");
            printf("4. Delete book\n");
            printf("5. Modify book\n");
            printf("6. Search book\n");
            printf("7. Exit\n");
            printf("Enter your choice: ");
            scanf("%9s", choice);

            // Send choice to server
            write(sd, choice, strlen(choice));

            // Handle different choices
            if(strcmp(choice,"one")==0){
                int fd = open("members.txt", O_RDWR);
                printf("Inside Critical Section\n");
                
                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;
                lock.l_pid = getpid();

                // Set write lock
                fcntl(fd, F_SETLKW, &lock);
                
                // Critical section
                printf("Enter member username: ");
                scanf("%s", member_username);
                write(sd, member_username, strlen(member_username));
                while(getchar() != '\n'); // Clear input buffer
                getchar(); 
                ret = read(sd, buf, sizeof(buf));
                if (ret == -1) {
                    perror("read error");
                    exit(EXIT_FAILURE);
                }
                write(1, buf, ret);

                // Unlock
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);  
                printf("Unlock successful\n");

                close(fd);
            }
            else if(strcmp(choice,"two")==0){
                int fd = open("members.txt", O_RDWR);
                printf("Inside Critical Section\n");
                
                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;
                lock.l_pid = getpid();

                // Set write lock
                fcntl(fd, F_SETLKW, &lock);
                
                // Critical section
                printf("Enter member username to delete: ");
                scanf("%s", member_username);
                write(sd, member_username, strlen(member_username));
                while(getchar() != '\n'); // Clear input buffer
                getchar(); 
                ret = read(sd, buf, sizeof(buf));
                if (ret == -1) {
                    perror("read error");
                    exit(EXIT_FAILURE);
                }
                write(1, buf, ret);

                // Unlock
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);  
                printf("Unlock successful\n");

                close(fd);
            }
            else if(strcmp(choice,"three")==0){
                int fd = open("books.txt", O_RDWR);
                printf("Inside Critical Section\n");
                
                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;
                lock.l_pid = getpid();

                // Set write lock
                fcntl(fd, F_SETLKW, &lock);
                
                // Critical section
                printf("Enter book title: ");
                scanf("%s", book_title);
                write(sd, book_title, strlen(book_title));
                while(getchar() != '\n'); // Clear input buffer
                getchar(); 
                ret = read(sd, buf, sizeof(buf));
                if (ret == -1) {
                    perror("read error");
                    exit(EXIT_FAILURE);
                }
                write(1, buf, ret);

                // Unlock
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);  
                printf("Unlock successful\n");

                close(fd);
            }
            else if(strcmp(choice,"four")==0){
                int fd = open("books.txt", O_RDWR);
                printf("Inside Critical Section\n");
                
                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;
                lock.l_pid = getpid();

                // Set write lock
                fcntl(fd, F_SETLKW, &lock);
                
                // Critical section
                printf("Enter book title to delete: ");
                scanf("%s", book_title);
                write(sd, book_title, strlen(book_title));
                while(getchar() != '\n'); // Clear input buffer
                getchar(); 
                ret = read(sd, buf, sizeof(buf));
                if (ret == -1) {
                    perror("read error");
                    exit(EXIT_FAILURE);
                }
                write(1, buf, ret);

                // Unlock
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);  
                printf("Unlock successful\n");

                close(fd);
            }
            else if (strcmp(choice, "five") == 0) {
                FILE *books = fopen("books.txt", "r+");
                if (books == NULL) {
                    printf("Error opening books file!\n");
                    return 1;
                }

                char book_title[MAX_BOOK_TITLE_LENGTH];
                char modified_title[MAX_BOOK_TITLE_LENGTH];

                // Receive input from user
                printf("Enter book title to modify: ");
                scanf("%s", book_title);
                printf("Enter modified title: ");
                scanf("%s", modified_title);

                // Send book titles to server
                write(sd, book_title, strlen(book_title));
                write(sd, modified_title, strlen(modified_title));

                // Read response from server
                ret = read(sd, buf, sizeof(buf));
                if (ret == -1) {
                    perror("read error");
                    exit(EXIT_FAILURE);
                }
                write(1, buf, ret);

                fclose(books);
            }

            else if(strcmp(choice,"six")==0){
                int fd = open("books.txt", O_RDONLY);
                printf("Inside Critical Section\n");
                
                struct flock lock;
                lock.l_type = F_RDLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;
                lock.l_pid = getpid();

                // Set read lock
                fcntl(fd, F_SETLKW, &lock);
                
                // Critical section
                printf("Enter book title to search: ");
                scanf("%s", search_title);
                write(sd, search_title, strlen(search_title));
                while(getchar() != '\n'); // Clear input buffer
                getchar(); 
                ret = read(sd, buf, sizeof(buf));
                if (ret == -1) {
                    perror("read error");
                    exit(EXIT_FAILURE);
                }
                write(1, buf, ret);
                printf("\n");
                // Unlock
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);  
                printf("Unlock successful\n");

                close(fd);
            }
            else if(strcmp(choice,"seven")==0){
                close(sd);
                exit(0);
            }
            else{
                printf("Invalid choice\n");
            }
        } else if (strcmp(user_type, "member") == 0) {
            printf("\nMember Menu:\n");
            printf("1. Search book\n");
            printf("2. Exit\n");
            printf("Enter your choice: ");
            scanf("%9s", choice);

            // Send choice to server
            write(sd, choice, strlen(choice));

            // Handle different choices
            if(strcmp(choice,"one")==0){

                int fd = open("books.txt", O_RDONLY);

                printf("Inside Critical Section\n");

                struct flock lock;
                lock.l_start = 0;
                lock.l_len = 0;
                lock.l_whence = SEEK_SET;
                lock.l_type = F_RDLCK;

                int res = fcntl(fd, F_SETLKW, &lock);
                if (res < 0){
                    printf("Lock unsuccessful\n");
                    return 0;
                }

                printf("Locked\n");


                printf("Enter book title to search: ");
                scanf("%s", search_title);
                write(sd, search_title, strlen(search_title));
                while(getchar() != '\n'); // Clear input buffer
                // getchar(); 
                ret = read(sd, buf, sizeof(buf));
                if (ret == -1) {
                    perror("read error");
                    exit(EXIT_FAILURE);
                }
                write(1, buf, ret);

                getchar();
                printf("Unlocking\n");

                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLKW, &lock);  
                printf("Unlock successful");

                close(fd);
            }
            
            else if(strcmp(choice,"two")==0){
                close(sd);
                exit(0);
            }
            else{
                printf("Invalid choice\n");
            }
        } else {
            perror("Invalid user type!\n");
            exit(EXIT_FAILURE);
        }
    }
    
    close(sd);

    return 0;}

