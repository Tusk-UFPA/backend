import pvlib
from pvlib import pvsystem, location, modelchain
from pyfirmata import Arduino, util, SERVO
import time
import datetime
from datetime import timedelta
from timezonefinder import TimezoneFinder
import pytz


latitude = -1.4739214821232673
longitude = -48.451553336970896
loc = location.Location(-1.4739214821232673, -48.451553336970896)

# Configuração do Arduino e Pinos
porta = 'COM7'  # Substitua pela porta correta (por exemplo, '/dev/ttyUSB0' no Linux)
board = Arduino(porta)
iterator = util.Iterator(board)
iterator.start()

# Pinos Digitais
motoHori = board.get_pin('d:{}:s'.format(9))
motoVerti = board.get_pin('d:{}:s'.format(10))

# Pinos Analógicos (formato 'a:{}:s')
ldrtopR = board.get_pin('a:{}:s'.format(0))
ldrtopL = board.get_pin('a:{}:s'.format(1))
ldrbotR = board.get_pin('a:{}:s'.format(2))
ldrbotL = board.get_pin('a:{}:s'.format(3))
pinTensaoSola = board.get_pin('a:{}:s'.format(4))
pinTensaoEoleco = board.get_pin('a:{}:s'.format(5))

# Limites de ângulos para os motores
angleMotoH = 0
motoHoriLimitHigh = 180
motoHoriLimitLow = 45

angleMotoV = 15
motoVertiLimitHigh = 140
motoVertiLimitLow = 15

# Limites dos Servos
motoHori_limit_high = 180
motoHori_limit_low = 0
motoVerti_limit_high = 140
motoVerti_limit_low = 25

# Função para mover os servos
def move_servo(servo, angle, limit_low, limit_high):
    angle = max(limit_low, min(angle, limit_high))
    servo.write(angle)
    time.sleep(0.05)

def trackingVerti(tol,avt,avb,dverti,angleMotoV,motoVertiLimitLow,motoVertiLimitHigh):

    if (-1*tol > dverti or dverti > tol):
        # print("V:", angleMotoV)
        if (avt > avb):
            angleMotoV += 1
        if (angleMotoV > motoVertiLimitHigh):
            angleMotoV = motoVertiLimitHigh

        elif (avt < avb):
            angleMotoV -= 1
        if (angleMotoV < motoVertiLimitLow):
            angleMotoV = motoVertiLimitLow
            
        motoVerti.write(angleMotoV)

def trackingHori(tol,avl,avr,dhori,angleMotoH,motoHoriLimitLow,motoHoriLimitHigh):

    if (-1*tol > dhori or dhori > tol):
        # print("H:", angleMotoH)
        if(avl < avr):
            angleMotoH += 1
        if(angleMotoH > motoHoriLimitHigh):
            angleMotoH = motoHoriLimitHigh
        
        elif (avl > avr):
            angleMotoH -= 1
        if(angleMotoH < motoHoriLimitLow):
            angleMotoH = motoHoriLimitLow
        
        motoHori.write(angleMotoH)

def horaAtual():
    # Calcule a data e hora atual em UTC
    time_now_utc = datetime.datetime.utcnow()

    # Crie uma instância de TimezoneFinder
    tf = TimezoneFinder()

    # Obtenha o fuso horário com base nas coordenadas
    timezone_str = tf.timezone_at(lng=longitude, lat=latitude)

    if timezone_str:
        local_timezone = pytz.timezone(timezone_str)

        # Obtenha a hora atual no fuso horário local
        local_time = datetime.datetime.now(local_timezone)

        # Obtenha o valor da diferença em horas entre o fuso horário local e o GMT/UTC
        gmt_offset = local_time.utcoffset()
        #print("Fuso horário local:", timezone_str)
        hours_dif, minutes_dif = divmod(gmt_offset.total_seconds(), 3600)
        #print(f"A diferença entre {timezone_str} e GMT é {hours_dif:.0f} horas e {minutes_dif // 60} minutos.")
    else:
        print("Fuso horário não encontrado para as coordenadas especificadas.")

    # Calcule a data e hora atual no fuso local
    dif_time = timedelta(hours=hours_dif, minutes=minutes_dif)
    time_now = time_now_utc + dif_time

    return time_now

def posSol():
    time_now = horaAtual()
    solar_position = pvlib.solarposition.get_solarposition(time_now, latitude, longitude)
    altitude = solar_position['apparent_elevation'].values[0]
    azimuth = solar_position['azimuth'].values[0]

    motoVerti.write(altitude)
    motoHori.write(azimuth)

def lerTensao():
    val = pinTensao.read()
    
    if val is not None:
        val = val * 46.0
        print(val)
    else:
        print("Nenhum valor lido do pino analógico.")
    
    time.sleep(0.1)
    
    return val


try:
    # Move os servos para as posições iniciais
    for angle in range(0, 46):
        move_servo(motoHori, angle, 0, motoHori_limit_high)

    for angle in range(15, 141):
        move_servo(motoVerti, angle, motoVerti_limit_low, motoVerti_limit_high)

    # Leitura dos Sensores e Controle dos Servos
    while True:
        # Lê os valores analógicos dos sensores LDR
        topR = ldrtopR.read() * 1000
        topL = ldrtopL.read() * 1000
        botR = ldrbotR.read() * 1000
        botL = ldrbotL.read() * 1000
        # print(f'\n')
        # print('rt: ',topR)
        # print('lt: ',topL)
        # print('rb: ',botR)
        # print('lb: ',botL)

        avt = (topL + topR) / 2
        avb = (botL + botR) / 2
        avl = (topL + botL) / 2
        avr = (topR + botR) / 2
        dverti = avt - avb
        dhori = avl - avr
        
        print(dverti)
        print(dhori)

        tol = 50
        sombra = 50

        # Adapta a lógica de controle dos servos com base nos valores dos sensores
        if (dverti < sombra and dhori < sombra):
            print('Sombra!')
            posSol()
            time.sleep(5)
        else:
            print('Sem Sombra!')
            trackingVerti(tol,avt,avb,dverti,angleMotoV,motoVertiLimitLow,motoVertiLimitHigh)
            time.sleep(0.1)
            trackingHori(tol,avl,avr,dhori,angleMotoH,motoHoriLimitLow,motoHoriLimitHigh)
            time.sleep(0.1)  # Ajuste conforme necessário

        try:
            tensaoSola = pinTensaoSola.read()
            tensaoEoleco = pinTensaoEoleco.read()
        
            if tensaoSola is not None:
                tensaoSola = tensaoSola * 46.0
                print(tensaoSola)
            else:
                print("Nenhum valor lido do pino analógico.")
            
            time.sleep(0.1)

            if tensaoEoleco is not None:
                tensaoEoleco = tensaoEoleco * 46.0
                print(tensaoEoleco)
            else:
                print("Nenhum valor lido do pino analógico.")
            
            time.sleep(0.1)
        except KeyboardInterrupt:
            pass

except KeyboardInterrupt:
    pass

finally:
    # Certifique-se de liberar os recursos do Arduino ao finalizar
    board.exit()