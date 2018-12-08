#!/usr/bin/python

import RPi.GPIO as GPIO
import subprocess
from time import sleep

GPIO.setmode(GPIO.BCM);
GPIO.setup(24, GPIO.OUT);

while True:
    GPIO.output(24, True);
    sleep(0.1);
    GPIO.output(24, False);
    sleep(1.9);

