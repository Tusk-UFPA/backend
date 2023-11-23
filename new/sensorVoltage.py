from pyfirmata import Arduino, util, SERVO
from datetime import timedelta
from timezonefinder import TimezoneFinder
import time

# Configuração do Arduino e Pinos
porta = 'COM3'  # Substitua pela porta correta (por exemplo, '/dev/ttyUSB0' no Linux)
board = Arduino(porta)
iterator = util.Iterator(board)
iterator.start()

pin_anal = 1

# Servo Objects
pin = board.get_pin('a:{}:s'.format(pin_anal))


while True:
    val = pin.read()
    
    if val is not None:
        val = val * 46.0
        print(val)
    else:
        print("Nenhum valor lido do pino analógico.")
    
    time.sleep(0.1)