import tkinter as tk
import asyncio
import struct

from bleak import BleakClient
from bleak.exc import BleakDeviceNotFoundError

address = "C8:85:42:5E:B4:BA"
ACCELEROMETER_UUID = "00001524-1112-efde-1523-785feabcd123"
GYROSCOPE_UUID = "00001525-1112-efde-1523-785feabcd123"
TEMPERATURE_UUID = "00001526-1112-efde-1523-785feabcd123"
STEPS_UUID = "00001527-1112-efde-1523-785feabcd123"

class Sensor:
    def __init__(self):
        self.accelerometer_data = [0, 0, 0]
        self.gyroscope_data = [0, 0, 0]
        self.temperature_data = 0
        self.steps_data = 0
        self.connection_length = 0.4

        self.root = tk.Tk()
        self.root.title('Sensor GUI')
        self.root.geometry("850x500")
        self.root.configure(bg='snow2')
        self.data_screen()
        self.root.mainloop(100)

    async def refresh_mainloop(self, n):
        self.root.mainloop(n)
        # await asyncio.sleep(0)

    async def update_screen(self):
        self.root.update()
        accelerometer_string = f"Acceleration [mm/s^2]: X = {'{:.2f}'.format(self.accelerometer_data[0])}, Y = {'{:.2f}'.format(self.accelerometer_data[1])}, Z = {'{:.2f}'.format(self.accelerometer_data[2])}"
        gyroscope_string = f"Pitch [deg/s]: {'{:.2f}'.format(self.gyroscope_data[0])}, roll: {'{:.2f}'.format(self.gyroscope_data[1])}, yaw: {'{:.2f}'.format(self.gyroscope_data[2])}"
        temperature_string = f"Temperature: {'{:.2f}'.format(self.temperature_data)} Celsius degrees"
        steps_data = f"Number of steps: {self.steps_data}"

        self.accelerometer_data_string.set(accelerometer_string)
        self.gyroscope_data_string.set(gyroscope_string)
        self.temperature_data_string.set(temperature_string)
        self.steps_data_string.set(steps_data)
        # await asyncio.sleep(0)

    def data_screen(self):
        self.data_frame = tk.Frame(self.root, bg='snow2')
        self.accelerometer_data_string = tk.StringVar()
        self.gyroscope_data_string = tk.StringVar()
        self.temperature_data_string = tk.StringVar()
        self.steps_data_string = tk.StringVar()

        header = tk.Label(self.data_frame, text='Pedometer', font=('Comic Sans MS', 30), fg='red', activebackground='blue', bg='snow2').grid(row=0, column=0, ipadx=200, ipady=20)
        label_accelerometer = tk.Label(self.data_frame, textvariable=self.accelerometer_data_string, font=('Comic Sans MS', 20), fg="blue", bg='snow2').grid(row=1, column=0, ipadx=20, ipady=20)
        label_gyroscope = tk.Label(self.data_frame, textvariable=self.gyroscope_data_string, font=('Comic Sans MS', 20), fg="blue", bg='snow2').grid(row=2, column=0, ipadx=20, ipady=20)
        label_temperature = tk.Label(self.data_frame, textvariable=self.temperature_data_string, font=('Comic Sans MS', 20), fg="blue", bg='snow2').grid(row=3, column=0, ipadx=20, ipady=20)
        label_steps = tk.Label(self.data_frame, textvariable=self.steps_data_string, font=('Comic Sans MS', 20), fg="blue", bg='snow2').grid(row=4, column=0, ipadx=20, ipady=20)

        self.data_frame.pack()


    def notification_handler(self, sender, data):
        if sender.uuid == ACCELEROMETER_UUID:
            self.accelerometer_data = [float(struct.unpack('<f', data[:4])[0]), float(struct.unpack('<f', data[4:8])[0]), float(struct.unpack('<f', data[8:])[0])]
        if sender.uuid == GYROSCOPE_UUID:
            self.gyroscope_data = [float(struct.unpack('<f', data[:4])[0]), float(struct.unpack('<f', data[4:8])[0]), float(struct.unpack('<f', data[8:])[0])]
        if sender.uuid == TEMPERATURE_UUID:
            self.temperature_data = float(struct.unpack('<f', data)[0])
        if sender.uuid == STEPS_UUID:
            self.steps_data = int.from_bytes(data, byteorder='big')

    async def read_accelerometer_data(self, client):
        await client.start_notify(ACCELEROMETER_UUID, self.notification_handler)
        await asyncio.sleep(self.connection_length)

    async def read_gyroscope_data(self, client):
        await client.start_notify(GYROSCOPE_UUID, self.notification_handler)
        await asyncio.sleep(self.connection_length)

    async def read_temperature_data(self, client):
        await client.start_notify(TEMPERATURE_UUID, self.notification_handler)
        await asyncio.sleep(self.connection_length)

    async def read_steps_data(self, client):
        await client.start_notify(STEPS_UUID, self.notification_handler)
        await asyncio.sleep(self.connection_length)


async def collect_data(sensor):
    try:
        async with BleakClient(address) as client:
            while True:
                await asyncio.gather(sensor.read_accelerometer_data(client), sensor.read_gyroscope_data(client), sensor.read_temperature_data(client), sensor.read_steps_data(client), refresh_screen(sensor))
                await asyncio.sleep(0)
    except (RuntimeError, BleakDeviceNotFoundError): asyncio.run(collect_data(sensor))

async def refresh_screen(sensor):
    await asyncio.gather(sensor.update_screen(), sensor.refresh_mainloop(100))

def main():
    s = Sensor()
    asyncio.run(collect_data(s))

main()