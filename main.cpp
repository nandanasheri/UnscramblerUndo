/* 	Program 6 - Unscrambler Undo

	The user is presented with a board that is filled with scrambled words.
	They have to rotate the rows and columns to get the words displayed in the right order.

	Author: Nandana Sheri
    Date: 11/24/2021
    Class: CS 141, Fall 2021, UIC
    System: CLion on PC

    Running the program looks like:
        Program 6: Unscrambler Undo
        CS 141, Fall 2021, UIC

        You will be presented with a board that is filled with scrambled words.
        Rotate the rows and columns to get the words displayed in the right order.
        Press 'U' to undo moves.

        1192 words have been read in from the dictionary.
           Choose board size. It must be a single digit >=4: 4
           Choose single-digit number of times to scramble board: 1

               A   B   C   D         Goal Board
              ----------------     ---------------
           0 | b |   | o | m |    | b | o | o | m |
              ----------------     ---------------
           1 |   | o | o | o |    |   | t | o | o |
              ----------------     ---------------
           2 | c | t | p | i |    |   | p | i | c |
              ----------------     ---------------
           3 | k |   | t | o |    | k |   | t | o |
              ----------------     ---------------
           Board string: b om oooctpik to
            Goal string: boom too pick to

        Enter one of the following:
           R Rotate a row or column
           G Generate a new board with new random words
           Q to quit.
        1. Your choice:
*/

#include <iostream>		// For Input and Output
#include <fstream>		// For file input and output
#include <cassert>		// For the assert statement
#include <vector>		// For vectors
#include <iomanip>		// For setw() and setfill()
#include <random>       // To use rand
using namespace std;

// Class Board should be declared here and should have private data members for:
//    1. The board letters (as a string or vector of char)
//    2. The list of words randomly selected from the dictionary
//    3. What the original unscrambled board looks like (which is also the completed board)
//    4. The size of the board
// Your Board class should not include the dictionary, because if it does then each Node
// would have its own copy, and you only need a single copy of the dictionary.

// Creation of a class Board to implement the board of the game with its size.
class Board {
public:
    void setSize (int size)                                     //size of the board
    { this -> size = size; }                                    // Function to set the board to a user input size
    void setRandomWords (string randomWords) {
        this -> randomWords = randomWords;
    }
    void setScramble ( int scramble) {                          // Function to set extent of scrambling
        this -> scramble = scramble;
    }
    void setCompletedBoard ( string completedBoard) {           // Function to set the completed board words
        completedBoardWords = completedBoard;
    }
    void setInitialBoardWords (string initialBoardWords) {      // Function to set board words after scrambling
        this -> initialBoardWords = initialBoardWords;
    }
    int getSize () { return size; }                                 // Function to get the size of the board
    string getRandomWords() { return randomWords;}                  // Function to get the random words
    int getScramble () { return scramble; }                         // Function get the extent of scrambling
    string getCompletedBoardWords () { return completedBoardWords; }// Function to get the original random words
    void displayBoard();                                            // To display the board at any instance
    void scrambleBoard();                                           // To scramble the board
    void rotateRowOnce(int rowOrCol);                               // To rotate the row once
    void rotateColOnce(int rowOrCol);                               // To rotate the column once
private:
    int size;                                                       // Size of the board is stored
    string randomWords;                                             // Random words generated are stored here
    int scramble;                                                   // The extent of scrambling stored here
    string completedBoardWords;                                     // What the completed board should be
    string initialBoardWords;                                       // Stores the random words after the first scramble
};

