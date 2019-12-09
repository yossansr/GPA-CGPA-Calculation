#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct Course {
    char name[50];
    float gradePoints;
    char grade[3];
    int creditHours;
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
    struct Semester *recentSemester;
    struct Student *next;
};

struct Student *recentStudent = NULL;

char activeStudent[50] = "\0";

// Function Declaration
void pushStudent(char[50]);
void pushSemester(int);
void pushCourse(struct Course, int);
void popStudent();
void popSemester();
void popCourseWithDetails(char[50]);

void calculateGpa(int);
float convertGrade(char[3]);
float calculateCgpa();

void sortSemesterASC();
void displayAll(struct Semester *);

bool mainMenu();
void submitInformationMenu();

struct Semester* checkSemester(int);
struct Course* checkCourse(char[50]);
struct Semester* checkCourseSemester(char[50]);
int coursePosition(char[50]);
int lastCourse(char[50]);

int main() {
    pushStudent("Yossan");
    popSemester();

    return 0;
}

void pushStudent(char name[50]) {
    struct Student *newStudent = (struct Student*) malloc(sizeof(struct Student));
    strcpy(newStudent->name, name);

    newStudent->next = recentStudent;
    recentStudent = newStudent;
}

void pushSemester(int semester) {
    struct Semester *newSemester = (struct Semester*) malloc(sizeof(struct Semester));
    newSemester->semester = semester;

    newSemester->next = recentStudent->recentSemester;
    recentStudent->recentSemester = newSemester;
}

void popSemester() {
    struct Semester *tempSemester = NULL;

    if(recentStudent->recentSemester == NULL || recentStudent == NULL) {
        printf("Semester Underflow\n");
    } else {
        tempSemester = recentStudent->recentSemester;

        recentStudent->recentSemester = recentStudent->recentSemester->next;

        tempSemester->next = NULL;

        free(tempSemester);
    }
}

void pushCourse(struct Course course, int semester) {
    struct Course *newCourse = NULL;
    if(checkSemester(semester) == NULL)
        newCourse = (struct Course*) malloc(sizeof(struct Course));
    else 
        newCourse = checkSemester(semester)->recentCourse;
    strcpy(newCourse->name, course.name);
    strcpy(newCourse->grade, course.grade);
    newCourse->gradePoints = convertGrade(course.grade);
    newCourse->creditHours = course.creditHours;
    if(checkSemester(semester) == NULL) {
        newCourse->next = recentStudent->recentSemester->recentCourse;
        recentStudent->recentSemester->recentCourse = newCourse;
        calculateGpa(recentStudent->recentSemester->semester);
    }
}

void popCourseWithDetails(char courseName[50]) {
    // Actualy temp course link is the link before the link that we want
    struct Course *tempCourse = checkCourse(courseName), *deletedCourse = NULL;
    if(tempCourse == NULL)
        printf("Course Not Found");
    else {
        // Check if it's in middle or not
        if(coursePosition(courseName) > 0 && coursePosition(courseName) != lastCourse(courseName)) {
            deletedCourse = tempCourse->next;
            tempCourse->next = tempCourse->next->next;
            free(deletedCourse);
        } else if(coursePosition(courseName) == 0) {
            struct Semester *currSemester = checkCourseSemester(courseName);
            tempCourse = currSemester->recentCourse;
            currSemester->recentCourse = currSemester->recentCourse->next;
            tempCourse->next = NULL;
            free(tempCourse);
        }
    }
}

