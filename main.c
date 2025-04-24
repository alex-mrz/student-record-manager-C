#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    char name[50];
    int age;
    float gpa;
    struct Student *next;
};

void addStudent(struct Student **head, char name[], int age, float gpa) {
    struct Student *newStudent = (struct Student*)malloc(sizeof(struct Student));
    if (newStudent == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    strcpy(newStudent->name, name);
    newStudent->age = age;
    newStudent->gpa = gpa;
    newStudent->next = NULL;

    if (*head == NULL) {
        *head = newStudent;  // If list is empty, new student becomes head
    } else {
        struct Student *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newStudent;  // Append at the end
    }
}

void saveToFile(struct Student *head, char mode[], int appendOnly) {
    FILE *file = fopen("students.txt", mode);  
    if (file == NULL) {
        printf("File opening failed!\n");
        return;
    }
    if (appendOnly) {
        struct Student *lastStudent = head;
        while (lastStudent->next != NULL) {
            lastStudent = lastStudent->next;
        }
        fprintf(file, "%s %d %.2f\n", lastStudent->name, lastStudent->age, lastStudent->gpa);
    } else {
        struct Student *temp = head;
        while (temp != NULL) {
            fprintf(file, "%s %d %.2f\n", temp->name, temp->age, temp->gpa);
            temp = temp->next;
        }
    }

    fclose(file);
    printf("Student data saved successfully!\n");
}

void loadFromFile(struct Student **head) {
    FILE *file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("No file found to load students from.\n");
        return;
    }

    char name[50];
    int age;
    float gpa;

    while (fscanf(file, "%49s %d %f", name, &age, &gpa) == 3) {
        addStudent(head, name, age, gpa);
    }

    fclose(file);
    printf("Students loaded successfully from file.\n");
}

int getStudent(struct Student *head, char name[]) {
    struct Student *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("Student found:\n");
            printf("Name: %s\nAge: %d\nGPA: %.2f\n", temp->name, temp->age, temp->gpa);
            return 1;
        }
        temp = temp->next;
    }
    printf("Student with name '%s' not found.\n", name);
    return 0;
}

int modifyStudent(struct Student *head, char name[], float newGPA) {
    struct Student *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("Modifying GPA for %s from %.2f to %.2f\n", temp->name, temp->gpa, newGPA);
            temp->gpa = newGPA;
            return 1;
        }
        temp = temp->next;
    }
    printf("Student with name '%s' not found.\n", name);
    return 0;
}

void rankStudentsByGPA(struct Student *head) {
    if (head == NULL) {
        printf("No students to rank.\n");
        return;
    }

    struct Student *i, *j;
    // Temporary student for swapping data
    struct Student temp;

    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (j->gpa > i->gpa) {
                // Swap all fields EXCEPT the pointer
                strcpy(temp.name, i->name);
                temp.age = i->age;
                temp.gpa = i->gpa;

                strcpy(i->name, j->name);
                i->age = j->age;
                i->gpa = j->gpa;

                strcpy(j->name, temp.name);
                j->age = temp.age;
                j->gpa = temp.gpa;
            }
        }
    }

    printf("\n🏆 Student Ranking by GPA:\n");
    int rank = 1;
    struct Student *tempPtr = head;
    while (tempPtr != NULL) {
        printf("%d. %s — GPA: %.2f\n", rank++, tempPtr->name, tempPtr->gpa);
        tempPtr = tempPtr->next;
    }
}

void freeList(struct Student *head) {
    struct Student *temp;
    while (head != NULL) {
        temp = head;      // Store the current node
        head = head->next; // Move to the next node BEFORE freeing
        free(temp);       // Free the stored node
    }
    printf("Memory freed successfully!\n");
}

int main() {
    int a;
    char overwrite[10];
    
    printf("Do you want to overwrite the student file? (yes/no): ");
    scanf("%s", overwrite);
    
    printf("Enter the number of students: ");
    scanf("%d", &a);

    struct Student *head = NULL;  // Head of the linked list
    char name[50];
    int age;
    float gpa;
    for (int i = 0; i < a; i++) {

        printf("Enter Student Name: ");
        scanf("%49s", name);
        printf("Enter Student Age: ");
        scanf("%d", &age);
        printf("Enter Student GPA: ");
        scanf("%f", &gpa);

        addStudent(&head, name, age, gpa);  // Add student to linked list
    }

    char mode[2];
    if (strcmp(overwrite, "yes") == 0) {
        strcpy(mode, "w");  // Overwrite mode
    } else {
        strcpy(mode, "a");  // Append mode
    }

// After adding students, save them to file
    saveToFile(head, mode, 0);

    // fclose(file);  

    // Ask the user what they want to do next
    char ans[10];
    printf("Do you wish to add more students (add), get student information (get), modify a student's GPA (change), or a ranking (rank)? \n");
    scanf("%s", ans);

    // Adding More Students
    while (strcmp(ans, "add") == 0) {
        printf("Enter Student Name: ");
        scanf("%49s", name);
        printf("Enter Student Age: ");
        scanf("%d", &age);
        printf("Enter Student GPA: ");
        scanf("%f", &gpa);

        addStudent(&head, name, age, gpa);
        saveToFile(head, "a", 1);

        printf("Do you wish to add more students? (yes/no) \n");
        scanf("%s", ans);
        if (strcmp(ans, "yes") == 0) {
            strcpy(ans, "add");
        }
    }

    if (strcmp(ans, "get") == 0) {
        char searchName[50];
        printf("Enter the name of the student to retrieve info: ");
        scanf("%49s", searchName);
        getStudent(head, searchName);
    }

    if (strcmp(ans, "change") == 0) {
        char targetName[50];
        float newGPA;
        printf("Enter the name of the student to modify GPA: ");
        scanf("%49s", targetName);
        printf("Enter new GPA: ");
        scanf("%f", &newGPA);

        int success = modifyStudent(head, targetName, newGPA);
        if (success) {
            saveToFile(head, "w", 0);  // Save only if modification succeeded
        } else {
            printf("No data was saved due to invalid student name.\n");
        }
    }

    if (strcmp(ans, "rank") == 0) {
        loadFromFile(&head);  // Load students from file
        rankStudentsByGPA(head);
    }

    freeList(head); 
    return 0;
}