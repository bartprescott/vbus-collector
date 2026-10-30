# vbus-collector

`vbus-collector` reads VBus serial data from a Resol DeltaSol BS Plus controller,
checks and decodes packets, and prints selected system values to standard
output.

## Hardware

Connect the controller's VBus interface to a serial port that this program can
read. One example setup is a USB RS422/485-to-serial adapter connected to the
controller; the adapter then appears as a serial device on the computer.

Use an adapter and wiring suitable for the controller's VBus interface, and
follow the controller and adapter documentation for the connections. The
program configures the serial port for **9600 baud, 8 data bits, no parity,
1 stop bit (8N1)**.

Typical serial device paths include:

- Linux: `/dev/ttyUSB0` or `/dev/ttyACM0`
- macOS: `/dev/cu.usbserial-XXXX` or `/dev/cu.usbmodemXXXX`

The device name depends on your adapter. Check your operating system's device
list if the examples do not match.

## Build

You need `make` and a C compiler such as GCC. From the repository directory:

```sh
make
```

This creates the `vbus-collector` executable.

## Run

Pass the serial device path as the first argument:

```sh
./vbus-collector /dev/ttyUSB0
```

By default, the program exits after it receives and prints one valid packet.
Pass any second argument to keep reading and printing packets until interrupted:

```sh
./vbus-collector /dev/ttyUSB0 continuous
```

Replace `/dev/ttyUSB0` with the device path for your adapter. You may need
permission to access the serial device.

## Output

Each valid packet produces a line on standard output with the system time,
four sensor temperatures, two pump speeds, and operating hours for two relays.
For example (values shown are illustrative):

```text
System time:12:34, Sensor1 temp:21.3C, Sensor2 temp:18.7C, Sensor3 temp:20.1C, Sensor4 temp:19.8C, Pump speed1:50%, Pump speed2:40%, Hours1:1234, Hours2:987
```

The program currently prints these selected values; other fields in the
controller packet are not included in its output.

## Troubleshooting

- If the program cannot open the port, check the device path and your access
  permissions.
- If it opens but prints nothing, check the adapter connection, wiring, and
  that the controller is sending VBus data.
- If using an RS422/485 adapter, verify that it is appropriate for the
  controller's interface and that its bus connections follow the hardware
  documentation.
