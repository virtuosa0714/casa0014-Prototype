# casa0014-Prototype
casa0014 prototype

This corner of the CASA0014 repository contains 1 project:

Weather Visualizer - A project that can control the color and intensity of the vespera lights by sensing the ambient temperature and humidity.

![0078e2543ebfee54117e46ed2b12c8a](https://github.com/user-attachments/assets/b5e80746-81c6-4134-b61d-25d1b9fedf74)

This repository contains the process of formulating my ideas for this project and the process of making improvements to it.

The weather in the UK is highly variable, so I had an epiphany and decided to create a device to visualize the rapidly changing environmental temperature.

At first, my intention was merely to use the temperature sensor to control the color of the lights. However, after discussing my design ideas with the teacher, he informed me that there is a sensor called DHT22 which can simultaneously measure temperature and humidity. So, I decided to use this sensor and add the function of controlling the color depth of the lights based on the humidity level.

So I began to learn about the DHT22 sensor and how to apply it in the code for Arduino. 

<img width="669" height="76" alt="image" src="https://github.com/user-attachments/assets/5265f6d0-c5e3-47bd-9d8a-8410171db7bf" />

This is the code for obtaining the temperature and humidity values of the environment through DHT22 sensors.

<img width="1352" height="454" alt="image" src="https://github.com/user-attachments/assets/b30fa97b-5f61-444b-9e2d-7f37eaf6758a" />
<img width="1435" height="789" alt="image" src="https://github.com/user-attachments/assets/354d56db-19a3-4ea2-9ef9-b691a7618cca" />

After measuring the classroom temperature and humidity, I set the color-changing temperature range of the lights at 25-30 degrees, and the humidity range at 50-100%. As the temperature rises within this range, the color changes from the cool blue tone to the warm red tone. As the humidity increases within the range, the color changes from light to dark.

After conducting a series of tests, I found that the color changes were too frequent. Therefore, I decided to add a switch to control whether the color would change or not. When the switch is turned on, the color changes normally according to the environmental temperature and humidity. When the switch is turned off, the color remains fixed.

<img width="1016" height="698" alt="image" src="https://github.com/user-attachments/assets/0eab93e2-dc47-4f9a-94f1-21a5774e9ba3" />

This is the circuit diagram after adding the switch. It connects the Arduino board, the DHT22 sensor and the switch.

During the testing phase, I heated up the air by breathing onto the DHT22 sensor to alter the temperature and humidity sensed by the sensor. This caused the temperature and humidity to increase, thereby changing the color of the light. Once I stopped breathing, the temperature and humidity would gradually return to the original environmental parameters of the classroom, and the color would also gradually change.

<img width="1033" height="640" alt="cold and dry" src="https://github.com/user-attachments/assets/e49749c2-b132-45a9-a0ec-e2f00101a7d0" />
<img width="1040" height="659" alt="cold and wet" src="https://github.com/user-attachments/assets/bd427f76-f6ad-4747-8291-646ec7a9f703" />

These two pictures are the comparison before and after the humidity increase. It can be seen that the color of the light has become darker.

<img width="1082" height="644" alt="warm and wet" src="https://github.com/user-attachments/assets/dada4eb3-0e47-4a12-8f01-3838cf3f5f26" />

This is the color of the light after both temperature and humidity have risen simultaneously.
