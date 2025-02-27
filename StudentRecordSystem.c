#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define PASSING_THRESHOLD 40

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int rollNumber;
    float marks;
} Student;

void welcomeMessage()
{
    char userName[MAX_NAME_LENGTH];
    printf("=========================================\n");
    printf("     Welcome to the Student Record System!\n");
    printf("=========================================\n");
    printf("\nPlease enter your name: "); // Added extra space
    fgets(userName, MAX_NAME_LENGTH, stdin);
    userName[strcspn(userName, "\n")] = '\0'; // Remove newline character
    printf("Hello, %s! Let's manage student records.\n", userName);
    printf("=========================================\n\n");
}

void addStudent(Student **students, int *count)
{
    *students = realloc(*students, (*count + 1) * sizeof(Student));
    if (*students == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    printf("\n--- Add New Student ---\n");
    printf("Enter student name: ");
    getchar(); // Clear buffer
    fgets((*students)[*count].name, MAX_NAME_LENGTH, stdin);
    (*students)[*count].name[strcspn((*students)[*count].name, "\n")] = '\0';

    printf("Enter roll number: ");
    if (scanf("%d", &(*students)[*count].rollNumber) != 1)
    {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n')
            ;   // Clear input buffer
        return; // Exit addStudent function due to error
    }

    printf("Enter marks: ");
    if (scanf("%f", &(*students)[*count].marks) != 1)
    {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n')
            ;   // Clear input buffer
        return; // Exit addStudent function due to error
    }

    (*count)++;
    printf("Student added successfully!\n\n"); // Added extra newline for spacing
}

void displayStudents(Student *students, int count)
{
    printf("\n--- Displaying All Students ---\n");
    if (count == 0)
    {
        printf("No student records available.\n\n"); // Added extra newline for spacing
        return;
    }
    printf("=========================================\n");
    printf("Name\t\tRoll No.\tMarks\tStatus\n");
    printf("=========================================\n");
    for (int i = 0; i < count; i++)
    {
        printf("%-15s%-12d%.2f\t%s\n",
               students[i].name, students[i].rollNumber, students[i].marks,
               students[i].marks >= PASSING_THRESHOLD ? "Pass" : "Fail");
    }
    printf("=========================================\n\n"); // Added extra newline for spacing
}

void modifyStudent(Student *students, int count)
{
    int rollNumber;
    printf("\n--- Modify Student Record ---\n");
    printf("Enter roll number of the student to modify: ");
    if (scanf("%d", &rollNumber) != 1)
    {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n')
            ;   // Clear input buffer
        return; // Exit modifyStudent function due to error
    }

    for (int i = 0; i < count; i++)
    {
        if (students[i].rollNumber == rollNumber)
        {
            printf("Enter new name: ");
            getchar(); // Clear buffer
            fgets(students[i].name, MAX_NAME_LENGTH, stdin);
            students[i].name[strcspn(students[i].name, "\n")] = '\0';

            printf("Enter new marks: ");
            if (scanf("%f", &students[i].marks) != 1)
            {
                printf("Invalid input. Please enter a number.\n");
                while (getchar() != '\n')
                    ;   // Clear input buffer
                return; // Exit modifyStudent function due to error
            }
            printf("Student record updated.\n\n"); // Added extra newline for spacing
            return;
        }
    }
    printf("Student with roll number %d not found.\n\n", rollNumber); // Added extra newline for spacing
}

void deleteStudent(Student **students, int *count)
{
    int rollNumber;
    printf("\n--- Delete Student Record ---\n");
    printf("Enter roll number of the student to delete: ");
    if (scanf("%d", &rollNumber) != 1)
    {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n')
            ;   // Clear input buffer
        return; // Exit deleteStudent function due to error
    }

    for (int i = 0; i < *count; i++)
    {
        if ((*students)[i].rollNumber == rollNumber)
        {
            for (int j = i; j < *count - 1; j++)
            {
                (*students)[j] = (*students)[j + 1];
            }
            *students = realloc(*students, (*count - 1) * sizeof(Student));
            (*count)--;
            printf("Student record deleted.\n\n"); // Added extra newline for spacing
            return;
        }
    }
    printf("Student with roll number %d not found.\n\n", rollNumber); // Added extra newline for spacing
}

void searchStudent(Student *students, int count)
{
    int rollNumber;
    printf("\n--- Search Student Record ---\n");
    printf("Enter roll number to search: ");
    if (scanf("%d", &rollNumber) != 1)
    {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n')
            ;   // Clear input buffer
        return; // Exit searchStudent function due to error
    }

    for (int i = 0; i < count; i++)
    {
        if (students[i].rollNumber == rollNumber)
        {
            printf("Student found: Name: %s, Roll Number: %d, Marks: %.2f\n\n",
                   students[i].name, students[i].rollNumber, students[i].marks); // Added extra newline for spacing
            return;
        }
    }
    printf("Student with roll number %d not found.\n\n", rollNumber); // Added extra newline for spacing
}

float calculateAverageMarks(Student *students, int count)
{
    float total = 0;
    for (int i = 0; i < count; i++)
    {
        total += students[i].marks;
    }
    return count > 0 ? total / count : 0; // Avoid division by zero
}

void sortStudentsByMarks(Student *students, int count, int ascending)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if ((ascending && students[j].marks > students[j + 1].marks) ||
                (!ascending && students[j].marks < students[j + 1].marks))
            {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

void saveToFile(Student *students, int count)
{
    FILE *file = fopen("student_records.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s %d %.2f\n", students[i].name, students[i].rollNumber, students[i].marks);
    }
    fclose(file);
    printf("Student records saved to file.\n\n"); // Added extra newline for spacing
}

void loadFromFile(Student **students, int *count)
{
    FILE *file = fopen("student_records.txt", "r");
    if (file == NULL)
    {
        printf("No existing records found.\n\n"); // Added extra newline for spacing
        return;
    }

    *count = 0;
    while (1)
    { // Safer loop condition
        *students = realloc(*students, (*count + 1) * sizeof(Student));
        if (*students == NULL)
        {
            printf("Memory allocation failed!\n");
            exit(1);
        }

        if (fscanf(file, "%s %d %f", (*students)[*count].name, &(*students)[*count].rollNumber, &(*students)[*count].marks) != 3)
        {
            // If fscanf fails to read 3 values, it means end of file or error
            if (feof(file))
                break; // End of file
            else
            {
                printf("Error reading from file. Skipping record.\n");
                // Clear the rest of the line from the file to avoid an infinite loop.
                int c;
                while ((c = fgetc(file)) != '\n' && c != EOF)
                    ;
                continue; // Go to the next iteration of the loop
            }
        }
        (*count)++;
    }
    fclose(file);
    printf("Student records loaded from file.\n\n"); // Added extra newline for spacing
}

int main()
{
    Student *students = NULL;
    int count = 0;
    int choice;

    welcomeMessage();
    loadFromFile(&students, &count);

    while (1)
    {
        printf("\n--- Menu ---\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Modify Student\n");
        printf("4. Delete Student\n");
        printf("5. Search Student\n");
        printf("6. Calculate Average Marks\n");
        printf("7. Sort Students by Marks\n");
        printf("8. Save to File\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");

        // Improved input handling to prevent infinite loop
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ;     // Clear input buffer
            continue; // Go back to the menu
        }

        switch (choice)
        {
        case 1:
            addStudent(&students, &count);
            break;
        case 2:
            displayStudents(students, count);
            break;
        case 3:
            modifyStudent(students, count);
            break;
        case 4:
            deleteStudent(&students, &count);
            break;
        case 5:
            searchStudent(students, count);
            break;
        case 6:
            printf("Average Marks: %.2f\n\n", calculateAverageMarks(students, count)); // Added extra newline for spacing
            break;
        case 7:
            printf("Sort by:\n1. Ascending\n2. Descending\nEnter choice: ");
            int sortChoice;
            if (scanf("%d", &sortChoice) != 1)
            {
                printf("Invalid input. Please enter a number.\n");
                while (getchar() != '\n')
                    ;  // Clear input buffer
                break; // Exit sort menu
            }
            sortStudentsByMarks(students, count, sortChoice == 1);
            printf("Students sorted.\n\n"); // Added extra newline for spacing
            break;
        case 8:
            saveToFile(students, count);
            break;
        case 9:
            free(students);
            printf("Exiting the program.\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n\n"); // Added extra newline for spacing
        }
    }
}
