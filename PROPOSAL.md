# Chess Game

Author: Andrea Roy

---
I will create a chess game. The game will support two players playing over a web server.

## Description and Minimum Viable Product
To make web HTTP requests, I plan to use the CURL library. The library will be integrated via CMake. Some I plan to set up a simple REST API to allow me to GET and POST moves to the server. I will debug and run the game on a local server hosted on my machine. I have some good experience working with REST APIs, including building a local server in Python for a smart home project, which is why I chose to add a server to my game.

The front end for the game will be built using the Cinder graphics library. The front end will be very simple, with just a board and pieces. A player will move a piece by clicking on the piece and then selecting a new square. When a legal move is made, a POST request will be sent with the move, and other player's client will be updated. I have no experience with Cinder graphics other than the snake game, so I anticipate that this will be the most difficult part for me.

My chess game back end will account for the rules of a chess game, including legal piece moves, checkmates, stalemates, etc. I designed (a very poor) chess game in Java 4 years ago which didn't really work, so I have some experience coding chess rules. I am a decent chess player and am consequently very interested in the game and this project.

## Timeline 
Since I have 3 weeks to work on the project, I plan to break down my work as follows:
- <b>Week 1:</b> Build the backend. Create classes for the game, pieces, etc. By the end of the week, I'd like to have a working chess game that I can play in my terminal.
- <b>Week 2:</b> Build the web server and start on the graphics. By the end of the week, I hope to have a web server up and running, and I should be able to have two different connections to the web server play a complete game through the terminal. If I have time, I want to start on the graphics because I anticipate that that will be the hardest part for me.
- <b>Week 3:</b> Graphics. By the end of the week I should have the front end built and working. Moving a piece should send a POST request to the server, and when the other player moves, it should update the board.

## Extra Features
In the future I'd like to implement the following:
- <b>Resign:</b> Add a button to the front end allowing a player to resign the game.
- <b>Timer:</b> Implement a timer into the front end. When a timer expires, it should update the server.
- <b>Draw Offer:</b> Implement a draw offer button. When one player offers a draw, the other player's client should notify them that a draw has been offered and give them the option to accept.
