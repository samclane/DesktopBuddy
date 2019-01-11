import atexit
import configparser
import logging
import os
import sched
import sys
import threading
import time
from shutil import copyfile

import discord
import serial
import serial.tools.list_ports
from pyautogui import hotkey

import SysTrayIcon

REFRESH_RATE = 0.1  # seconds

DISCONNECTED = 0
CONNECTED = 1
MUTED = 2
DEAFENED = 3

logger = logging.getLogger()
logger.setLevel(logging.INFO)


class DiscordListener:
    def __init__(self):
        self.client = discord.Client()
        self.config = configparser.ConfigParser()
        if not os.path.isfile("config.ini"):
            copyfile("default.ini", "config.ini")
        self.config.read("config.ini")
        self.username = self.config['LoginInfo']['Username']
        self.password = self.config['LoginInfo']['Password']

        self.port = self.get_arduino_port()
        self.ser = serial.Serial(self.port, timeout=0)

        self.threads = dict()
        self.sched = sched.scheduler(time.time, time.sleep)
        self.sched.enter(REFRESH_RATE, 1, self.update_status)
        sched_thread = threading.Thread(target=self.sched.run, daemon=True)
        sched_thread.start()
        self.threads["sched_thread"] = sched_thread

        def run_tray_icon(): SysTrayIcon.SysTrayIcon('tray_icon.ico', 'DesktopBuddy', (),
                                                     on_quit=lambda *_: self.exit())

        tray_thread = threading.Thread(target=run_tray_icon, daemon=False)
        tray_thread.start()
        self.threads["tray_thread"] = tray_thread

        self.voice_state = DISCONNECTED
        self.attempt_login()

    @staticmethod
    def get_arduino_port():
        logging.info("Finding Arduino port...")
        ports = list(serial.tools.list_ports.comports())
        for p in ports:
            if 'arduino' in p.description.lower():
                logging.info("Done")
                return p.device
        raise Exception("No Arduino found on any COM ports")

    def get_client_state(self):
        state = DISCONNECTED
        if self.client.is_logged_in:
            for server in self.client.servers:
                mem = server.get_member(self.client.user.id)
                vs = mem.voice
                if vs.voice_channel is not None:
                    if vs.deaf or vs.self_deaf:
                        state = DEAFENED
                        break
                    elif vs.mute or vs.self_mute:
                        state = MUTED
                        break
                    else:
                        state = CONNECTED
                        break
        return state

    def update_status(self):
        state = self.get_client_state()
        if self.voice_state != state:
            self.voice_state = state
            self.ser.write(str(state).encode())
        if self.ser.in_waiting:
            if self.ser.read() == b'\x00':
                hotkey('pause')
        self.sched.enter(REFRESH_RATE, 1, self.update_status)

    def attempt_login(self):
        logging.info("Attempting to log in as {}".format(self.username))
        if self.threads.get("client_thread"):
            self.client.logout()
        client_thread = threading.Thread(target=self.client.run, args=(self.username, self.password), daemon=True)
        client_thread.start()
        self.threads["client_thread"] = client_thread
        logging.info("Login successful!")

    def exit(self):
        logging.info("Exiting...")
        self.ser.close()
        sys.exit()


def main():
    listener = DiscordListener()
    atexit.register(listener.exit)


if __name__ == "__main__":
    while True:
        try:
            main()
        except:
            pass
        else:
            break
