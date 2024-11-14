// WLED hostname
const wledHostname = "http://lightandheat.local/json/state";

// Function to set brightness
async function setBrightness(brightness) {
    const data = {
        "bri": brightness,    // Set brightness (0-255)
        "on": brightness > 0  // Automatically turn on if brightness is above 0
    };
    await sendWLEDRequest(data);
}

// Function to turn lights on or off
async function toggleLights(state) {
    const data = {
        "on": state          // true to turn on, false to turn off
    };
    await sendWLEDRequest(data);
}

// Function to set the color
async function setColor(color) {
    const data = {
        "seg":[{"col":[color]}]         // Set color "col": [[128, 0, 128]]
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

// Example usage:
setBrightness(10);
toggleLights(true);
setColor([255,0,255]); 