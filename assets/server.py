from flask import Flask, jsonify, request
app = Flask(__name__)

#Dictionary mapping a game_id onto the last move in that game
moves = {}

class move:
    def __init__(self, move_str, number, color):
        self.move = move_str
        self.number = number
        self.color = color
    
    def __str__(self):
        return jsonify(
                move=self.move,
                number=self.number,
                color=self.color)

@app.route('/moves', methods=['GET', 'POST'])
def update():
    if 'game_id' not in request.args:
            return("no game id provided", 400)
    #GET
    if request.method == 'GET':
        if request.args['game_id'] not in moves:
            return("game not on server", 400)
        return (moves[request.args['game_id']].__str__(), 200)
    #POST
    elif request.method == 'POST':
        if 'color' not in request.args:
            return("no color provided", 400)
        elif 'number' not in request.args:
            return("no move number provided", 400)
        elif 'move' not in request.args:
            return("no move string provided", 400)
        elif len(request.args['move']) != 4:
            return("invalid move string", 400)
        elif request.args['game_id'] not in moves:
            if request.args['color'] != 'white':
                return("the first move of a game must be played by white", 400)
            elif request.args['number'] != "1":
                return("move number must be 1", 400)
            else:
                moves[request.args['game_id']] = move(request.args['move'],
                        int(request.args['number']), "white")
                return("success", 200)
        else:
            last = moves[request.args['game_id']]
            if request.args['color'] == last.color:
                return("can't move the same color twice", 400)
            elif request.args['color'] == "white" \
                and int(request.args['number']) != last.number + 1:
                return("invalid move number", 400)
            elif request.args['color'] == "black" \
                and int(request.args['number']) != last.number:
                return("invalid move number", 400)
            else:
                moves[request.args['game_id']] = move(request.args['move'],
                        int(request.args['number']), request.args['color'])
                return("success", 200)


if __name__ == '__main__':
    app.run()

