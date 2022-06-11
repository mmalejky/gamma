## Demo of interactive mode
![gamma-preview](https://user-images.githubusercontent.com/104734237/173177543-611e59dc-597b-40cb-bcfb-4404e28de0e4.gif)

## Game description
The game is parameterized by four values: ```width```, ```height```, ```players``` and ```max_areas```.

The game is played on a rectangular (dimensions of ```width``` by ```height```) game board that consists of square fields. Adjacent squares on the board are those that meet on the sides. Fields touching only their corners are not considered adjacent. A set of squares make an area if from each square you can reach another square by passing through adjacent squares only. A single field is also an area. The game can be played by one or more ```players```. At the beginning of the game the board is empty. The players take turns occupying one field per round, placing their pawn on it. A player may occupy any unoccupied field, provided that the following rule is met: 
- a set of fields occupied by the same player may not, in any phase of the game, consist of more than ```max_areas``` areas.

Each player may make a golden move once during the whole game, which consists of taking another player's pawn from the board and putting his own pawn in its place, but this move still may not violate the rule of the maximum number of occupied areas by any player. A player who is unable to make a move according to the above rule is out of the game, but may return to the game after some golden move by another player. The game ends when no more players can make a move. The player who occupies the highest number of squares wins.

## Build instructions
- To make the game, run
```
mkdir build && cd build
cmake ../src
make
```
- To make test of game engine, run ```make testing```
- To make Doxygen documentation, run ```make doc```

## Usage modes
- Interactive mode (type ```I <width> <height> <players> <max_areas>```)\
The game is controlled using your keyboard (via [ANSI escape codes](https://en.wikipedia.org/wiki/ANSI_escape_code)). Possible actions include:
    - Arrow keys to move cursor
    - Press ```SPACE``` to occupy the field
    - Press ```G``` to make a golden move
    - Press ```C``` to skip your turn
    - Press ```CTRL+D``` to end the game early

- Batch mode (aka debug mode) (type ```B <width> <height> <players> <max_areas>```)\
The game is controlled by typing commands. Result of every command is printed to _stdio_.\
Truth is represented by ```1```, false by ```0```. Possible actions include:
    - ```m player x y``` – tries to perform a move by specified player on field with coordinates (x, y)
    - ```g player x y``` – tries to perform a golden move by specified player on field with coordinates (x, y)
    - ```b player``` – prints the number of fields taken by specified player
    - ```f player``` – prints the number of fields that specified player can obtain
    - ```q player``` – checks, whether specified player can make a golden move
    - ```p``` – prints the board
    - ```# comment``` - comments are ignored
