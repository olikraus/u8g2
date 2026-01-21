## Raspberry Pi Pico / RP2040 HAL Example

This directory contains an example **HAL implementation for Raspberry Pi Pico–class boards**
using the official Raspberry Pi Pico SDK.

This example is provided in response to the request made in
[u8g2 issue #2159](https://github.com/olikraus/u8g2/issues/2159), where example HAL
implementations were explicitly welcomed.

Although the display controller used here differs from the one discussed in that issue,
the overall structure and integration approach are the same and can be adapted to other
RP2040-based systems.

### Scope and Intent

- Demonstrates how to integrate u8g2 with the Raspberry Pi Pico SDK
- Provides a minimal HAL layer for GPIO, delay, and communication callbacks
- Intended as a **reference implementation**, not a fully generic or complete port
- Meant to be copied and adapted for specific projects

### Tested Hardware

- Raspberry Pi Pico (RP2040)
- Raspberry Pi Pico 2 (RP2350)

The same HAL implementation was tested on both platforms and works without modification.

### Notes

- No existing u8g2 HAL implementations are modified
- The example is self-contained and does not affect other platforms
- Display interface and controller specifics are intentionally kept minimal