// Set up the board based on the random words chosen from the dictionary
void initializeBoard(
        Board &currentBoard,                   // The board, of type board
        const vector<string> &boardWords)   // Words to be placed on the board
        {
    string theBoard = "";
    int boardSize = currentBoard.getSize() * currentBoard.getSize();
    // Create the board by concatenating all the words, with a space between them.
    int i;
    for( i=0; i<boardWords.size()-1; i++) {
        theBoard = theBoard + boardWords[ i] + " ";
    }

    // Separately append the last word, without a following blank, in case the words fit
    // perfectly without any extra blanks at the end.
    theBoard = theBoard + boardWords[ i];
    // Append extra spaces if needed so board ends up the right size
    while( theBoard.size() < boardSize) {
        theBoard += " ";
    }
    currentBoard.setRandomWords(theBoard);              // Board words set as a member of the class
    currentBoard.setInitialBoardWords(theBoard);      // Initial board words set as a member of the class
        } // end initializeBoard()

        //---------------------------------------------------------------------------
        // Read in all the words from the dictionary file into a vector
        void readWordsIntoDictionary(
                vector <string> & dictionary,   // Dictionary where words will be stored
                string fileName)                // File name where words will be read from
                {
    ifstream inStream;                  // Declare an input stream for reading
    inStream.open( fileName);            // Open dictionary file
    assert( inStream.fail() == false );  // Ensure file open worked

    // Clear vector in case it already had some words in it
    dictionary.clear();

    // Keep repeating while input from the file yields a word
    string newWord;                     // Store a single input word
    while( inStream >> newWord) {        // While there is another word to be read
        // Add this new word to the end of the vector, growing it in the process
        dictionary.push_back(newWord);
    }

    // Close the dictionary file
    inStream.close();
                } //end readWordsIntoDictionary()

        //---------------------------------------------------------------------------
        // Select random words from the dictionary to be used for the board
        void selectRandomWords(
                vector<string> dictionaryWords,	    // Vector of dictionary words
                vector<string> &wordsChosen,		// Vector to store and return the words that were randomly chosen
                Board &currentBoard,                 // Current Board used at any instance of type Board
                int maxTotalLengthOfWords)			// Max total length of all the words plus spaces
                {
    string allWordsPlusSpaces = "";

    // Clear the random words vector, in case this is not the first time we do this.
    wordsChosen.clear();

    while( allWordsPlusSpaces.length() < maxTotalLengthOfWords) {
        // Get a random word from the dictionary
        int randomWordIndex = rand() % dictionaryWords.size();
        string randomWord = dictionaryWords.at( randomWordIndex);
        // Verify that this new word will not go past the end of the board
        if( (randomWord.length() + allWordsPlusSpaces.length()) > maxTotalLengthOfWords) {
            continue;
        }
        // Length of new word is okay, so add it
        wordsChosen.push_back( randomWord);
        allWordsPlusSpaces += randomWord + " "; // add space at the end to separate words
    } // end while(allWordsPlusSpaces.length()...
    currentBoard.setRandomWords(allWordsPlusSpaces);        // Sets the board words as a string
    currentBoard.setCompletedBoard(allWordsPlusSpaces);    // Sets the board words for completion
    } // end selectRandomWords()

    //---------------------------------------------------------------------------
    // Display the current board.  While we visualize the board as two-dimensional,
    // we store it as a single string, with the rows concatenated after each other.
    // Function to display the board before the game with random words
    void Board::displayBoard() {
    // Display the top column letters.  Also clear the fill character, otherwise
    // it could be leftover as '-' from the previous board display.
    cout << endl
    << "    " << setfill(' ');
    for( int i=0; i<size; i++) {
        cout << setw( 4) << (char)('A'+i);
    }
    // Display the heading above the solved board on the right
    cout << "         Goal Board";

    // Display the board character by character, formatted as a square board
    for( int row=0; row<size; row++) {
        // Display a separator row of dash characters, like: "--------------"
        cout << "  \n      " << setw(size*4+3) << setfill('-') << "   ";
        // Display a second row of dashes for the right-hand goal board
        cout << "  " << setw(size*4+3) << setfill('-') << "\n   ";

        // Display the row number
        cout << row << "";

        // Display the working board values on the left
        for(int col=0; col<size; col++) {
            int index = row*size + col; // compute the index into the 1D board vector
            cout << " | " << randomWords[index];
        }
        cout << " |   ";      // Last '|' at the end of each line.

        // Display the solved board values to the right
        for(int col=0; col<size; col++) {
            int index = row*size + col; // compute the index into the 1D board vector
            cout << " | " << completedBoardWords[index];
        }
        cout << " | ";      // Last '|' at the end of each line.
    }

    // Display the bottom line on the board
    cout << "\n      " << setw(size*4) << setfill('-') << "";
    // Display the bottom line on the Goal board to the right
    cout << "     " << setw(size*4) << setfill('-') << "\n";

    // Display the board characters and goal characters as strings.
    cout << "   Board string: " << randomWords << endl;
    cout << "    Goal string: " << completedBoardWords
    << endl
    << endl;
}
            // ---------------------------------------------------------------------------
            // Rotate characters within a single row of the board
            void Board :: rotateRowOnce(
                    int rowToRotate)        // 0..(n-1) where n is the size of one side of the square board
                    {
    // Calculate the indices on the board for the start and end of the row
    int rowStartingBoardIndex = rowToRotate * size;
    int rowEndingBoardIndex = rowStartingBoardIndex + size - 1;
    // Store the last element on the row, as shifting right will overwrite it
    char lastElement = randomWords.at( rowEndingBoardIndex);
    // Shift all row elements to the right by one, starting from the
    // right, working our way back to the left.
    // E.g.      b e l o w
    //  becomes: w b e l o
    for( int i=rowEndingBoardIndex; i>rowStartingBoardIndex; i--) {
        randomWords.at(i) = randomWords.at( i-1);  // Overwrite element i with its left neighbor
    }
    // Overwrite the left-most element of the row with what used to be the last element
    randomWords.at( rowStartingBoardIndex) = lastElement;
                    } // end rotateRow()
                    // ---------------------------------------------------------------------------
                    // Rotate characters within a single column of the board
                    void Board :: rotateColOnce(
                            int colToRotate)        // 'A'..('A'+n-1) where n is the size of one side of the square board
                            {
    // Calculate the indices on the board for the start and end of the column
    int colStartingBoardIndex = colToRotate;
    int colEndingBoardIndex = colStartingBoardIndex + (size * (size - 1));
    // Store the last element in the column, as shifting down will overwrite it
    char bottomElement = randomWords.at( colEndingBoardIndex);

    // Shift all column elements to down by one, starting from the
    // bottom, working our way back up to the top.
    // E.g.  b		w
    //		 e		b
    //		 l	==>	e
    //		 o		l
    //		 w		o
    for( int i=colEndingBoardIndex; i>colStartingBoardIndex; i = i - size) {
        randomWords.at(i) = randomWords.at( i-size);  // Overwrite element i with its above neighbor
    }
    // Overwrite the top-most element of the row with what used to be the last element
    randomWords.at( colStartingBoardIndex) = bottomElement;
                            } // end rotateCol()
                            //---------------------------------------------------------------------------
                            // Do the user-selected board rotation consisting of the row number or column
                            // letter, followed by the number of rotations, which can be negative.
                            // E.g. 2 1 would rotate row 2 to the right by 1
                            //      a -2 would rotate the first column (column A) up by 2
                            void handleRotation(
                                    Board &currentBoard )        // Current Board of type Board
                                    {
    char rowOrCol = ' ';
    int numPositionsToRotate = 0;
    int boardCharsPerSide = currentBoard.getSize();
    // Infinite loop to handle user input validation
    while( true) {
        cout << "   Enter the row/column to rotate, and # of (+/-) rotations: ";
        cin >> rowOrCol >> numPositionsToRotate;
        rowOrCol = toupper(rowOrCol);   // Convert rowOrCol value to upper-case.  Numbers don't end up changing.

        // See if rowOrCol is not within the valid range
        if( !( (rowOrCol >= '0' && rowOrCol <= '0'+boardCharsPerSide - 1) ||   // Row is within valid row range
        (rowOrCol >= 'A' && rowOrCol <= 'A'+boardCharsPerSide - 1)      // Column is within valid column range
        )
        ) {
            // It is not within the valid ragne
            cout << "   *** Row/column value is not within the valid range.  Retry." << endl;
            continue;
        }
        // Validate numeric shift amount is within range
        if( abs( numPositionsToRotate) > boardCharsPerSide - 1) {
            cout << "   *** Shift value should be between -" << boardCharsPerSide - 1
            << " and " << boardCharsPerSide - 1 << ". Retry."
            << endl;
            continue;
        }

        // Now that row/col and # of rotations validation tests have passed, break out of the loop
        break;
    } //end while( true)

    // If the number of rotations is negative, translate it to the corresponding positive amount.
    // E.g. In a 4x4 board, rotating a column up 1 (-1) is equivalent to shifting it down 4-1 = 3 times.
    //		In a 5x5 board, rotating a row left 3 (-3) is equivalent to shifting right 5-3 = 2 times
    if( numPositionsToRotate < 0) {
        numPositionsToRotate = boardCharsPerSide + numPositionsToRotate;
    }

    // User input has been validated and converted to a positive number of shifts.  Do this many shifts.
    for( int i=0; i<numPositionsToRotate; i++) {
        if( rowOrCol >= '0' && rowOrCol <= '0'+9) {
            // Convert the row value '0'..'9' to a numeric value 0..9
            currentBoard.rotateRowOnce(rowOrCol-'0');
        }
        else {
            // Convert the column value 'A'..'I' to a numeric value 0..9
            currentBoard.rotateColOnce(rowOrCol-'A');
        }
    } //end for( int i...
    } //end handleRotation()
    // Scramble the board, based on the selected difficulty level
    void Board::scrambleBoard() {
    // Prompt for and get the number of board scrambling steps.  Each step does a random row
    // rotation and then a random column rotation.
    scramble = -1;
    // Loop to validate user input
    do {
        cout << "   Choose single-digit number of times to scramble board: ";
        cin >> scramble;

        // Validate user input, reprompting if not one of the valid options
        if ( scramble < 1 || scramble > 9) {
            cout << "   *** The number of times must be a single digit >= 1. Retry." << endl;
        }
    } while( scramble < 1 || scramble > 9);

    // Rotate random rows and then a random column. Repeat this <numScrambles> times.
    for( int i=0; i < scramble; i++) {
        // Rotate a random row
        int randRow = rand() % size;
        rotateRowOnce(randRow);

        // Rotate a random column
        int randCol = rand() % size;
        rotateColOnce(randCol);
    }
}// end scrambleBoard()
// Clear the board, generate random words, initialize the board, store copy
// of completed board to use in checking for a win, scramble the initial
// board and store a copy to allow resetting the board.
//    All this is done in this function because it is done once at the
// beginning of the game, and is also done again if 'G' to generate a new
// board is chosen.
void setUpBoards(
        vector<string> dictionaryWords, // List of all dictionary words, for random selections
        vector<string> &boardWords,     // List of words randomly selected from dictionary
        Board &currentBoard)             // Current Board of type Board
        {
    // Prompt for and get board size
    int boardCharsPerSide = currentBoard.getSize();
    do {
        cout << "   Choose board size. It must be a single digit >=4: ";
        cin >> boardCharsPerSide;

        // Validate input to be >= 4 and <= 9
        if( boardCharsPerSide < 4 || boardCharsPerSide > 9) {
            cout << "   *** The board size must be a single digit >=4. Retry." << endl;
        }
    } while( boardCharsPerSide < 4 || boardCharsPerSide > 9);
    currentBoard.setSize(boardCharsPerSide);                    // Sets the board size
    int boardSize = boardCharsPerSide * boardCharsPerSide;		// Total number of chars on the board

    // Blank out all initial boards, in case this is not the first time doing setup.
    string theBoard = currentBoard.getRandomWords();          // Playing board
    theBoard = "";
    string completedBoard = currentBoard.getCompletedBoardWords();
    completedBoard = "";    // Completed board, used to check for win

    // Generate the random words, store a copy of the completed board, create and store
    // a copy of the initial scrambled board.
    selectRandomWords( dictionaryWords, boardWords, currentBoard, boardSize);
    // Append the words onto the board, with spaces in between
    initializeBoard( currentBoard, boardWords);

    // Store the completed board, to later allow checking for win. Stored as a member of the class
    completedBoard = currentBoard.getRandomWords();
    currentBoard.setCompletedBoard(completedBoard);

    // Scramble the board, prompting for the number of desired rotations, which sets the difficulty.
    currentBoard.scrambleBoard();
        } //end setUpBoards()


                                                                        // ******  IMPORTANT!!!  READ THIS!!!  ******
                                                                        //
                                                                        // FIRST get the program working using classes BEFORE implementing the linked-list portion
                                                                        // outlined in the code below.
                                                                        //
                                                                        // Once your classes version of program 5 is working also implement a Node struct or class
                                                                        // used to store each move.  Your Node class should have private data members for:
                                                                        //       1. A Board  (of type Board defined above)
                                                                        //       2. A moveNumber
                                                                        //       3. A pointer to the next Node
                                                                        // Implement class Board here
                                                                        // ...

                                                                        //-------------------------------------------------------------------------------------
                                                                        // Node declaration to implement a linked list to store moves, used to implement
                                                                        // undo. You should store the old move number, the old board, and a pointer
                                                                        // to the next Node.  Note that for this program you are not required to use
                                                                        // a class for the Node.  It could just be a struct without worrying about the
                                                                        // proper uses of public and private.
                                                                        class Node {
public:
    Board theBoard;                                                 // Stores the entire class board in theBoard
    int moveNumber;                                                 // Each move number of the game is stored here
    Node *pNext;                                                    // Pointer to next node on the list
};

