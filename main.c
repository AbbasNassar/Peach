/* Abbas Nassar 1210482 Sec.3 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char tokenCommunicate[20];
char operationCommunicate[20];
int indexCommunicate;

struct word;
typedef struct word *wordNode;

struct queue;
typedef struct queue *queuePtr;

struct undoStack;
typedef struct undoStack *undoNode;

struct redoStack;
typedef struct undoStack *redoNode;

struct word
{

    char word[20];
    wordNode next;
    wordNode prev;
};

struct queue
{
    wordNode front;
    wordNode rear;
};

struct undoStack
{
    char token[20];    // Word to push or pop.
    char operation[6]; // Type of the operation (Insert/Remove).
    int index;         // The index to store the word to.
    undoNode next;     // Points to the next element in the stack.

} undoStack;

struct redoStack
{
    char token[20];    // Word to push or pop.
    char operation[6]; // Type of the operation (Insert/Remove).
    int index;         // The index to store the word to.
    undoNode next;     // Points to the next element in the stack.

} redoStack;

queuePtr createWordsQueue(); // Returns the head of the queue
wordNode createWordNode(char word[]);
void enqueue(queuePtr q, char word[]); // Insert always at the front of the queue
void dequeue(queuePtr q);              // Dequeue rear from queue and return its word
void deleteQueue(queuePtr q);          // Delete the entire queue
void printQueue(queuePtr q);           // Used to print the queue

undoNode createUndoStack();                                           // Returns the head of the undo stack
void undoPush(undoNode top, char token[], const char *op, int index); // Pushes new element to the stack
void undoPop(undoNode top, char line[]);                              // Pops the head from the undo stack
// I passed the original string to be able to edit on it
// The edit on the original string are based on the content of the node to be popped
void deleteUndoStack(undoNode top);                                   // Used to empty the stack
void printUndoStack(undoNode top);                                    // Used to print the stack in the wanted format

redoNode createRedoStack();                                           // Returns the head of the redo stack
void redoPush(redoNode top, char token[], const char *op, int index); // Pushes new element to the stack
void redoPop(redoNode top, char line[]);                              // Pops the head from the undo stack
// I passed the original string to be able to edit on it
// The edit on the original string are based on the content of the
void deleteRedoStack(redoNode top);                                   // Used to empty the stack
void printRedoStack(redoNode top);                                    // Used to print the stack in the wanted format

int getStartingIndex(char line[], char word[], int lenOfLine, int lenOfWord);                 // This function searches for the first occurrence of word in the original string,
// then it returns the index of the first character of the word,
// returns -1 if word not found
void removeString(char line[], char word[], int lenOfLine, int lenOfWord, int startingIndex); // Remove word from the original string by getting
// The index of the first char of the word in line,
// then perform a shift left operation on the characters starting by the first char after this word
// by the length of the deleted word
void insertString(char line[], char word[], int lenOfLine, int lenOfWord, int startingIndex); // Insert new word to original string by getting
// then perform a shift right operation on the characters starting by the first char after this word
// by the length of the inserted word
// and finally inserting the word in the created space
int main()
{
    char line[300]; // Create the string where to store the original string and perform operations on it
    /* Read string from file */
    FILE *file = fopen("../input.txt", "r");
    if (file == NULL)
    {
        printf("Error finding the file :(\n");
    }
    /* Read all the lines until the end is reached */
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line); // Print the read data
    }
    printf("\n");
    /* Close file */
    fclose(file);
    undoNode undoStackPtr = createUndoStack(); // Create the undo stack
    redoNode redoStackPtr = createRedoStack(); // Create the redo stack
    int selection = 0;                         // Menu
    do
    {
        printf("1- Print the loaded text\n");
        printf("2- Insert text to the strings\n");
        printf("3- Remove strings from the list\n");
        printf("4- Perform undo operation\n");
        printf("5- Perform redo operation\n");
        printf("6- Print the Undo Stack and the Redo stack\n");
        printf("7- Save the updated text to the output file\n");
        printf("8- Exit\n");
        scanf("%d", &selection);
        if (selection == 1)
        {
            /* Read string from file */
            FILE *file = fopen("../input.txt", "r");
            if (file == NULL)
            {
                printf("Error finding the file :(\n");
            }
            /* Read all the lines until the end is reached */
            while (fgets(line, sizeof(line), file) != NULL)
            {
                printf("%s", line); // Print the read data
            }
            printf("\n");
            /* Close file */
            fclose(file);
        }
        else if (selection == 2)
        {
            /* Creating the queue to store the read string on it word by word*/
            queuePtr queue = createWordsQueue();
            const char *operation = "Insert";
            /* String to store the read data*/
            char sentence[50];
            printf("Insert text:\n");
            getchar();
            gets(sentence);
            char *token;
            /* get the first token */
            token = strtok(sentence, " ");
            /* walk through other tokens */
            while (token != NULL)
            {
                /* Enqueue the word into the previously defined queue */
                enqueue(queue, token);
                token = strtok(NULL, " ");
            }
            /* Append what's in the queue to the original text */
            printf("Modified data:\n %s\n", line);
            while (queue->front != NULL)
            {
                char appWord[20];
                /* When i dequeue from the queue the first inserted word will be copied to the global variable tokenCommunicate (FCFS) */
                dequeue(queue);
                strcpy(appWord, tokenCommunicate);
                strcat(line, " ");
                strcat(line, appWord);
                undoPush(undoStackPtr, appWord, operation, -1);
            }
            /* Print the modified data */
            printf("Modified data:\n %s\n", line);
        }
        else if (selection == 3)
        {
            const char *operation = "Remove";
            char wordToDelete[20];
            printf("Please the word you want to remove: \n");
            scanf("%s", wordToDelete);
            int ls = strlen(line);
            int lw = strlen(wordToDelete);
            int startingIndex = getStartingIndex(line, wordToDelete, ls, lw); // returns the index in array
            if (startingIndex == -1)
            {
                printf("There is no such word as '%s' in the string :(\n", wordToDelete);
            }
            else
            {
                printf("%d\n", startingIndex);
                removeString(line, wordToDelete, ls, lw, startingIndex);
                printf("Modified data:\n%s\n", line);
                undoPush(undoStackPtr, wordToDelete, operation, startingIndex);
            }
        }
        else if (selection == 4)
        {
            undoPop(undoStackPtr, line);
            redoPush(redoStackPtr, tokenCommunicate, operationCommunicate, indexCommunicate);
            printf("Modified data:\n%s\n", line);
        }
        else if (selection == 5)
        {
            redoPop(redoStackPtr, line);
            undoPush(undoStackPtr, tokenCommunicate, operationCommunicate, indexCommunicate);
            printf("Modified data:\n%s\n", line);
        }
        else if (selection == 6)
        {
            printf("Undo stack content: \n");
            printUndoStack(undoStackPtr);
            printf("\nRedo stack content: \n");
            printRedoStack(redoStackPtr);
        }
        else if (selection == 7)
        {
            /* Store the data to the file */
            FILE *file = fopen("../input.txt", "w");

            if (file == NULL)
            {
                printf("Error in finding the file :(\n");
            }
            else
            {
                /* Print new string in the file */
                fprintf(file, "%s", line);
                printf("Data successfully stored in file.\n");
            }
            /* Close file */
            fclose(file);
        }
        else if (selection == 8)
        {
            printf("Best of luck :)\n");
            /* Delete all extra data */
            deleteUndoStack(undoStackPtr);
            deleteRedoStack(redoStackPtr);
            exit(0);
        }
        else
        {
            printf("Invalid selection number. Try again ;)\n");
        }

    } while (1);
    return 0;
}

