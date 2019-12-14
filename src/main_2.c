#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

// Sturct Declaration
struct Course {
    char id[10];
    char name[50];
    char grade[3];
    int creditHours;
    float gradePoints;
    struct Course *next;
};

struct Semester {
    int semester;
    float gpa;
    struct Course *recentCourse;
    struct Semester *next;
};

struct Student {
    char name[50];
    float cgpa;
    struct Semester *recentSemester;
    struct Student *next;
};

struct Student *recentStudent = NULL, *activeStudent = NULL;

// Function Declartion
void calculateCgpa();
void calculateGpa(int);
void changeStudent(char[50]);
float convertGrade(char[3]);

void displayMenu();

struct Course *getCourseMemory(char[10]);
struct Semester *getSemesterMemory(struct Semester*, int);
struct Student *getStudentMemory(struct Student*, char[50]);
int getStudentMemoryOrder(struct Student*);

bool isCourseExist(char[10]);
bool isSemesterExist(struct Semester*, int);
bool isStudentExist(struct Student*, char[50]);

int mainMenu();
void modifyMenu();

void popCourse(char[10]);
void popSemester(int);
void popStudent(struct Student*);
void pushSemester(int);
void pushStudent(char[50]);

char *myStrlwr(char[50]);
void submitMenu();

int main() {
    bool isContinue = true;
    while(isContinue) isContinue = mainMenu();
    return 0;
}

void calculateCgpa() {
    struct Semester *currSemester = activeStudent->recentSemester;
    float cgpa = 0;
    int totalSemester = 0;
    
    while(currSemester != NULL) {
        totalSemester++;
        cgpa += currSemester->gpa;

        currSemester = currSemester->next;
    }

    activeStudent->cgpa = cgpa / (float)totalSemester;
}

void calculateGpa(int semester) {
    if(isSemesterExist(activeStudent->recentSemester, semester)) return;
    struct Course *currCourse = getSemesterMemory(activeStudent->recentSemester, semester)->recentCourse;
    float totalOfGradePoints = 0, gpa = 0;
    int totalOfCreditHours = 0;
    while(currCourse != NULL) {
        totalOfCreditHours += currCourse->creditHours;
        totalOfGradePoints += ((float)currCourse->creditHours * currCourse->gradePoints);
        currCourse = currCourse->next;
    }
    gpa = totalOfGradePoints / (float)totalOfCreditHours;
    getSemesterMemory(activeStudent->recentSemester, semester)->gpa = gpa;
}



void changeStudent(char name[50]) {
    if(!isStudentExist(recentStudent, name)) {
        printf("Student not found\n");
        return;
    } else activeStudent = getStudentMemory(recentStudent, name);
}

float convertGrade(char grade[3]) {
    float gradePoints = 0;
    if(strcmp(grade, "A-") == 0) {
        gradePoints = 3.67;
    } else if(strcmp(grade, "B+") == 0) {
        gradePoints = 3.33;
    } else if(strcmp(grade, "B-") == 0) {
        gradePoints = 2.67;
    } else if(strcmp(grade, "C+") == 0) {
        gradePoints = 2.33;
    } else if(strcmp(grade, "C-") == 0) {
        gradePoints = 1.67;
    } else if(strcmp(grade, "D+") == 0) {
        gradePoints = 1.33;
    } else if(strcmp(grade, "D-") == 0) {
        gradePoints = 0.67;
    } else if(strncmp(grade, "A", 1) == 0) {
        gradePoints = 4;
    } else if(strncmp(grade, "B", 1) == 0) {
        gradePoints = 3;
    } else if(strncmp(grade, "C", 1) == 0) {
        gradePoints = 2;
    } else if(strncmp(grade, "D", 1) == 0) {
        gradePoints = 1;
    } else if(strncmp(grade, "E", 1) == 0) {
        gradePoints = 0;
    }

    return gradePoints;
}

struct Course *getCourseMemory(char id[10]) {
    struct Semester *currSemester = activeStudent->recentSemester;
    struct Course *currCourse = NULL;

    while(currSemester != NULL) {
        currCourse = currSemester->recentCourse;
        while (currCourse != NULL) {
            if(strncmp(id, myStrlwr(currCourse->id), strlen(id)) == 0) return currCourse;
            currCourse = currCourse->next;
        }
        currSemester = currSemester->next;
    } 

    return NULL;
}

struct Semester *getCourseSemesterMemory(char id[50]) {
    struct Semester *currSemester = activeStudent->recentSemester;
    struct Course *currCourse = NULL;

