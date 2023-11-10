# Fire_System

https://github.com/gihadmecha/Fire_System/assets/19871755/9dd0edd9-05c1-49cf-be58-79f162afbc46

to measure the environment's temperature and detect smoke levels, enabling the timely activation of water sprinklers and alarms in response to a potential fire hazard.

Demo: https://youtu.be/VXHsIy-3uFs?si=8t_whU7VJDfkJ1_V

## How
It gets the environment's temperature using a Temperature Sensor, and the smoke level using a Smoke Sensor and puts them on the Display Screen.
- Normal State:
  - If temp < 45 °C
- Heat State:
  - If temp > 45 °C
  - The yellow light is on
- Fire State:
  - If temp > 45 °C and smoke level > 50
  - The Red light is on
  - Sprinkler sprays water

    
You can't exit Fire State until you fix the issue and enter the confirmation password.

## Hardware
- atmega32
- LM35
- MQ-2
- Stepper Motor
- ln298
- Keypad 4x4
- LCD 16x2
- Leds
- Resistors

## Peripherals
- ADC
- DIO

## Make sure you have installed
- [ATMEG32 MCAL layer](https://github.com/gihadmecha/Embedded_Systems/tree/main/atmega32_ECU/atmega32_ECU/MCAL).
- [HAL layer](https://github.com/gihadmecha/Embedded_Systems/tree/main/atmega32_ECU/atmega32_ECU/HAL).
- [Defined Data Types](https://github.com/gihadmecha/Embedded_Systems/blob/main/atmega32_ECU/atmega32_ECU/StdTypes.h).
- [atmega32 memory](https://github.com/gihadmecha/Embedded_Systems/blob/main/atmega32_ECU/atmega32_ECU/MemMap.h).
- [utilities](https://github.com/gihadmecha/Embedded_Systems/blob/main/atmega32_ECU/atmega32_ECU/UTILS.h).

## Softwares you should install
- Microchip Studio for AVR
- Proteus