queuePtr createWordsQueue()
{
    /* Allocate memory for the queue */
    queuePtr q = (queuePtr)malloc(sizeof(struct queue));

    if (q == NULL)
    {
        printf("Could not allocate queue. Memory is full.\n");
        exit(0);
    }
    q->front = q->rear = NULL;
    return q;
}
wordNode createWordNode(char word[])
{
    /* Allocate memory for the queue node */
    wordNode temp = (wordNode)malloc(sizeof(struct word));

    if (temp == NULL)
    {
        printf("Could not allocate a new word node. Memory allocation failed.\n");
        exit(0);
    }

    strcpy(temp->word, word);
    temp->next = NULL;
    temp->prev = NULL;

    return temp;
}
void enqueue(queuePtr q, char word[])
{
    /* Handle if the word is too big */
    if (strlen(word) > 20)
    {
        printf("The word you've entered is too big. The limit is 20 characters.\n");
        exit(0);
    }
    /* Creates a new word node with the word string provided */
    wordNode temp = createWordNode(word);
    /* Queue is empty */
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }
    /* Insert in front */
    q->front->prev = temp;
    temp->next = q->front;
    temp->prev = NULL;
    q->front = temp;
}
void dequeue(queuePtr q) // Dequeue rear from queue
{
    /* Handle if queue is empty */
    if (q->front == NULL)
    {
        printf("Queue is empty :(.\n");
        return;
    }
    strcpy(tokenCommunicate, q->rear->word);
    printf("Rear string %s\n", q->rear->word);
    if (q->front != q->rear)
    {
        wordNode temp = q->rear;
        q->rear = q->rear->prev;
        q->rear->next = NULL;
        free(temp);
    }
    else
    {
        q->rear = NULL;
        q->front = NULL;
    }
    // if rear becomes NULL, change rear also to NULL.
    if (q->rear == NULL)
    {
        q->front = NULL;
    }
}
void deleteQueue(queuePtr q)
{
    if (q->front == NULL)
    {
        printf("Queue is empty :(. Nothing to delete.\n");
        return;
    }
    wordNode currentNode = q->front;
    wordNode nextNode;

    /* iterate through the nodes and delete each one starting from the front node */
    while (currentNode != NULL)
    {
        nextNode = currentNode->next;
        free(currentNode);
        currentNode = nextNode;
    }
    /* Update the front and rear pointers of the queue */
    q->front = q->rear = NULL;
    printf("Queue deleted.\n");
}
void printQueue(queuePtr q)
{
    if (q->front == NULL)
    {
        printf("Queue is empty :(\n");
        return;
    }
    /* Print the queue from the front to the end */
    wordNode temp = q->front;
    while (temp != NULL)
    {
        printf("%s  ", temp->word);
        temp = temp->next;
    }
    printf("\n");
}

