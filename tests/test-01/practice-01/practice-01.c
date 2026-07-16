#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    char name[101];
    char subject[10];
    char className;
    struct Student* next;
} Student;

// === ONLY CODE WITHIN THE DESIGNATED AREA . DO NOT MODIFY OTHER PARTS ===

Student* mergeExamLists(Student* mathList, Student* literatureList, Student* englishList, Student* physicsList) {
    Student* head = NULL;
    Student* tail = NULL;

    void appendList(Student* list) {
        if (!list) return;

        if (!head) {
            head = list;
        } else {
            tail->next = list;
        }

        while (list->next) {
            list = list->next;
        }
        tail = list;
    }

    appendList(mathList);
    appendList(literatureList);
    appendList(englishList);
    appendList(physicsList);

    return head;
}
// === DO NOT MODIFY OTHER PARTS ===

Student* makeNode(const char* name, const char* subject, char className) {
    Student* s = (Student*)malloc(sizeof(Student));
    strcpy(s->name, name);
    strcpy(s->subject, subject);
    s->className = className;
    s->next = NULL;
    return s;
}

void appendNode(Student** head, Student** tail, Student* node) {
    if (!*head) {
        *head = *tail = node;
    } else {
        (*tail)->next = node;
        *tail = node;
    }
}

void buildList(int count, const char* subject, Student** head, Student** tail) {
    for (int i = 0; i < count; i++) {
        char name[101], className;
        if (scanf("%100s %c", name, &className) != 2) return;
        appendNode(head, tail, makeNode(name, subject, className));
    }
}

void printList(Student* head) {
    while (head) {
        printf("%s-%s-%c", head->name, head->subject, head->className);
        if (head->next) printf("\n");
        head = head->next;
    }
}

int main() {
    int m, l, e, p;
    if (scanf("%d %d %d %d", &m, &l, &e, &p) != 4) return 0;

    Student *mathHead = NULL, *mathTail = NULL;
    Student *litHead = NULL, *litTail = NULL;
    Student *engHead = NULL, *engTail = NULL;
    Student *phyHead = NULL, *phyTail = NULL;

    buildList(m, "toan", &mathHead, &mathTail);
    buildList(l, "van", &litHead, &litTail);
    buildList(e, "anh", &engHead, &engTail);
    buildList(p, "vatly", &phyHead, &phyTail);

    Student* res = mergeExamLists(mathHead, litHead, engHead, phyHead);
    printf("\n__JUDGE_RESULT__\n");
    printList(res);
    return 0;
}