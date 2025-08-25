#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

struct queue {
    struct node *front, *rear;
};

void create_queue(struct queue *q) {
    q->front = q->rear = NULL;
}

void insert(struct queue *q, int val) {
    struct node *ptr = (struct node *)malloc(sizeof(struct node));
    ptr->data = val;
    ptr->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = ptr;
    } else {
        q->rear->next = ptr;
        q->rear = ptr;
    }
}

int delete(struct queue *q) {
    if (q->front == NULL) {
        printf("Queue Underflow\n");
        return -1;
    }

    struct node *ptr = q->front;
    int val = ptr->data;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(ptr);
    return val;
}

int peek(struct queue *q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return -1;
    }
    return q->front->data;
}

void display(struct queue *q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return;
    }

    struct node *temp = q->front;
    printf("Queue elements: ");
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main() {
    struct queue q;
    create_queue(&q);

    int option, val, result;

    do {
        printf("\n--- Menu ---\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Display \n");
        printf("4. Peek\n");
        printf("5. Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &val);
                insert(&q, val);
                printf("%d inserted into queue.\n", val);
                break;
            case 2:
                result = delete(&q);
                if (result != -1)
                    printf("Deleted element: %d\n", result);
                break;
            case 3:
                display(&q);
                break;
            case 4:
                result = peek(&q);
                if (result != -1)
                    printf("Front element: %d\n", result);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (option != 5);

    return 0;
}