undoNode createUndoStack()
{
    undoNode S;
    S = (undoNode)malloc(sizeof(struct undoStack));
    if (S == NULL)
    {
        printf("Could not create stack\n");
        return NULL;
    }
    else
    {
        S->next = NULL;
    }

    return S;
}
void undoPush(undoNode top, char token[], const char *op, int index)
{
    undoNode newNode = (undoNode)malloc(sizeof(struct undoStack));

    if (top == NULL)
    {
        printf("Stack is not found\n");
        return;
    }
    else if (newNode == NULL)
    {
        printf("Out of space\n");
        return;
    }
    else if (top->next == NULL)
    {
        /* If stack is empty */
        /* Push the new node to the stack and update the top pointer */
        newNode->next = top->next;
        top->next = newNode;
        strcpy(newNode->token, token);
        strcpy(newNode->operation, op);
        newNode->index = index;
        return;
    }
    /* Push the new node to the stack and update the top pointer */
    strcpy(newNode->token, token);
    strcpy(newNode->operation, op);
    newNode->index = index;
    undoNode temp = top->next;
    top->next = newNode;
    newNode->next = temp;
}
void undoPop(undoNode top, char line[])
{
    undoNode temp;
    if (top == NULL)
    {
        printf("Stack is not found\n");
        return;
    }
    else if (top->next == NULL)
    {
        printf("Stack is empty\n");
        return;
    }
    else if (top->next->next == NULL)
    {
        /* Handle if stack has one element */
        temp = top->next;
        strcpy(tokenCommunicate, temp->token);
        strcpy(operationCommunicate, temp->operation);
        indexCommunicate = temp->index;
        top->next = NULL;
        /* Since we are in the undo stack we want to do the opposite of the operation name*/
        if (strcasecmp(temp->operation, "Insert") == 0)
        {
            /* Remove token in node from the original string */
            int startingIndex = getStartingIndex(line, temp->token, strlen(line), strlen(temp->token)); // returns the index in array
            printf("%d\n", startingIndex);
            removeString(line, temp->token, strlen(line), strlen(temp->token), startingIndex);
        }
        else
        {
            /* insert token in node to the original string */
            insertString(line, temp->token, strlen(line), strlen(temp->token), temp->index);
        }

        char *pointerToken = (char *)malloc(20 * sizeof(char));
        strcpy(pointerToken, temp->token);
        free(temp);
        return;
    }
    /* Stack has more than one element */
    /* Same algorithm as before */
    undoNode ptr = top->next->next;
    temp = top->next;
    strcpy(tokenCommunicate, temp->token);
    strcpy(operationCommunicate, temp->operation);
    indexCommunicate = temp->index;
    if (strcasecmp(temp->operation, "Insert") == 0)
    {
        int startingIndex = getStartingIndex(line, temp->token, strlen(line), strlen(temp->token));
        printf("%d\n", startingIndex);
        removeString(line, temp->token, strlen(line), strlen(temp->token), startingIndex);
        printf("The finished product is: %s\n", line);
    }
    else
    {
        insertString(line, temp->token, strlen(line), strlen(temp->token), temp->index);
    }
    top->next = ptr;
    /* Make the next node of the previous top to be the top */
    top = temp->next;
    free(temp);
}
void deleteUndoStack(undoNode top)
{
    if (top == NULL)
    {
        printf("Stack is not found\n");
        return;
    }
    else if (top->next == NULL)
    {
        printf("Stack is empty\n");
        return;
    }
    while (top->next == NULL)
    {
        undoPop(top, "");
    }
    printf("Stack has been deleted\n");
}
void printUndoStack(undoNode top)
{
    if (top == NULL)
    {
        printf("Stack is not found\n");
        return;
    }
    else if (top->next == NULL)
    {
        printf("Stack is empty\n");
        return;
    }
    /* Print all nodes in the requested format */
    undoNode temp = top->next;
    while (temp != NULL)
    {
        printf("%s | %s | %d\n", temp->token, temp->operation, temp->index);
        temp = temp->next;
    }
}

