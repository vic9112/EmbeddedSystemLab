import asyncio
from bleak import BleakScanner, BleakClient
import time

uuid_clock_service = '0000b240-0000-1000-8000-00805f9b34fb'
uuid_x_characteristic = '0000b241-0000-1000-8000-00805f9b34fb'
uuid_y_characteristic = '0000b242-0000-1000-8000-00805f9b34fb'
uuid_fb_characteristic = '0000b243-0000-1000-8000-00805f9b34fb'
uuid_ac_characteristic = '0000b244-0000-1000-8000-00805f9b34fb'

async def main():
    devices = await BleakScanner.discover()
    device_dict = {}
    for d in devices:
        if d.name != "Unknown" and d.name is not None:
            if(d.name=="VicServer24"):
                my_device=d
            device_dict[d.name] = d
    print(device_dict)
    distance = 0
    cal_dis = 0
    address=my_device.address
    async with BleakClient(address) as client:
        svcs = await client.get_services()
        while True:
            x_byte = await client.read_gatt_char(uuid_x_characteristic, response=False)
            y_byte = await client.read_gatt_char(uuid_y_characteristic, response=False)
            fb_byte = await client.read_gatt_char(uuid_fb_characteristic, response=False)
            ac_byte = await client.read_gatt_char(uuid_ac_characteristic, response=False)
            x=int.from_bytes(x_byte)
            y=int.from_bytes(y_byte)
            fb=int.from_bytes(fb_byte)
            ac=int.from_bytes(ac_byte)
            if (fb > 240):
                distance += fb
            if (ac > 240):
                cal_dis += ac
            print(f"Distance: {distance + fb}, x: {x/100}, y: {y/100}, cal_dis: {cal_dis + ac}\n")
            time.sleep(1)

asyncio.run(main())