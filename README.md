# casa0014-Prototype
casa0014 prototype
A project that can control the color and intensity of the  vespera lights by sensing the ambient temperature and humidity.

![0078e2543ebfee54117e46ed2b12c8a](https://github.com/user-attachments/assets/b5e80746-81c6-4134-b61d-25d1b9fedf74)

At first, my intention was merely to use the temperature sensor to control the color of the lights. However, after discussing my design ideas with the teacher, he informed me that there is a sensor called DHT22 which can simultaneously measure temperature and humidity. So, I decided to use this sensor and add the function of controlling the color depth of the lights based on the humidity level.

So I began to learn about the DHT22 sensor and how to apply it in the code for Arduino. 

<img width="669" height="76" alt="image" src="https://github.com/user-attachments/assets/5265f6d0-c5e3-47bd-9d8a-8410171db7bf" />
This is the code for obtaining the temperature and humidity values of the environment through sensors.

After measuring the classroom temperature and humidity, I set the color-changing temperature range of the lights at 25-30 degrees, and the humidity range at 50-100%. As the temperature rises within this range, the color changes from the cool blue tone to the warm red tone. As the humidity increases within the range, the color changes from light to dark.

After conducting a series of tests, I found that the color changes were too frequent. Therefore, I decided to add a switch to control whether the color would change or not. When the switch is turned on, the color changes normally according to the environmental temperature and humidity. When the switch is turned off, the color remains fixed.