    while(currSemester != NULL) {
        currCourse = currSemester->recentCourse;
        while (currCourse != NULL) {
            if(strncmp(id, myStrlwr(currCourse->id), strlen(id)) == 0) return currSemester;
            currCourse = currCourse->next;
        }
        currSemester = currSemester->next;
    } 

    return NULL;
}

struct Semester *getSemesterMemory(struct Semester *currSemester, int semester) {
    if(currSemester == NULL) return NULL;
    if(currSemester->semester == semester) return currSemester;
    return getSemesterMemory(currSemester->next, semester);
}

struct Student *getStudentMemory(struct Student *currStudent, char name[50]) {
    if(currStudent == NULL)
        return NULL;
    if(strncmp(name, myStrlwr(currStudent->name), strlen(name)) == 0) 
        return currStudent;

    return getStudentMemory(currStudent->next, name);
}

int getStudentMemoryOrder(struct Student *theStudent) {
    struct Student *currStudent = recentStudent;
    int position = 0;
    if(theStudent == NULL) {
        return 0;
    } else {
        while(currStudent != NULL) {
            position++;
            if(currStudent == theStudent)
                return position;
            currStudent = currStudent->next;
        }
    }
    return 0;
}

bool isCourseExist(char id[10]) {
    struct Semester *currSemester = activeStudent->recentSemester;
    struct Course *currCourse;

    while(currSemester != NULL) {
        currCourse = currSemester->recentCourse;
        while(currCourse != NULL) {
            if(strncmp(id, myStrlwr(currCourse->id), strlen(id)) == 0) return true;
            currCourse = currCourse->next;
        }

        currSemester = currSemester->next;
    }
    return false;
} 

bool isSemesterExist(struct Semester *currSemester, int semester) {
    if(currSemester == NULL) return false;
    if(currSemester->semester == semester)  return true;
    
    return isSemesterExist(currSemester->next, semester);
}

bool isStudentExist(struct Student *currStudent, char name[50]) {
    if(currStudent == NULL) return false;
    if(strncmp(name, myStrlwr(currStudent->name), strlen(name)) == 0) return true;
    
    return isStudentExist(currStudent->next, name);
}

int mainMenu() {
    int choosenMenu;
    char yOrN;
    system("clear");
    if(activeStudent != NULL) {
        printf("1. New Student\n");
        printf("2. Change Student\n");
        printf("3. Delete Student\n");
        printf("4. Submit Information\n");
        printf("5. Modify Information\n");
        printf("6. Display\n");
        printf("0. Exit\n");
        printf("Enter menu : ");
        scanf("%d", &choosenMenu);
    } else {
        choosenMenu = 1;
    }
    switch (choosenMenu) {
    case 0:
        return 0;
        break;
    
    case 1: {
        char studentName[50];
        printf("New Student\n");
        printf("Name : ");
        scanf("%s", studentName);
        pushStudent(studentName);
        break;
    }
    case 2:{
        char studentName[50];
        printf("Change to student name : ");
        scanf("%s", studentName);
        changeStudent(myStrlwr(studentName));
        break;
    }
    case 3:
        printf("Delete Student\n");
        printf("Are you confirm to delete the student (y/n) : ");
        scanf(" %c", &yOrN);
        if(yOrN == 'y')
            popStudent(activeStudent);

        activeStudent = recentStudent;
        break;

    case 4:
        printf("Sumbit Information\n");
        submitMenu();
        break;

    case 5:
        printf("Modify Information\n");
        modifyMenu();
        break;

    case 6:
        printf("Display\n");
        displayMenu();

    default:
        break;
    }

    return 1;
}

void modifyMenu() {
    int choosenMenu;
    printf("1. Modify Course Information\n");
    printf("2. Delete a Course\n");
    printf("3. Delete a Semester"\n);
    printf("Choose the menu : ");
    scanf("%d", &choosenMenu);

    switch (choosenMenu) {
    case 1:{
        char id[10], name[50], grade[3];
        int creditHours;
        printf("Course ID    : ");
        scanf("%s", id);
        if(!isCourseExist(myStrlwr(id))) printf("The course is not exist, you will create the new one\n");
        printf("Course Name  : ");
        scanf("%s", name);
        printf("Credit Hours : ");
        scanf("%d", &creditHours);
        printf("Grade        : ");
        scanf("%s", grade);
        break;
    }

    case 2: {
        char id[10];
        printf("Course ID : ");
        scanf("%s", id);
        if(!isCourseExist(myStrlwr(id))) printf("The course is not exist\n");
        else popCourse(id);
    }

    case 3: {
        int semester;
        printf("Semester : ");
        scanf("%d", &semester);
        if(!isSemesterExist(activeStudent->recentSemester, semester)) printf("The semester is not exist\n");
        else popSemester(semester);
    }
    default:
        break;
    }
}

