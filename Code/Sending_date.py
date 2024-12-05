async def foo():
    # data = hass.states.sensor.greenhouse_10_10_2_48 
    # temperature = data.attributes["temperature"]
    # BrightnessRecieve = data.attributes["brightness"]
    BrightnessSend = 0
    sensor = random.randint(0, 100)
    
    if sensor < 50:  
        BrightnessSend = 3
    else: BrightnessSend = 254
        
    async with aiohttp.ClientSession() as session:
        async with session.post("http://10.10.2.2/json/state", headers={'Content-Type':'application/json'}, data=json.dumps({"bri": BrightnessSend})) as response:
            response_text = await response.text()
            _LOGGER.warning(f"Sensor value: {sensor}, Brightness sent: {BrightnessSend}, Response status: {response.status}, Response text: {response_text}")