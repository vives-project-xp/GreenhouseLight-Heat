async def send_sensor_data():
    # Genereer willekeurige waarden voor temperatuur en helderheid
    temperature = random.randint(15, 30)  # Bijvoorbeeld 15 tot 30 graden Celsius
    brightness = random.randint(0, 255)  # Helderheid van 0 tot 255
    
    payload = {
        "temperature": temperature,
        "brightness": brightness
    }
    
    async with aiohttp.ClientSession() as session:
        async with session.post("http://10.10.2.2/json/state",
                                headers={'Content-Type': 'application/json'},
                                data=json.dumps(payload)) as response:
            response_text = await response.text()
            _LOGGER.warning(f"Temperature sent: {temperature}, Brightness sent: {brightness}, "
                            f"Response status: {response.status}, Response text: {response_text}")