void popCourse(char id[10]) {
    struct Course *deletedCourse = getCourseMemory(id), *currCourse = getCourseSemesterMemory(id)->recentCourse;
    if(currCourse == deletedCourse) {
        activeStudent->recentSemester->recentCourse = deletedCourse->next;
        deletedCourse->next = NULL;
        free(deletedCourse);
    } else {
        while(currCourse->next != deletedCourse) {
            currCourse = currCourse->next;
        }
        currCourse->next = deletedCourse->next;
        deletedCourse->next = NULL;
        free(deletedCourse);
    }
}

void popSemester(int semester) {
    struct Semester *deletedSemester = getSemesterMemory(activeStudent->recentSemester, semester), *currSemester = activeStudent->recentSemester;
    if(currSemester == deletedSemester) {
        activeStudent->recentSemester = deletedSemester->next;
        deletedSemester->next = NULL;
        free(deletedSemester);
    } else {
        while(currSemester->next != deletedSemester) {
            currSemester = currSemester->next;
        }
        currSemester->next = deletedSemester->next;
        deletedSemester->next = NULL;
        free(deletedSemester);
    }
}

void popStudent(struct Student *theStudent) {
    struct Student *tempStudent = recentStudent;
    if(getStudentMemoryOrder(theStudent) == 1) {
        recentStudent = recentStudent->next;
    } else if(getStudentMemoryOrder(theStudent) > 1) {
        while (tempStudent->next != theStudent) {
            tempStudent = tempStudent->next;
        }
        tempStudent->next = theStudent->next;
    }
    free(theStudent);
}

void pushCourse(char id[10], char courseName[50], char grade[3], int creditHours, int semester) {
    struct Course *newCourse = (struct Course*) malloc(sizeof(struct Course));
    if(isCourseExist(myStrlwr(id))) {
        printf("The course is already exist, you will modify it");
        newCourse = getCourseMemory(id);
    }
    
    strcpy(newCourse->id, id);
    strcpy(newCourse->name, courseName);
    strcpy(newCourse->grade, grade);
    newCourse->gradePoints = convertGrade(grade);
    newCourse->creditHours = creditHours;

    if(!isCourseExist(myStrlwr(id))) {
        newCourse->next = getSemesterMemory(activeStudent->recentSemester, semester)->recentCourse;
        getSemesterMemory(activeStudent->recentSemester, semester)->recentCourse = newCourse;
    }
    calculateGpa(semester);
}

void pushSemester(int semester) {
    if(isSemesterExist(activeStudent->recentSemester, semester)) {
        printf("The semester is exist\n");
        return;
    } else {
        struct Semester *newSemester = (struct Semester*) malloc(sizeof(struct Semester));
        newSemester->semester = semester;

        newSemester->next = activeStudent->recentSemester;
        activeStudent->recentSemester = newSemester;
    }
}

void pushStudent(char name[50]) {
    struct Student *newStudent = (struct Student*) malloc(sizeof(struct Student));
    strcpy(newStudent->name, name);

    newStudent->next = recentStudent;
    recentStudent = newStudent;
    activeStudent = newStudent;
}

char *myStrlwr(char string[50]) {
    for (int i = 0; i < strlen(string); i++) {
        string[i] = tolower(string[i]);
    }
    return string;
}

void submitMenu() {
    char yOrN;
    bool isContinue = true;
    int semester, creditHours;
    char courseName[50], grade[3], id[10];
    while(isContinue) {
        printf("Enter semester : ");
        scanf("%d", &semester);
        pushSemester(semester);

        while(isContinue) {
            printf("\nEnter Course Information\n");
            printf("Course ID    : ");
            scanf("%s", id);
            printf("Course Name  : ");
            scanf("%s", courseName);
            fflush(stdin);
            printf("Credit Hours :");
            scanf("%d", &creditHours);
            printf("Grade        : ");
            scanf("%s", grade);
            pushCourse(id, courseName, grade, creditHours, semester);
            printf("Enter course again (y/n) : ");
            scanf(" %c", &yOrN);
            if(tolower(yOrN) == 'n') isContinue = false;
        }

        printf("Enter another semester (y/n) : ");
        scanf(" %c", &yOrN);
        if(tolower(yOrN) == 'y') isContinue = true;
    }
}