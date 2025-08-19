#include <stdio.h>
#define MAX 10

int queue[MAX];
int front = -1, rear = -1;

void insert(void);
int delete_element(void);
int peek(void);
void display(void);

int main() {
    int option, val;

    do {
        printf("\n ***** MAIN MENU *****");
        printf("\n 1. Insert an element");
        printf("\n 2. Delete an element");
        printf("\n 3. Peek");
        printf("\n 4. Display the queue");
        printf("\n 5. EXIT");
        printf("\n Enter your option : ");
        scanf("%d", &option);

        switch(option) {
            case 1:
                insert();
                break;
            case 2:
                val = delete_element();
                if(val != -1)
                    printf("\n The number deleted is : %d", val);
                break;
            case 3:
                val = peek();
                if(val != -1)
                    printf("\n The first value in queue is : %d", val);
                break;
            case 4:
                display();
                break;
            case 5:
                printf("\n Exiting program.");
                break;
            default:
                printf("\n Invalid option.");
        }
    } while(option != 5);

    return 0;
}

void insert() {
    int num;
    printf("\n Enter the number to be inserted in the queue : ");
    scanf("%d", &num);

    if ((front == 0 && rear == MAX - 1) || (rear + 1) % MAX == front) {
        printf("\n OVERFLOW");
        return;
    }

    if (front == -1 && rear == -1) {
        front = rear = 0;
    } else {
        rear = (rear + 1) % MAX;
    }

    queue[rear] = num;
}

int delete_element() {
    int val;

    if (front == -1 && rear == -1) {
        printf("\n UNDERFLOW");
        return -1;
    }

    val = queue[front];
    if (front == rear) {
        front = rear = -1;
    } else {
        front = (front + 1) % MAX;
    }

    return val;
}

int peek() {
    if (front == -1 && rear == -1) {
        printf("\n QUEUE IS EMPTY");
        return -1;
    }
    return queue[front];
}

void display() {
    int i;

    if (front == -1 && rear == -1) {
        printf("\n QUEUE IS EMPTY");
        return;
    }

    printf("\n Queue elements are:\n");

    i = front;
    while (1) {
        printf("\t%d", queue[i]);
        if (i == rear)
            break;
        i = (i + 1) % MAX;
    }
}
