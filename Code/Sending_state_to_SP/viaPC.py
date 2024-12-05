import socket
import json

# Wi-Fi gegevens
DEVICE_IP = "10.10.2.77"  # Vervang met het IP-adres van je WiZ slimme stekker
UDP_PORT = 38899             # Standaard UDP-poort van WiZ apparaten

def send_command(state):
    """
    Stuurt een commando naar de WiZ slimme stekker.
    :param state: True (aan) of False (uit)
    """
    # Maak de JSON opdracht
    command = {
        "method": "setState",
        "params": {
            "state": state
        }
    }
    
    # JSON naar string
    command_str = json.dumps(command)
    
    # Maak een UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(2)  # Optioneel: timeout voor antwoord
    
    try:
        # Verstuur de UDP-pakket naar de slimme stekker
        sock.sendto(command_str.encode(), (DEVICE_IP, UDP_PORT))
        print(f"Commando verstuurd: {command_str}")
    except Exception as e:
        print(f"Fout bij versturen: {e}")
    finally:
        sock.close()

# Test de functies
if __name__ == "__main__":
    print("Schakel de stekker in...")
    send_command(True)  # Zet aan
    input("Druk op Enter om de stekker uit te schakelen...")
    send_command(False)  # Zet uit
