# Heater guide

## Requirements

1. **Smart Plug**: we used the [Wiz smart plug](https://www.hubo.be/nl/p/wiz-smart-plug-stekker/943560/?srsltid=AfmBOop1jMLLTrlQRWXBZI4sboYYit9EBC_hZCbBdXXBF2tYZz2Lpcub) which is easy to install with a simple guide that comes with the product.
2. **Heater**: we used a [Vonroc heater](https://www.brico.be/nl/tuin-terras-buitenleven/terrasverwarming/terrasverwarmers-accessoires/terrasverwarmers/vonroc-heater-marsili-compact-2000w-zwart/10138731#specs) because it has a IP55 which is good for outdoor use.
3. **ESP-32**: this is to control the smart plug.

## Step 1: Set up the smart plug

There is an easy installation guide that comes with the product itself. Just plug the smart plug in an outlet and open the app, then click on the plus icon to connect a device, choose for a smart plug. Then you can choose to set it up manually by clicking on the 3 stripes on the rop left corner. Set up the wifi and complete the guide.

## Step 2: Connect the heater to the smart plug

When the smart plug is set up, you can connect the heater to the smart plug. Just plug the heater in and turn it on. If there are anny settings on you heater, you can set them up now to what you think is best. Now you can already control the smart plug and so also the heater from your phone.

## Step 3: Code for ESP-32 to control the smart plug

Via the ESP-32 you can control the smart plug. This is done by sending a HTTP request to the smart plug. [Here](../../Code/Sending_state_to_SP/ESP_To_SmartPlug/ESP_To_SmartPlug.ino) is the code we used to connect and control the smart plug.

