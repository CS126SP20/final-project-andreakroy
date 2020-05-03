# Development

---

 - **4/19/20** Implemented Piece abstract base class 
   - Need to override the class for each piece
   - Need to implement move logic for each piece

 - **4/21/20** Completed implementations for all the piece classes.
    - Working on fixing a memory leak in King class
    - Still need to implement logic for checks and checkmates etc.

 - **4/22/20** Included embedded server library in the project
   - Will have to include via CMake.
   
 - **4/24/20** Implemented Game graphics 
   - Must find better images since current ones are low resolution
   - Need to find a way to add buttons to the UI
   - Added sound for an illegal move

 - **4/25/20** Switched server side to Sqlite 
    - Need to find a way to concurrently read and write from the database. 

 - **4/28/20** Switched server again to REST API
    - Using CURL (included through CMake) to make web requests
    - For now, the server is a simple Flask HTTP app hosted on a local Apache
     server.
     
 - **4/30/20** Game Logic
    - Added in logic to evaluate the board state as a white win, black win
    , draw or game in progress.
    - Implemented methods to check all legal moves for each player's king
     and check whether or not the king is in check.

 - **4/31/20** Supporting multiple players
    - Using CURL (included through CMake) to make web requests
    - Each player's move communicates with the server through a POST request.
    - Whenever update() is called in the cinder app, the player's client
     receives an update from the server with the last move played.
    - Using nlohmann/json to pr
 
 - **5/01/20** Game Logic
    - Added in logic to evaluate the board state as a white win, black win
    , draw or game in progress.
    - Implemented methods to check all legal moves for each player's king
     and check whether or not the king is in check.

 - **5/02/20** Bug Fixing 
    - Fixed bugs in the game logic with checks and game state and wrote tests
     to test the changes.
     
 - **5/03/20** Graphics 
    - Implementing ending screen to notify the players when the game is over. 

 - **5/04/20** Multiplayer logic fixes 
    - Prevented players from playing twice in a row.
    - Reduced the number of times the client gets an update.
    - Parsing command line arguments with gflags.
 
 - **5/05/20** Testing and ReadMe
    - Wrote more tests for the game logic.
    - Updated README.md with dependencies and installation instructions.
    - Fixed a couple bugs in the game logic.
  

