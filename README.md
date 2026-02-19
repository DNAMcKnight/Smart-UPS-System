# Automated UPS Shutdown System Using ESP8266

This project uses an ESP8266 to automatically shut down a PC and UPS during a power outage.

## How It Works

This project uses an ESP8266 NodeMCU as the brains of the operations and a SG90 Servo as the actuator for the button as I don't want to modify the UPS itself. I created a custom sensor using an LDR and a transistor to pull a pin high/low to trigger autonated shutdown. This code for the ESP8266 is written using the platformio ecosystem. There's a seperate fastAPI server that runs on the PC where we can modify the timeout settings and popup timeout settings and how the PC reacts to the overall commands.

## Why Does This Exist?

I recently bought a new computer and a 1500VA UPS, I looked but there weren't any smart UPS available for me to buy and given how my last one was misused I wanted to make sure both the PC and the UPS had an extend life for many years to come. I am the kind of guy who likes to leave my computer on unattended which means I might not be around to quickly shutdown in case of power outage. However the first problem was that this thing has a loud cooling fan built into the UPS which runs literally all the time. The fan unfortunately is louder than the new PC and I have to put it in the other room to mitigate the noise. I have to walk all the way around to turn the UPS ON/OFF each time. This is how I came up with this project. The main goal of the project is to quickly turn off the computer and the UPS automatically, even when unattended. 

This project is locally isolated, meaning you can't access it outside the house, however I think that isolation works well because it's smart enough to take care of the computer all by itself and unless you really need to turn on the computer remotely, nobody needs to be able to access the local ESP. However that can be easily resolved by using a raspberry pi or an old android that has tailscale connected. Just use tailscale to access the local network and turn on the UPS and use a magic packet to wake on lan using the Android/raspberry pi. Given that your motherboard fully supports this feature, mine does not... I can not wake on lan after power cycling. Which technically isn't game over, you'll just have to turn on another feature that turns on the pc the moment it gets power.

## Specifications & Dependencies
### Power Supply

The system is powered by a single 18650 LiPo battery which is being charged through a TP4056 board. This is ideal because it has a protection board builtin. The battery is charged through a standard 5v charger and we're using a mini boost module that  has a variable output that can boost from 3.7v to 5v. This board isn't great at higher voltages but is a perfect fit at 5v which is still lower than the minimum voltage 6v working voltage for the servo but we can get by. 

### Servo Connection

The servo is connected through the same VCC and GND pins, however I have added a couple of filtering capacitor to help reduce servo jittering. The servo ideally needs 6v to function effectively however we're under powering it which can lead to jitter during load. This is one fundamental flaw of this system as of right now. However boosting the voltage hasn't helped so either we need to create a 2s battery pack or need to create two different power supplies which might be overkill.

### Code Structure

All the code is written under `src/main.cpp` file. We are using the D4 pin as the signal pin to move the servo. In the setup function we are running the WiFiManager which is responsible for connecting WiFi once connected we are using the `webserver` function to run the webserver. In the webserver we are calling the `toggle_switch` function which is moving the servo when the button is pressed. As for the code for the webserver on the PC follow the instructions in `pc_server` and just run on startup.

### WebServer Function

This function uses the LittleFS library to load the HTML,CSS and the JS files. Although this is overkill for such small functionality but it's nice to have total control over these bits and pieces however remember to go to `Platformio Extension>Platform>Build Filesystem Image` and then `Platformio Extension>Platform>Upload Filesystem Image` to get these files on to the ESP or LittleFS will complain it can't find the files. Guess how I know!

Note:
The HTML, CSS and the JS files along with any other file you may want to use need to go in the `data` folder for it to work.

### Dependencies

- ESP8266WebServer
- WiFiManager (*external package*)
- ESP8266Ping (*external package*)
- ArduinoJson (*external package*)
- LittleFS
- Servo

## The Master Plan

This might seem like it's a glorified button press but it's not. I want to create a system that will let me do the following.

1. Let me turn the UPS ON/OFF anytime I want.
2. Will be able to detect if the Power is out.
3. If the power is out then it will shutdown the PC and then proceed to turn off the UPS.

So basically this project will be responsible for quickly shutting down the PC safely and then turning the UPS off to save power. This will also allow me to turn the UPS back on when the power comes back (*this can also easily be automated*)

## Limitations

This new UPS has a warranty which means we're not allowed to mod it in any way. This means we can't open it up and hook things up directly, instead we must work in a non destructive way so if we ever need to claim the warranty we can without issue.
