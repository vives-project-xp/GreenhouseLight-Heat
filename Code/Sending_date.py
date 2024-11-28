async def foo():
    # data = hass.states.sensor.greenhouse_10_10_2_48
    # temperature = data.attributes["temperature"]
    # Brightness = data.attributes["brightness"]
    Brightness = 100
    
    #send data to an esp32
    import aiohttp
    import json
    url = "http://10.10.2.2/json/state"
    data = {"bri": Brightness}
    
    headers = {
        'Content-Type': 'application/json'
    }
    
    async with aiohttp.ClientSession() as session:
        async with session.post(url, headers=headers, data=json.dumps(data)) as response:
            response_text = await response.text()
            print(response_text)