void calculateGpa(int semester) {
    struct Semester *currSemester = recentStudent->recentSemester;
    bool found = false;
    while(!found && currSemester != NULL) {
        if(currSemester->semester == semester) {
            found = true;
        } else {
            currSemester = currSemester->next;
        }
    }

    struct Course *currCourse = currSemester->recentCourse;
    float totalOfGradePoints = 0, gpa = 0;
    int totalOfCreditHours = 0;
    while (currCourse != NULL) {
        totalOfCreditHours += currCourse->creditHours;
        totalOfGradePoints += ((float)currCourse->creditHours * currCourse->gradePoints);
        currCourse = currCourse->next;
    }

    gpa = totalOfGradePoints / (float)totalOfCreditHours;
    currSemester->gpa = gpa;
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

float calculateCgpa() {
    struct Semester *currSemester = recentStudent->recentSemester;
    float cgpa = 0;
    int totalSemester = 0;
    
    while(currSemester != NULL) {
        totalSemester++;
        cgpa += currSemester->gpa;

        currSemester = currSemester->next;
    }

    return cgpa / (float)totalSemester;
}

void sortSemesterASC() {
    struct Semester *currSemester = recentStudent->recentSemester;
    struct Semester *currSemester2 = currSemester->next;
    struct Semester *minSemester = NULL, *tempSemester = NULL;

    while (currSemester != NULL) {
        minSemester = currSemester;
        while (currSemester->next != NULL) {
            if(minSemester->semester > currSemester2->semester) {
                minSemester = currSemester2;
                currSemester2 = currSemester2->next;
            }
        }
        tempSemester = currSemester;
        currSemester = minSemester;
        minSemester = tempSemester;
        
        currSemester = currSemester->next;
    }

    displayAll(currSemester);
    
}

void displayAll(struct Semester *currSemester) {
    while (currSemester != NULL) {
        printf("<<<< Semester %d >>>>\n", currSemester->semester);
        struct Course *currCourse = currSemester->recentCourse;
        while (currCourse != NULL) {
            printf("Course Name  : %s\n", currCourse->name);
            printf("Credit Hours : %d\n", currCourse->creditHours);
            printf("Grade        : %s\n", currCourse->grade);
            printf("Grade Points : %.2f\n\n", currCourse->gradePoints);
            currCourse = currCourse->next;
        }
        printf("GPA Result : %.2f\n\n", currSemester->gpa);
        currSemester = currSemester->next;
    }
    printf("CGPA Result : %.2f\n\n\n\n", calculateCgpa());
}

bool mainMenu() {
    if(activeStudent[0] == '\0') {
        printf("Hello Guest\n\n");
    } else {
        printf("Hello %s\n\n", activeStudent);
    }
    int choosenMenu = 0, choosenSemester;
    bool isContinue = true;
    printf("==========================================\n");
    printf("======GPA & CGPA CALCULATION PROGRAM======\n");
    printf("==========================================\n");
    printf("1. New Student\n");
    printf("2. Delete Student\n");
    printf("3. Submit Information\n");
    printf("4. Delete Information\n");
    printf("5. Display Information\n");
    printf("6. Exit\n");
    printf("Enter the menu : ");
    scanf("%d", &choosenMenu);
    
    switch (choosenMenu)
    {
    case 1:
        printf("Add New Student\n");
        char name[50];
        printf("Enter your name : ");
        scanf("%s", name);
        pushStudent(name);
        break;
    
    case 2:
        break;

    case 3:
        printf("Submit Information\n");
        submitInformationMenu();
        break;
    
    case 4:
        printf("Delete Information\n");
        printf("1. Delete Semester");
        printf("2. Delete Course");
        printf("Enter the menu : ");
        scanf("%d", &choosenMenu);

        switch (choosenMenu)
        {
        case 1:
            
            break;
        

        default:
            break;
        }
        break;
    default:
        break;
    }

    return isContinue;
}

void submitInformationMenu() {
    struct Course course;
    int semester = 1;
    bool isContinueCourse, isContinueSemester = true;
    char yOrN;
    while(isContinueSemester) {
        isContinueCourse = true;
        printf("Enter the semester : ");
        scanf("%d", &semester);
        getchar();
        if(checkSemester(semester) == NULL) {
            pushSemester(semester);
        }
        else {
            printf("The semester is exist, do you want to change it (y/n) : ");
            scanf(" %c", &yOrN);
            if(yOrN == 'n' || yOrN == 'N')
                return;
        }
        printf("Enter the informations of Semester %d\n", semester);
        while(isContinueCourse) {
            printf("Enter Course Name : ");
            fgets(course.name, 50, stdin);
            printf("Enter Credit Hours : ");
            scanf("%d", &course.creditHours);
            getchar();
            printf("Enter Grade : ");
            fgets(course.grade, 3, stdin);
            system("clear");
            pushCourse(course, semester);
            printf("Enter next course (y/n) : ");
            scanf(" %c", &yOrN);
            if(yOrN == 'n' || yOrN == 'N')
                isContinueCourse = false;
            getchar();
        }
        printf("Enter next semester (y/n) : ");
        scanf(" %c", &yOrN);
        if(yOrN == 'N' || yOrN == 'n')
            isContinueSemester = false;
    }
}

struct Semester* checkSemester(int semester) {
    struct Semester *currSemester = recentStudent->recentSemester, *theSemester = NULL;
    while (currSemester != NULL) {
        if(semester == currSemester->semester) {
            theSemester = currSemester;
        }
    }
    return theSemester;
}

struct Course* checkCourse(char courseName[50]) {
    struct Semester *currSemester = recentStudent->recentSemester;
    struct Course *currCourse = NULL, *theCourse = NULL, *tempCourse = NULL;
    int startTemp = 0;
    
    while(currSemester != NULL) {
        tempCourse = currCourse;
        if(startTemp == 0) {
            startTemp++;
        } else if(startTemp == 1) {
            tempCourse = currSemester->recentCourse;
        }
        currCourse = currSemester->recentCourse;
        while (currCourse != NULL) {
            if (strncmp(courseName, currCourse->name, strlen(courseName)) == 0) {
                theCourse = tempCourse;
            }
            tempCourse = tempCourse->next;
            currCourse = currCourse->next;
        }
        currSemester = currSemester->next; 
    }

    return theCourse;
}

struct Semester* checkCourseSemester(char courseName[50]) {
    struct Semester *currSemester = recentStudent->recentSemester, *theSemester = NULL;
    struct Course *currCourse = NULL, *theCourse = NULL;
    
    while(currSemester != NULL) {
        currCourse = currSemester->recentCourse;
        while (currCourse != NULL) {
            if (strncmp(courseName, currCourse->name, strlen(courseName)) == 0) {
                theSemester = currSemester;
            }
            
            currCourse = currCourse->next;
        }
        currSemester = currSemester->next; 
    }

    return theSemester;
}

int coursePosition(char courseName[50]) {
    struct Semester *currSemester = recentStudent->recentSemester;
    struct Course *currCourse = NULL;
    int currPosition, position;
    
    while(currSemester != NULL) {
        currPosition = -1;
        currCourse = currSemester->recentCourse;
        while (currCourse != NULL) {
            currPosition++;
            if (strncmp(courseName, currCourse->name, strlen(courseName)) == 0) {
                 position = currPosition;
            }
            
            currCourse = currCourse->next;
        }
        currSemester = currSemester->next; 
    }

    return position;
}

int lastCourse(char courseName[50]) {
    struct Semester *currSemester = checkCourseSemester(courseName);
    struct Course *currCourse = currSemester->recentCourse;
    int lastCourse = -1;
    while(currCourse != NULL) {
        lastCourse++;
    }

    return lastCourse;
}