redoNode createRedoStack()
{
    redoNode S;
    S = (redoNode)malloc(sizeof(struct redoStack));
    if (S == NULL)
    {
        printf("Could not create stack\n");
        return NULL;
    }
    else
    {
        S->next = NULL;
    }

    return S;
}
void redoPush(redoNode top, char token[], const char *op, int index)
{
    redoNode newNode = (redoNode)malloc(sizeof(struct redoStack)); //<-- to find a address in the memo to save the new node in it.

    if (top == NULL)
    {
        printf("Stack is not found\n");
        return;
    }
    else if (newNode == NULL)
    {
        printf("Out of space\n");
        return;
    }
    else if (top->next == NULL)
    {
        newNode->next = top->next;
        top->next = newNode;
        strcpy(newNode->token, token);
        strcpy(newNode->operation, op);
        newNode->index = index;
        printf("%s  %s  %d pushed to the stack. \n", newNode->token, newNode->operation, newNode->index); // <-- to tell the user the push success.
        return;
    }

    strcpy(newNode->token, token);
    strcpy(newNode->operation, op);
    newNode->index = index;
    undoNode temp = top->next;
    top->next = newNode;
    newNode->next = temp;
    printf("%s  %s  %d pushed to the stack. \n", newNode->token, newNode->operation, newNode->index); // <-- to tell the user the push success.
}
void redoPop(redoNode top, char line[])
{
    /* Same as undo pop but since it is redo we want to make the operation equivalent to the one stored in node */
    redoNode temp;
    if (top == NULL)
    {
        printf("Stack is not found\n");
        return;
    }
    else if (top->next == NULL)
    {
        printf("Stack is empty\n");
        return;
    }
    else if (top->next->next == NULL)
    { // stack has one element
        temp = top->next;
        strcpy(tokenCommunicate, temp->token);
        strcpy(operationCommunicate, temp->operation);
        indexCommunicate = temp->index;
        if (strcasecmp(temp->operation, "Insert") == 0)
        {
            insertString(line, temp->token, strlen(line), strlen(temp->token), temp->index);
        }
        else
        {
            int startingIndex = getStartingIndex(line, temp->token, strlen(line), strlen(temp->token)); // returns the index in array
            printf("%d\n", startingIndex);
            removeString(line, temp->token, strlen(line), strlen(temp->token), startingIndex);
        }
        top->next = NULL;
        free(temp);
        return;
    }

    redoNode ptr = top->next->next;
    strcpy(tokenCommunicate, temp->token);
    strcpy(operationCommunicate, temp->operation);
    indexCommunicate = temp->index;
    if (strcasecmp(temp->operation, "Insert") == 0)
    {
        insertString(line, temp->token, strlen(line), strlen(temp->token), temp->index);
    }
    else
    {
        int startingIndex = getStartingIndex(line, temp->token, strlen(line), strlen(temp->token)); // returns the index in array

        printf("%d\n", startingIndex);
        removeString(line, temp->token, strlen(line), strlen(temp->token), startingIndex);
    }
    top->next = ptr;
    char *pointerToken = (char *)malloc(20 * sizeof(char));
    strcpy(pointerToken, temp->token);
    top = temp->next;
    free(temp);
}
void deleteRedoStack(redoNode top)
{
    if (top == NULL)
    {
        printf("Stack is not found\n");
        return;
    }
    else if (top->next == NULL)
    {
        printf("Stack is empty\n");
        return;
    }
    while (top->next == NULL)
    {
        redoPop(top, "");
    }
    printf("Stack has been deleted\n");
}
void printRedoStack(redoNode top)
{
    if (top == NULL)
    {
        printf("Stack is not found\n");
        return;
    }
    else if (top->next == NULL)
    {
        printf("Stack is empty\n");
        return;
    }
    // will print all the nodes:
    redoNode temp = top->next;
    while (temp != NULL)
    {
        printf("%s | %s | %d\n", temp->token, temp->operation, temp->index);
        temp = temp->next;
    }
}