// Display the move numbers and board letters in each Node on the linked list.
// The pTemp pointer should iterate down the list, being used to display the
// values in each Node.
void displayList( Node *pTemp) {
    cout << "List is: ";
    while (pTemp != NULL) {
        Board playingBoard = pTemp -> theBoard;
        int moveNumber = pTemp -> moveNumber;
        string theBoard = playingBoard.getRandomWords();
        pTemp = pTemp -> pNext;
        cout << moveNumber << ":" << theBoard;
        if (pTemp != NULL) {                          // For greater than 1 node in the list
            cout << " -> ";
        }
    }
    cout << endl;
} // end displayList()


//--------------------------------------------------------------------------------
// Undo the move represented by the front node on the list, then delete this node.
// Parameters should be:
//  - pointer to the head of the list, which could change, so should be: Node *&pHead
//  - the old game Board to be restored from what is initially the second
//    Node on the list, before the front Node is deleted.
//  - the old moveNumber, which should be restored again from what is initially
//    the second Node on the list, before the front Node is deleted
void deleteFrontNodeFromList(Node *&pHead, Board &previousBoard, int &oldMoveNumber) {
    // There should always be at least one Node (the starting board) on the list.
    // If there is only this single Node, then give an error message and don’t
    // do the undo, since we are already at the beginning of the game.
    if (pHead -> pNext == NULL) {
        cout << "*** You cannot undo past the beginning of the game.  Retry. ***" << endl;
    }
    else {
        cout << "* Undoing move *" << endl;
        Node *pTemp;                    // Temporary pointer pTemp
        pTemp = pHead;                  // initialized to pHead
        pHead = pHead -> pNext;         // pHead points to previous board
        delete pTemp;                   // Latest move is then deleted
        previousBoard = pHead -> theBoard;
        oldMoveNumber = pHead -> moveNumber;
    }

} //end deleteFrontNodeFromList()

