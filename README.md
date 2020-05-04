# Multiplayer Chess Game
A chess game playable between two players over an `http` server.

## Installation
Clone the project from Github: `git clone https://github.com/CS126SP20/final
-project-andreakroy.git`.

Build the project with CMake using the command `cmake -S $path_to_project -B
 $path_to_binary`.
 
## Running the App
 Run the app with the command `./$path_to_binary --url=$server_url --color
 =$color --game_id=$id`.
 
 For a single player game, do not supply the url argument. The color argument
  will determine from which point of view the game is played.
  For a multiplayer game, follow the steps below for running the web server
  . Both players should run the game using the url of the web server, the
   color they'd like to play, and the same game_id. The game continues until
    there is a draw or a checkmate. Support for resigning/offering draws
     should be implemented in the near future.
   
   When the app runs, there will be a traditional chess board on the screen
   . To move a piece, click the square holding the piece you would like to
    move. The square will turn yellow indicating that the piece is selected
    . Then selet the destination move. If the move is successful, the move
     will be made.

##Dependencies
- `Cinder 0.9.2:` https://github.com/cinder/Cinder/releases
- `libcurl 7.7.0:` https://curl.haxx.se/libcurl/
- `nlohmann/json 3.7.3:` https://github.com/nlohmann/json
- `OpenGL 3.2:` https://www.opengl.org/

##Running the Server
To run the server for a multiplayer game, host the flask app in
 `assets` folder on a local server (an Apache server for instance). For a
  tutorial, see https://www.digitalocean.com/community/tutorials/how-to-deploy-a-flask-application-on-an-ubuntu-vps.
  
  <b> The flask app included is not meant to be used in a production
   environment. </b> It is merely to be used to play a mutiplayer game
    locally, and as an example of how the chess game might interface with a
     production server.