int getStartingIndex(char line[], char word[], int lenOfLine, int lenOfWord)
{

    int startingIndex = -1;

    for (int i = 0; i < lenOfLine; i++)
    {
        int equalChars = 0;
        for (int j = 0; j < lenOfWord; j++)
        {
            if (line[i] != word[j])
                break;
            else
            { // Line[i] == word[j]
                equalChars++;
                i++;
            }
        }
        if (equalChars == lenOfWord)
        {
            startingIndex = i - lenOfWord;
            break;
        }
    }
    return startingIndex;
}
void removeString(char line[], char word[], int lenOfLine, int lenOfWord, int startingIndex)
{
    int i;
    int isSpace = 0;
    for (i = startingIndex; i <= lenOfLine - lenOfWord; i++)
    {
        int j = i + lenOfWord + isSpace;
        /* Handle space */
        if (line[i - 1] == ' ' && line[j] == ' ')
        {
            j++;
            line[i] = line[j];
            isSpace++;
        }
        else
        {
            line[i] = line[j];
        }
    }
    line[i] = '\0';
}
void insertString(char line[], char word[], int lenOfLine, int lenOfWord, int startingIndex)
{

    if (startingIndex == -1)
        startingIndex = lenOfLine;
    /* Shifting loop */
    for (int i = lenOfLine; i >= startingIndex; i--)
    {
        line[i + lenOfWord + 1] = line[i];
    }
    int j = 0;
    for (int i = startingIndex; i <= lenOfLine + lenOfWord; i++)
    {

        if (word[j] == '\0')
        {
            line[i] = ' ';
            break;
        }
        line[i] = word[j];
        j++;
    }
}