# Text Editor with Undo/Redo Functionality

This project is a simple text editor implemented in C, which supports basic text operations like insert, remove, undo, and redo. The editor uses a queue for word management and two stacks for undo and redo operations.

## Features
1. **Print the loaded text:** Displays the content of the text file.
2. **Insert text to the strings:** Allows the user to input text which is then appended to the original string.
3. **Remove strings from the list:** Allows the user to remove a specific word from the original string.
4. **Perform undo operation:** Reverts the last text modification.
5. **Perform redo operation:** Reapplies the last undone text modification.
6. **Print the Undo Stack and the Redo stack:** Displays the contents of the undo and redo stacks.
7. **Save the updated text to the output file:** Saves the modified text back to the file.
8. **Exit:** Exits the application.

## Data Structures
### Word Node
```c
struct word {
    char word[20];
    wordNode next;
    wordNode prev;
};
```
### Queue
```c
struct queue {
    wordNode front;
    wordNode rear;
};
```
### Undo/Redo Stack Node
```c
struct undoStack {
    char token[20];
    char operation[6];
    int index;
    undoNode next;
} undoStack, redoStack;
```
## Functions
### Queue Operations
* `queuePtr createWordsQueue()`: Creates and returns a new queue.
* `wordNode createWordNode(char word[])`: Creates and returns a new word node.
* `void enqueue(queuePtr q, char word[])`: Adds a word to the front of the queue.
* `void dequeue(queuePtr q)`: Removes and returns the word from the rear of the queue.
* `void deleteQueue(queuePtr q)`: Deletes the entire queue.
* `void printQueue(queuePtr q)`: Prints all words in the queue.
### Stack Operations
* `undoNode createUndoStack()`: Creates and returns a new undo stack.
* `void undoPush(undoNode top, char token[], const char *op, int index)`: Pushes a new element onto the undo stack.
* `void undoPop(undoNode top, char line[])`: Pops the top element from the undo stack and performs the corresponding undo operation.
* `void deleteUndoStack(undoNode top)`: Deletes the undo stack.
* `void printUndoStack(undoNode top)`: Prints all elements in the undo stack.
* `redoNode createRedoStack()`: Creates and returns a new redo stack.
* `void redoPush(redoNode top, char token[], const char *op, int index)`: Pushes a new element onto the redo stack.
* `void redoPop(redoNode top, char line[])`: Pops the top element from the redo stack and performs the corresponding redo operation.
* `void deleteRedoStack(redoNode top)`: Deletes the redo stack.
* `void printRedoStack(redoNode top)`: Prints all elements in the redo stack.
### String Operations
* `int getStartingIndex(char line[], char word[], int lenOfLine, int lenOfWord)`: Finds and returns the starting index of the word in the line.
* `void removeString(char line[], char word[], int lenOfLine, int lenOfWord, int startingIndex)`: Removes the word from the line starting at the given index.
* `void insertString(char line[], char word[], int lenOfLine, int lenOfWord, int startingIndex)`: Inserts the word into the line starting at the given index.

## Usage
1. Ensure you have a text file named input.txt in the same directory as the executable.
2. Run the program.
3. Follow the menu prompts to perform the desired operations.
