# Utilities

In Java utilities functions are static methods in DoomGame class.

* [doomTicsToMs](#doomTicsToMs)
* [msToDoomTics](#msToDoomTics)
* [doomTicsToSec](#doomTicsToSec)
* [secToDoomTics](#secToDoomTics)
* [doomFixedToDouble](#doomFixedToDouble)
* [isBinaryButton](#isBinaryButton)
* [isDeltaButton](#isDeltaButton)
* [sleep](#sleep)

---
### <a name="doomTicsToMs"></a> `doomTicsToMs`

| C++    | `double doomTicsToMs(double tics, unsigned int ticrate = 35)` |
| :--    | :--                                                           |
| Lua    | `number doomTicsToMs(number tics, number ticrate = 35)`       |
| Java   | `double doomTicsToMs(double tics, int ticrate = 35)` |
| Python | `float doom_tics_to_ms(float tics, int ticrate = 35)`         |

Changed in 1.1

Calculates how many tics will be made during given number of milliseconds.


---
### <a name="msToDoomTics"></a>`msToDoomTics`

| C++    | `double msToDoomTics(double ms, unsigned int ticrate = 35)` |
| :--    | :--                                                         |
| Lua    | `number msToDoomTics(number ms, number ticrate = 35)`       |
| Java   | `double msToDoomTics(double ms, int ticrate = 35)` |
| Python | `float ms_to_doom_tics(float ms, int ticrate = 35)`         |

Changed in 1.1

Calculates the number of milliseconds that will pass during specified number of tics.


---
### <a name="doomTicsToSec"></a>`doomTicsToSec`

| C++    | `double doomTicsToSec(double tics, unsigned int ticrate = 35)` |
| :--    | :--                                                            |
| Lua    | `number doomTicsToSec(number tics, number ticrate = 35)`       |
| Java   | `double doomTicsToSec(double tics, int ticrate = 35)` |
| Python | `float doom_tics_to_sec(float tics, int ticrate = 35)`         |

Added in 1.1

Calculates how many tics will be made during given number of seconds.


---
### <a name="secToDoomTics"></a>`secToDoomTics`

| C++    | `double secToDoomTics(double sec, unsigned int ticrate = 35)` |
| :--    | :--                                                           |
| Lua    | `number secToDoomTics(number sec, number ticrate = 35)`       |
| Java   | `double secToDoomTics(double sec, int ticrate = 35)` |
| Python | `float sec_to_doom_tics(float sec, int ticrate = 35)`         |

Added in 1.1

Calculates the number of seconds that will pass during specified number of tics.


---
### <a name="doomFixedToDouble"></a>`doomFixedToDouble`

| C++    | `double doomFixedToDouble(int / double doomFixed)`  |
| :--    | :--                                                 |
| Lua    | `number doomFixedToDouble(number doomFixed)`        |
| Java   | `double doomFixedToDouble(int / double doomFixed)`  |
| Python | `float doom_fixed_to_double(int / float doomFixed)` |

Converts Doom's fixed point numeral to a floating point value.

See also: 
- Types: `User variables` in `GameVariables`

Aliases (added in 1.1):

| Lua    | `number doomFixedToNumber(number doomFixed)`       |
| :--    | :--                                                |
| Python | `float doom_fixed_to_float(int / float doomFixed)` |


---
### <a name="isBinaryButton"></a>`isBinaryButton`

| C++    | `bool isBinaryButton(Button button)`    |
| :--    | :--                                     |
| Lua    | `boolean isBinaryButton(Button button)` |
| Java   | `boolean isBinaryButton(Button button)` |
| Python | `bool is_binary_button(Button button)`  |

Returns true if button is binary button.


---
### <a name="isDeltaButton"></a>`isDeltaButton`

| C++    | `bool isDeltaButton(Button button)`    |
| :--    | :--                                    |
| Lua    | `boolean isDeltaButton(Button button)` |
| Java   | `boolean isDeltaButton(Button button)` |
| Python | `bool is_delta_button(Button button)`  |

Returns true if button is delta button.


---
### <a name="sleep"></a> `sleep`

| Lua    | `void sleep(number sleepTime)` |
| :--    | :--                            |

Pause thread for specified number of milliseconds (without busy waiting). 
Added because of the absence in stock Lua.
