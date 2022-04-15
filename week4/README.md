# Gyro Simon State Documentation

## State Diagram
![your-UML-diagram-name](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/tnhabib/making_embedded_systems/master/week4/gyroStateDiag.iuml)

## State Table
| State                    | Action                                                                                                       | Next State    | Button     | Gyro                                                                | Timeout   |
|--------------------------|--------------------------------------------------------------------------------------------------------------|---------------|------------|---------------------------------------------------------------------|-----------|
| START                    | Show Title                                                                                                   |               | START_GAME | X                                                                   | X         |
| START_GAME               | Reset Sequence<br>Select First Element                                                                       | PLAY_SEQUENCE | X          | X                                                                   | X         |
| INCREASE_SEQUENCE_SIZE   | inc sequence<br>select next element                                                                          | PLAY_SEQUENCE | X          | X                                                                   | X         |
| INCREMENT_SEQUENCE_INDEX | Incement sequence index<br>If (Index == (SequenceEnd +1) -> INCREMENT_SEQUENCE_SIZE<br>Else Go to WAIT State |               | X          | X                                                                   | X         |
| PLAY_SEQUENCE            | Play Sequence                                                                                                | WAIT          | X          | X                                                                   | X         |
| WAIT                     | Reset Timer                                                                                                  | X             | X          | If (match)-><br>  INCREMENT_SEQUENCE_INDEX<br>else-><br>  GAME_OVER | GAME_OVER |
| GAME OVER                | Display Score<br>Delay<br>Show Title                                                                         | X             | START_GAME | X                                                                   | X         |
