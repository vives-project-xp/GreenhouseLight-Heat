// WLED hostname
const wledHostname = "http://lightandheat.local/json/state";

// Configuration
const maxLux = 4500; // Maximum lux provided by the LED strip
const targetLux = 25000; // Desired lux for the plants

// Function to set brightness
async function setBrightness(brightness) {
    const data = {
        "bri": Math.min(Math.max(brightness, 0), 255), // Clamp brightness between 0 and 255
        "on": brightness > 0  // Automatically turn on if brightness is above 0
    };
    await sendWLEDRequest(data);
}

// Generic function to send JSON data to WLED
async function sendWLEDRequest(data) {
    try {
        const response = await fetch(wledHostname, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(data)
        });

        if (response.ok) {
            console.log("Command sent successfully:", data);
        } else {
            console.error("Error sending command:", response.status);
        }
    } catch (error) {
        console.error("Failed to send command:", error);
    }
}

// Function to calculate and adjust brightness based on sensor lux
async function adjustBrightness(sensorLux) {
    console.log("Sensor Lux:", sensorLux);

    // Calculate the needed lux from LEDs
    const neededLux = targetLux - sensorLux;

    // If no additional light is needed, turn off LEDs
    if (neededLux <= 0) {
        console.log("Sufficient light detected. Turning off LEDs.");
        await setBrightness(0);
        return;
    }

    // Calculate the brightness percentage required
    const brightnessPercentage = neededLux / maxLux;

    // Convert to WLED brightness level (0-255)
    const brightnessLevel = Math.round(brightnessPercentage * 255);

    console.log(`Needed Lux: ${neededLux}, Adjusting Brightness to: ${brightnessLevel}`);
    await setBrightness(brightnessLevel);
}

// Example usage with a light sensor reading
async function main() {
    const sensorLux = await getLightSensorValue(); // Replace with actual sensor function
    await adjustBrightness(sensorLux);
}

// Dummy function to simulate light sensor reading
async function getLightSensorValue() {
    // Replace this with actual code to read the sensor value
    return 2000; // Example: 15,000 lux detected
}

// Run the main function
main();
