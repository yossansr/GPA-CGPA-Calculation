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
    bool status;
    float gradePoints;
    struct Course *next;
};

struct Semester {
    int semester;
    float gpa;
	int totalCreditHours;
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

void displaySemesterCourse(struct Semester*, struct Course*);
void display(struct Semester*, struct Course*);
void displayMenu();
void displayFailCourse(struct Semester*, struct Course*);

bool generateCourseStatus(float);
struct Course *getCourseMemory(char[10]);
struct Semester *getCourseSemesterMemory(char[10]);
int getCourseSemester(char[10]);
struct Semester *getSemesterMemory(struct Semester*, int);
struct Student *getStudentMemory(struct Student*, char[50]);
int getStudentMemoryOrder(struct Student*);
int getStudentCreditHours();

bool isCourseExist(char[10]);
bool isSemesterExist(struct Semester*, int);
bool isStudentExist(struct Student*, char[50]);

int mainMenu();
void modifyMenu();

void popCourse(char[10]);
void popSemester(int);
void popStudent(struct Student*);
void pushCourse(char[10], char[50], char[3], int, int);
void pushSemester(int);
void pushStudent(char[50]);

void sortSemesterAsc();
void sortSemesterGpaDesc();
void submitMenu();

char *myStrlwr(char[50]);
char *myStrupr(char[50]);


int main() {
    bool isContinue = true;
    while(isContinue) isContinue = mainMenu();
    return 0;
}

void calculateCgpa() {
    struct Semester *currSemester = activeStudent->recentSemester;
    float cgpa = 0;
    int totalSemester = 0;

    while(currSemester) {
        totalSemester++;
        cgpa += currSemester->gpa;

        currSemester = currSemester->next;
    }

    activeStudent->cgpa = cgpa / (float)totalSemester;
}


void calculateGpa(int semester) {
    if(!isSemesterExist(activeStudent->recentSemester, semester)) return;
    struct Course *currCourse = getSemesterMemory(activeStudent->recentSemester, semester)->recentCourse;
    float totalOfGradePoints = 0, gpa = 0;
    int totalOfCreditHours = 0;
    while(currCourse) {
        totalOfCreditHours += currCourse->creditHours;
        totalOfGradePoints += ((float)currCourse->creditHours * currCourse->gradePoints);
        currCourse = currCourse->next;
    }
    gpa = totalOfGradePoints / (float)totalOfCreditHours;
    getSemesterMemory(activeStudent->recentSemester, semester)->gpa = gpa;
}



void changeStudent(char name[50]) {
    char tempName[30];
    strcpy(tempName, name);
    if(!isStudentExist(recentStudent, myStrupr(name))) {
        printf("Student not found\n");
        return;
    } else activeStudent = getStudentMemory(recentStudent, tempName);
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

void displayMenu() {
	int choosenMenu;
	system("cls");
	printf(" DISPLAY MENU\n");
	printf("|====|===================================|\n");
    printf("| NO | Menu                              |\n");
    printf("|====|===================================|\n");
	printf("| 1. | Display Semester Ascending        |\n");
	printf("| 2. | Display Semester with Highest GPA |\n");
	printf("| 3. | Display Course in a Semester      |\n");
	printf("| 4. | Display Failed Course             |\n");
	printf("|====|===================================|\n");
	printf("Choose the menu : ");
	scanf("%d",&choosenMenu);

	switch(choosenMenu) {
		case 1:
			system("cls");
			printf("Display Semester Ascending\n");
			sortSemesterAsc();
			display(activeStudent->recentSemester, activeStudent->recentSemester->recentCourse);

            system( "pause" );
			break;
        case 2:
        	system("cls");
            printf("Display Semester with Highest GPA\n");
			sortSemesterGpaDesc();
            display(activeStudent->recentSemester, activeStudent->recentSemester->recentCourse);

            system( "pause" );
            break;
    	case 3:{
    		int semester;
    		system("cls");
    		printf("Display Course in a Semester\n");
    		printf("Enter semester : ");
    		scanf("%d", &semester);
    		sortSemesterAsc();
    		if(isSemesterExist(activeStudent->recentSemester, semester)) {
    			system("cls");
    			displaySemesterCourse(getSemesterMemory(activeStudent->recentSemester, semester), getSemesterMemory(activeStudent->recentSemester, semester)->recentCourse);
			} else {
				printf("Semester is not exist\n");
			}
			system("pause");
			break;
		}
		case 4:
			system("cls");
			printf("Display Fail Course\n");
			sortSemesterAsc();
			displayFailCourse(activeStudent->recentSemester, activeStudent->recentSemester->recentCourse);

            system( "pause" );
			break;

	}
}

int getStudentCreditHours() {
	struct Semester *currSemester = activeStudent->recentSemester;
	int i = 0;
	while(currSemester) {
		i += currSemester->totalCreditHours;
		currSemester = currSemester->next;
	}
	return i;
}

void display(struct Semester *currSemester, struct Course *currCourse) {
	if(!currCourse) {
		printf("|===========|===================================================================|\n");
		printf("| GPA of This Semester : %.2f\n", currSemester->gpa);
		printf("|===========|===================================================================|\n\n");
        if(currSemester->next) return display(currSemester->next, currSemester->next->recentCourse);
        else {
        	printf("\nTotal of Credit Hours : %d\n", getStudentCreditHours());
            printf("%s's CGPA : %.2f\n\n", activeStudent->name, activeStudent->cgpa);
            printf("|===============================================================================|\n");
            return;
        }
	}
    if(currCourse == getSemesterMemory(activeStudent->recentSemester, currSemester->semester)->recentCourse) {
        printf("<<<< Information of Semester %d >>>>\n", currSemester->semester);
        printf("|===========|===================================================================|\n");
        printf("| COURSE ID | COURSE NAME                           | CREDIT HOURS  | GRADE     |\n");
        printf("|===========|===================================================================|\n");

    }

	char status[10];
	if (currCourse->status) strcpy(status, "Pass");
	else strcpy(status, "Fail");
	int i;
	printf("| %s", myStrupr(currCourse->id));
	for(i = 0; i < 10 - strlen(currCourse->id); i++) printf(" ");

	printf("| %s", myStrupr(currCourse->name));
	for(i = 0; i < 38 - strlen(currCourse->name); i++) printf(" ");


	printf("| %d", currCourse->creditHours);
	for(i = 0; i < 13; i++) printf(" ");

	printf("| %s (%s)", currCourse->grade, status);
	for(i = 0; i < 10 - (strlen(currCourse->grade) + strlen(status) + 3); i++) printf(" ");
	printf("|\n");

	return display(currSemester, currCourse->next);
}

void displaySemesterCourse(struct Semester *currSemester, struct Course *currCourse) {
	if(currCourse == currSemester->recentCourse) {
		currSemester = getCourseSemesterMemory(currCourse->id);
		printf("Information Course of Semester %d\n", currSemester->semester);
		printf("|===========|===================================================================|\n");
        printf("| COURSE ID | COURSE NAME                           | CREDIT HOURS  | GRADE     |\n");
        printf("|===========|===================================================================|\n");
	}
	if(!currCourse) {
		printf("|===========|===================================================================|\n");
		printf("| GPA : %.2f\n", currSemester->gpa);
		printf("|===========|===================================================================|\n\n");
		return;
	} else {
		char status[10];
		if (currCourse->status) strcpy(status, "Pass");
		else strcpy(status, "Fail");
		int i;
		printf("| %s", myStrupr(currCourse->id));
		for(i = 0; i < 10 - strlen(currCourse->id); i++) printf(" ");

		printf("| %s", myStrupr(currCourse->name));
		for(i = 0; i < 38 - strlen(currCourse->name); i++) printf(" ");

		printf("| %d", currCourse->creditHours);
		for(i = 0; i < 13; i++) printf(" ");

		printf("| %s (%s)", currCourse->grade, status);
		for(i = 0; i < 10 - (strlen(currCourse->grade) + strlen(status) + 3); i++) printf(" ");
		printf("|\n");
		return displaySemesterCourse(currSemester ,currCourse->next);
	}
}

void displayFailCourse(struct Semester *currSemester, struct Course *currCourse) {
	if(!currCourse) {
		printf("|===========|===================================================================|\n\n");
        if(currSemester->next) return displayFailCourse(currSemester->next, currSemester->next->recentCourse);
        else return;
	}

    if(currCourse == getSemesterMemory(activeStudent->recentSemester, currSemester->semester)->recentCourse) {
        printf("<<<< Information Failed Course of Semester %d >>>>\n", currSemester->semester);
        printf("|===========|===================================================================|\n");
        printf("| COURSE ID | COURSE NAME                           | CREDIT HOURS  | GRADE     |\n");
        printf("|===========|===================================================================|\n");
    }

	char status[10];
	if (!currCourse->status) {
		strcpy(status, "Fail");
		int i;
		printf("| %s", myStrupr(currCourse->id));
		for(i = 0; i < 10 - strlen(currCourse->id); i++) printf(" ");

		printf("| %s", myStrupr(currCourse->name));
		for(i = 0; i < 38 - strlen(currCourse->name); i++) printf(" ");

		printf("| %d", currCourse->creditHours);
		for(i = 0; i < 13; i++) printf(" ");

		printf("| %s (%s)", currCourse->grade, status);
		for(i = 0; i < 10 - (strlen(currCourse->grade) + strlen(status) + 3); i++) printf(" ");
		printf("\n");
	}

	return displayFailCourse(currSemester, currCourse->next);
}

bool generateCourseStatus(float point) {
    if(point < 2) return false;
    return true;
}

struct Course *getCourseMemory(char id[10]) {
    struct Semester *currSemester = activeStudent->recentSemester;
    struct Course *currCourse = NULL;

    while(currSemester) {
        currCourse = currSemester->recentCourse;
        while (currCourse) {
            if(strncmp(id, myStrlwr(currCourse->id), strlen(id)) == 0) return currCourse;
            currCourse = currCourse->next;
        }
        currSemester = currSemester->next;
    }

    return NULL;
}

int getCourseSemester(char id[10]) {
    struct Semester *currSemester = activeStudent->recentSemester;
    struct Course *currCourse = NULL;

    while(currSemester != NULL) {
        currCourse = currSemester->recentCourse;
        while (currCourse) {
            if(strncmp(id, myStrlwr(currCourse->id), strlen(id)) == 0) return currSemester->semester;
            currCourse = currCourse->next;
        }
        currSemester = currSemester->next;
    }

    return -1;
}

struct Semester *getCourseSemesterMemory(char id[10]) {
    struct Semester *currSemester = activeStudent->recentSemester;
    struct Course *currCourse = NULL;

    while(currSemester) {
        currCourse = currSemester->recentCourse;
        while (currCourse) {
            if(strncmp(id, myStrlwr(currCourse->id), strlen(id)) == 0) return currSemester;
            currCourse = currCourse->next;
        }
        currSemester = currSemester->next;
    }

    return NULL;
}

struct Semester *getSemesterMemory(struct Semester *currSemester, int semester) {
    if(!currSemester) return NULL;
    if(currSemester->semester == semester) return currSemester;
    return getSemesterMemory(currSemester->next, semester);
}

struct Student *getStudentMemory(struct Student *currStudent, char name[50]) {
    char tempName[30];
    strcpy(tempName, name);
    if(!currStudent)
        return NULL;
    if(strncmp(myStrlwr(name), myStrlwr(currStudent->name), strlen(currStudent->name)) == 0) {
        strcpy(currStudent->name, myStrupr(tempName));
        return currStudent;
    }
    return getStudentMemory(currStudent->next, tempName);
}

int getStudentMemoryOrder(struct Student *theStudent) {
    struct Student *currStudent = recentStudent;
    int position = 0;
    if(!theStudent) {
        return 0;
    } else {
        while(currStudent) {
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

	if(!currSemester->recentCourse) return false;

    while(currSemester) {
        currCourse = currSemester->recentCourse;
        while(currCourse) {
            if(strncmp(id, myStrlwr(currCourse->id), strlen(id)) == 0) return true;
            currCourse = currCourse->next;
        }

        currSemester = currSemester->next;
    }
    return false;
}

bool isSemesterExist(struct Semester *currSemester, int semester) {
    if(!currSemester) return false;
    if(currSemester->semester == semester)  return true;

    return isSemesterExist(currSemester->next, semester);
}

bool isStudentExist(struct Student *currStudent, char name[50]) {
    if(!currStudent) return false;
    if(strncmp(name, myStrupr(currStudent->name), strlen(currStudent->name)) == 0) return true;

    return isStudentExist(currStudent->next, name);
}

int mainMenu() {
    int choosenMenu;
    char yOrN;
    system("cls");
    if(!activeStudent) {
		printf("Hello Guest, please add a student first\n");
		system("pause");
	}
    else printf("Hello %s!\n\n", activeStudent->name);

    if(activeStudent) {
    	calculateCgpa();
    	printf(" MAIN MENU\n");
    	printf("|====|======================|\n");
    	printf("| NO | Menu                 |\n");
    	printf("|====|======================|\n");
        printf("| 0. | Exit                 |\n");
        printf("| 1. | New Student          |\n");
        printf("| 2. | Change Student       |\n");
        printf("| 3. | Delete Student       |\n");
        printf("| 4. | Submit Information   |\n");
        printf("| 5. | Modify Information   |\n");
        printf("| 6. | Display              |\n");
        printf("|===========================|\n");
        printf("Enter menu : ");
        scanf("%d", &choosenMenu);
    } else {
        choosenMenu = 1;
    }
    switch (choosenMenu) {
    case 0:
    	system("cls");
        return 0;
        break;

    case 1: {
    	system("cls");
        char studentName[50];
        fflush(stdin);
        printf("New Student\n");
        printf("Name : ");
        scanf("%[^\n]%*c", studentName);
        pushStudent(myStrupr(studentName));
        break;
    }
    case 2:{
    	system("cls");
        char studentName[50];
        fflush(stdin);
        printf("Change to student name : ");
        scanf("%[^\n]%*c", studentName);
        if(isStudentExist(recentStudent, myStrupr(studentName))) changeStudent(myStrupr(studentName));
        else printf("The student is not exist\n");
		system("pause");
        break;
    }
    case 3:
    	system("cls");
        printf("Delete Student\n");
        printf("Are you confirm to delete the student (y/n) : ");
        scanf(" %c", &yOrN);
        if(tolower(yOrN) == 'y')
            popStudent(activeStudent);

        activeStudent = recentStudent;
        break;

    case 4:
    	system("cls");
        printf("Sumbit Information\n");
        submitMenu();
        break;

    case 5:
    	if(!activeStudent->recentSemester) {
    		printf("You don't have any semester\n");
    		system("pause");
    		return 1;
		}
    	system("cls");
        printf("Modify Information\n");
        modifyMenu();
        break;

    case 6:
    	if(!activeStudent->recentSemester) {
    		printf("You don't have any semester\n");
    		system("pause");
    		return 1;
		}
    	system("cls");
        printf("Display\n");
        displayMenu();
        break;

    default:
    	system("cls");
    	printf("BAD INPUT!\n");
    	system("pause");
        break;
    }

    return 1;
}

void modifyMenu() {
    int choosenMenu;
    printf(" Modify Menu\n");
    printf("|====|===========================|\n");
    printf("| No | Menu                      |\n");
    printf("|====|===========================|\n");
    printf("| 1. | Modify Course Information |\n");
    printf("| 2. | Delete a Course           |\n");
    printf("| 3. | Delete a Semester         |\n");
    printf("|====|===========================|\n");
    printf("Choose the menu : ");
    scanf("%d", &choosenMenu);
	system("cls");
    switch (choosenMenu) {
    case 1:{
        char id[10], name[50], grade[3];
        int creditHours;
        printf("Course ID    : ");
        scanf("%s", id);
        if(!isCourseExist(myStrlwr(id))) {
            printf("The course is not exist\n");
            system( "pause" );
            return;
        }
        fflush(stdin);
        printf("Course Name  : ");
        scanf("%[^\n]%*c", name);
        printf("Credit Hours : ");
        scanf("%d", &creditHours);
        printf("Grade        : ");
        scanf("%s", grade);
        pushCourse(id, name, grade, creditHours, getCourseSemester(id));
        break;
    }

    case 2: {
        char id[10];
        printf("Course ID : ");
        scanf("%s", id);
        if(!isCourseExist(myStrlwr(id))) printf("The course is not exist\n");
        else {
        	popCourse(id);
			printf("Course %s has been deleted\n", id);
		}
        system("pause");
        break;
    }

    case 3: {
        int semester;
        printf("Semester : ");
        scanf("%d", &semester);
        if(!isSemesterExist(activeStudent->recentSemester, semester)) printf("The semester is not exist\n");
        else {
        	popSemester(semester);
			printf("Semester %d has been deleted\n", semester);
		}
        system("pause");
        break;
    }
    default:
        break;
    }
}

void popCourse(char id[50]) {
    struct Course *deletedCourse = getCourseMemory(id), *currCourse = getCourseSemesterMemory(id)->recentCourse;
    struct Semester *currSemester = getCourseSemesterMemory(id);
    currSemester->totalCreditHours -= deletedCourse->creditHours;
    if(currCourse == deletedCourse) {
        activeStudent->recentSemester->recentCourse = deletedCourse->next;
        deletedCourse->next = NULL;
        deletedCourse = NULL;
    } else {
        while(currCourse->next != deletedCourse) {
            currCourse = currCourse->next;
        }
        currCourse->next = deletedCourse->next;
        deletedCourse->next = NULL;
        deletedCourse = NULL;
    }
    if(!activeStudent->recentSemester->recentCourse) popSemester(currSemester->semester);
	else calculateGpa(currSemester->semester);

    free(deletedCourse);

}


void popSemester(int semester) {
    struct Semester *deletedSemester = getSemesterMemory(activeStudent->recentSemester, semester), *currSemester = activeStudent->recentSemester;
    if(currSemester == deletedSemester) {
        activeStudent->recentSemester = deletedSemester->next;
        deletedSemester->next = NULL;
        deletedSemester = NULL;
        free(deletedSemester);
    } else {
        while(currSemester->next != deletedSemester) {
            currSemester = currSemester->next;
        }
        currSemester->next = deletedSemester->next;
        deletedSemester->next = NULL;
        deletedSemester = NULL;
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
        printf("The course is already exist, you will modify it\n\n");
        system( "pause" );
        newCourse = getCourseMemory(id);
    }
    getSemesterMemory(activeStudent->recentSemester, semester)->totalCreditHours += creditHours;
    strcpy(newCourse->id, id);
    strcpy(newCourse->name, courseName);
    strcpy(newCourse->grade, myStrupr(grade));
    newCourse->gradePoints = convertGrade(grade);
    newCourse->creditHours = creditHours;
    newCourse->status = generateCourseStatus(newCourse->gradePoints);

    if(!isCourseExist(myStrlwr(id))) {
        newCourse->next = getSemesterMemory(activeStudent->recentSemester, semester)->recentCourse;
        getSemesterMemory(activeStudent->recentSemester, semester)->recentCourse = newCourse;
    }
    calculateGpa(semester);
}

void pushSemester(int semester) {
    if(isSemesterExist(activeStudent->recentSemester, semester)) {
        printf("The semester is exist\n");
        system("pause");
        return;
    } else {
        struct Semester *newSemester = (struct Semester*) malloc(sizeof(struct Semester));
        newSemester->semester = semester;
		newSemester->recentCourse = NULL;
		newSemester->totalCreditHours = 0;
        newSemester->next = activeStudent->recentSemester;
        activeStudent->recentSemester = newSemester;
    }
}

void pushStudent(char name[50]) {
    struct Student *newStudent = (struct Student*) malloc(sizeof(struct Student));
    strcpy(newStudent->name, name);
    newStudent->recentSemester = NULL;

    newStudent->next = recentStudent;
    recentStudent = newStudent;
    activeStudent = newStudent;
}

void sortSemesterAsc() {
	struct Semester *currSemester = activeStudent->recentSemester, *currSemester2 = NULL, *minSemester = currSemester, *tempSemester = NULL;

	while(currSemester) {
		currSemester2 = currSemester->next;
		minSemester = currSemester;

		while(currSemester2) {
			if(minSemester->semester > currSemester2->semester) minSemester = currSemester2;

			currSemester2 = currSemester2->next;
		}

		tempSemester = (struct Semester*) malloc(sizeof(struct Semester));
		tempSemester->semester = currSemester->semester;
		tempSemester->totalCreditHours = currSemester->totalCreditHours;
		tempSemester->gpa = currSemester->gpa;
		tempSemester->recentCourse = currSemester->recentCourse;
		currSemester->semester = minSemester->semester;
		currSemester->gpa = minSemester->gpa;
		currSemester->recentCourse = minSemester->recentCourse;
		currSemester->totalCreditHours = minSemester->totalCreditHours;
		minSemester->semester = tempSemester->semester;
		minSemester->gpa = tempSemester->gpa;
		minSemester->recentCourse = tempSemester->recentCourse;
		minSemester->totalCreditHours = tempSemester->totalCreditHours;

		currSemester = currSemester->next;
	}
}

char *myStrlwr(char string[50]) {
	int i;
    for (i = 0; i < strlen(string); i++) {
        string[i] = tolower(string[i]);
    }
    return string;
}

void sortSemesterGpaDesc() {
    struct Semester *currSemester = activeStudent->recentSemester, *currSemester2 = NULL, *maxSemester = NULL, *tempSemester = NULL;

	while(currSemester) {
		currSemester2 = currSemester->next;
		maxSemester = currSemester;

		while(currSemester2) {
			if(maxSemester->gpa < currSemester2->gpa) maxSemester = currSemester2;

			currSemester2 = currSemester2->next;
		}

		tempSemester = (struct Semester*) malloc(sizeof(struct Semester));
		tempSemester->semester = currSemester->semester;
		tempSemester->gpa = currSemester->gpa;
		tempSemester->recentCourse = currSemester->recentCourse;
		tempSemester->totalCreditHours = currSemester->totalCreditHours;
		currSemester->semester = maxSemester->semester;
		currSemester->gpa = maxSemester->gpa;
		currSemester->totalCreditHours = maxSemester->totalCreditHours;
		currSemester->recentCourse = maxSemester->recentCourse;
		maxSemester->totalCreditHours = tempSemester->totalCreditHours;
		maxSemester->semester = tempSemester->semester;
		maxSemester->gpa = tempSemester->gpa;
		maxSemester->recentCourse = tempSemester->recentCourse;

		currSemester = currSemester->next;
	}
}

char *myStrupr(char string[50]) {
	int i;
    for (i = 0; i < strlen(string); i++) {
        string[i] = toupper(string[i]);
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
        	system("cls");
            printf("\nEnter Course Information of Semester %d\n", semester);
            printf("Course ID    : ");
            scanf("%s", id);
            fflush(stdin);
            printf("Course Name  : ");
            scanf("%[^\n]%*c", courseName);
            fflush(stdin);
            printf("Credit Hours : ");
            scanf("%d", &creditHours);
            fflush(stdin);
            printf("Grade        : ");
            scanf("%s", grade);
            pushCourse(id, courseName, grade, creditHours, semester);
            printf("Enter course again (y/n) : ");
            scanf(" %c", &yOrN);
            if(tolower(yOrN) == 'n') isContinue = false;
        }
		system("cls");
        printf("Enter another semester (y/n) : ");
        scanf(" %c", &yOrN);
        if(tolower(yOrN) == 'y') isContinue = true;
        else calculateCgpa();
    }
}