void deleteEntireList (Node *pHead) {
    Node *pTemp;
    while (pHead != NULL) {
        pTemp = pHead;
        pHead = pHead -> pNext;
        delete pTemp;
    }
}
//--------------------------------------------------------------------------------
// Create a new node and prepend it to the beginning of the list.
// Parameters should be:
//  - pointer to the head of the list, which could change, so should be Node *&pHead.
//  - the Board to be added to the new Node at the front of the list
//  - the current moveNumber to be added to the new Node at the front of the list
void prependNodeToList(Node *&pHead, Board currentBoard, int moveNumber) {
    Node *pTemp = new Node ;                        // Initially a Temporary pointer allocates space on the heap
    pTemp -> theBoard = currentBoard;               // Data is added onto the new node
    pTemp -> moveNumber = moveNumber;
    pTemp -> pNext = pHead;                         // Points to the next node in the list
    pHead = pTemp;                                  // As we are prepending to the list pHead now points to new node
}

int main()
{
    // For random number generation seed the random number generator to 1, so that results
    // are predictable.  To make results different every time instead you would
    // use srand( time( NULL));   Don't do this if you want your code to match test cases!
    srand( 1);
    Board currentBoard;             // Current Board of type Board is initialized
    vector<string> dictionaryWords;	// Vector to store the words from the dictionary
    vector<string> boardWords;		// Vector to store the random words chosen to be included in the board
    int boardCharsPerSize = 5;		// Default number of rows and columns of the board
    currentBoard.setSize(boardCharsPerSize);                    // Sets size initially as 5
    int boardSize = boardCharsPerSize * boardCharsPerSize;		// Total number of characters on the board
    string theBoard;			    // The board, that changes as moves are made
    currentBoard.setRandomWords(theBoard); // Sets random words as a member of the class
    string completedBoard;	        // The completed board, used to check for a win
    currentBoard.setCompletedBoard(completedBoard); // Sets completed board as a member of the class
    char userInput = ' ';			// Character to read in the user input
    int moveNumber = 1;             // Move number, incremented after each valid move
    Node *pHead = NULL;             // pHead is a pointer to the beginning of the list initialized to NULL

    // Display program identification and overview.
    cout << "Program 6: Unscrambler Undo \n"
    << "CS 141, Fall 2021, UIC	     \n"
    << " \n"
    << "You will be presented with a board that is filled with scrambled words.    \n"
    << "Rotate the rows and columns to get the words displayed in the right order. \n"
    << "Press 'U' to undo moves.   \n"
    << endl;

    // Read in all the words from dictionary file into a vector.
    // Word list adapted from https://www.ef.com/wwen/english-resources/english-vocabulary/top-3000-words/
    readWordsIntoDictionary( dictionaryWords, "commonWords.txt");
    cout << dictionaryWords.size() << " words have been read in from the dictionary." << endl;

    // Select random words and setup all the boards
    setUpBoards( dictionaryWords, boardWords, currentBoard);
    theBoard=currentBoard.getRandomWords();
    prependNodeToList(pHead, currentBoard, moveNumber);     // First board is stored into the list
    // Loop to prompt for and process user input
    while(true) {
        currentBoard.displayBoard();
        displayList(pHead);                                                // Print list of nodes
        cout << "Enter one of the following: \n"
        << "   R Rotate a row or column \n"
        << "   G Generate a new board with new random words \n"
        << "   U Undo most recent move\n"
        << "   Q to quit. \n"
        << moveNumber << ". Your choice: ";
        cin >> userInput;
        userInput = toupper(userInput);

        // Handle menu options
        switch( userInput) {
            // 'Q' for Quit
            case 'Q': {
                cout << "Thank you for playing! Exiting program... \n" << endl;
                exit( 0);      // Exit the program.
                break;
            }
            // 'R' to rotate
            case 'R': {
                handleRotation( currentBoard);
                theBoard = currentBoard.getRandomWords();                       // Updates theBoard string
                moveNumber++;
                prependNodeToList(pHead,currentBoard,moveNumber); // Prepends new node to list after rotation
                break;
            }
            // 'G' to generate a new board with new random words
            case 'G': {
                // Select random words and setup all the boards
                moveNumber = 1;
                setUpBoards( dictionaryWords, boardWords, currentBoard);
                deleteEntireList(pHead);
                pHead = NULL;
                prependNodeToList(pHead, currentBoard, moveNumber);
                break;
            }
            case 'U': {
                // To undo moves using linked lists
                deleteFrontNodeFromList(pHead,currentBoard,moveNumber);
                break;
            }
            default: {
                cout << "You did not enter one of the valid options. Try again." << endl;
                break;
            }
        } //end switch( userInput)

        // Check for a win
        theBoard = currentBoard.getRandomWords();
        completedBoard = currentBoard.getCompletedBoardWords();
        if( theBoard == completedBoard) {
            currentBoard.displayBoard();
            cout << "Congratulations, you won! \n"
            << "Exiting program... \n"
            << endl;
            return 0;
        }

        // Increment move number
    } // end while(true)

    return 0;
} //end main()

