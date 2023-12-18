#!/bin/env python3

import asyncio
from websockets.server import serve

async def echo(websocket):
    print("Connected")
    async for message in websocket:
        await websocket.send(message)

async def main():
    async with serve(echo, "localhost", 12345):
        await asyncio.Future()  # run forever

asyncio.run(main())
