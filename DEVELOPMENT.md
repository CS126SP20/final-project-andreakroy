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
    - For now, the server is a simple Flask HTTP server to debug.

  

