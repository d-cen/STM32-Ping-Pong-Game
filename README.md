# ECE 36200 Project: Ping Pong Game

## Description
In this project, a pong game was built using STM32F051R8T6, and displayed on ILI9341. The game was designed for two players, each player uses 2 push buttons to control the paddle's direction to bounce off the ball. There are 3 different level of difficulty to choose, and the level of difficulty was measured by the velocity of the ball. The win condition is to be the first to score three points, and the scores are displayed on the CFAL1602 OLED. After game ends, music is played. The game can be restart after pressing pushbutton.

## Components
|Component Name                             |Count  |
| :---                                      | :---: |
|STM32F051R8T6                              |1      |
|ILI9341 LCD (2.8" TFT LCD with Touchscreen)|1      |
|CFAL1602 OLED                              |1      |
|LM324 IC                                   |1      |
|Speaker                                    |1      |
|1k Ohms resistors                          |4      |
|Pushbutton                                 |4      |

## Testing
Only source files are provided here. Remember to include "StdPeriph_Driver" when running.
