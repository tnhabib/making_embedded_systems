# Gyro Simon State Documentation
# Overview

For the final project , I will be implementing a motion simon says game that uses the Gyro on the STMF4 Discovery Board. 

Peripherals used:
 * LCD Display - to draw and display sequences to repeat, display score
 * Button - to start the game
 * Gyro Sensor (I3G4250D) to capture input (lean left, lean right, lean up, lean down) to match sequence as it is presented and grows
 * LED Matrix - if time permits this will replace the LED Display
 
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
