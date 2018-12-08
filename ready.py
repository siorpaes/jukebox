#!/usr/bin/python

import RPi.GPIO as GPIO
import subprocess
from time import sleep

led = 25;

GPIO.setmode(GPIO.BCM);
GPIO.setup(led, GPIO.OUT);

while True:
    GPIO.output(led, True);
    sleep(0.1);
    GPIO.output(led, False);
    sleep(1.9);

