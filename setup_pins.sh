#!/bin/sh
echo 248 > /sys/class/gpio/export
echo 249 > /sys/class/gpio/export
echo 214 > /sys/class/gpio/export
echo 216 > /sys/class/gpio/export
echo 217 > /sys/class/gpio/export
echo low > /sys/class/gpio/gpio214/direction
echo high > /sys/class/gpio/gpio249/direction
echo low > /sys/class/gpio/gpio248/direction
echo in > /sys/class/gpio/gpio216/direction
echo in > /sys/class/gpio/gpio217/direction
echo high > /sys/class/gpio/gpio214/direction
