import requests

# WLED hostname
WLED_HOSTNAME = "http://lightandheat.local/json/state"

# Configuration
MAX_LUX = 4500  # Maximum lux provided by the LED strip
TARGET_LUX = 25000  # Desired lux for the plants

def set_brightness(brightness):
    """
    Set the brightness of the WLED.
    """
    brightness = max(0, min(brightness, 255))  # Clamp brightness between 0 and 255
    data = {
        "bri": brightness,
        "on": brightness > 0  # Automatically turn on if brightness > 0
    }
    
    try:
        response = requests.post(
            WLED_HOSTNAME,
            json=data,
            headers={"Content-Type": "application/json"}
        )
        if response.ok:
            print(f"Command sent successfully: {data}")
        else:
            print(f"Error sending command: {response.status_code}, {response.text}")
    except requests.RequestException as e:
        print(f"Failed to send command: {e}")

def adjust_brightness(sensor_lux):
    """
    Adjust the WLED brightness based on sensor lux.
    """
    print(f"Sensor Lux: {sensor_lux}")
    
    # Calculate the needed lux from LEDs
    needed_lux = TARGET_LUX - sensor_lux
    
    if needed_lux <= 0:
        print("Sufficient light detected. Turning off LEDs.")
        set_brightness(0)
        return
    
    # Calculate brightness percentage
    brightness_percentage = needed_lux / MAX_LUX
    
    # Convert to WLED brightness level (0-255)
    brightness_level = round(brightness_percentage * 255)
    
    print(f"Needed Lux: {needed_lux}, Adjusting Brightness to: {brightness_level}")
    set_brightness(brightness_level)

def get_light_sensor_value():
    """
    Simulate reading a light sensor value.
    Replace this function with actual sensor integration.
    """
    # Example: Simulated sensor reading
    return 20000

def main():
    """
    Main function to adjust WLED brightness based on light sensor input.
    """
    sensor_lux = get_light_sensor_value()
    adjust_brightness(sensor_lux)

if __name__ == "__main__":
    main()
