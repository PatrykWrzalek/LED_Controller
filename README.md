# LED_Controller Project

## Project Summary

**LED_Controller** is a versatile ESP32-C6 based project designed for advanced LED strip control. The system integrates multiple LED types, including light **COLD**, **WARM** (PWM controlled), and **WS2812 addressable RGB LEDs**, and can be controlled via both **IR remote (NEC protocol)** and **smartphone over WiFi**.  

Key project features:
- Multi-tasking architecture using **FreeRTOS**.
- LED control abstraction for PWM and WS2812 LEDs.
- IR remote control (NEC standard) with event-driven handling.
- WiFi connectivity with **HTTP API** for remote LED control.
- Persistent settings storage using **NVS**, allowing saved color and brightness presets.
- Over-the-Air (**OTA**) updates for firmware maintenance.
- Debugging infrastructure including **Console** and **GDB Stub**.
- **Watchdog** integration to prevent system hangs.
- Optional MQTT / RainMaker integration for cloud-based control.

The project is structured with **modular components**, making it easy to extend or adapt for additional hardware or features.  

---

## Project Development Roadmap

Track progress with the checkboxes below:

- [x] Hello World / ESP32-C6 Setup  
- [ ] Console setup for early debugging  
- [ ] Watchdog integration  
- [ ] FreeRTOS task architecture & event queues  
- [ ] PWM LED control (light COLD & WARM)  
- [ ] WS2812 LED control (RMT)  
- [ ] IR NEC remote control  
- [ ] NVS settings storage  
- [ ] WiFi station connection  
- [ ] HTTP Server / API endpoints  
- [ ] Web-based control UI  
- [ ] OTA firmware updates  
- [ ] MQTT / RainMaker integration (optional)

---

## Recommended Architecture

- **led_task** — handles all LED output (PWM + WS2812)  
- **ir_task** — decodes IR commands and sends events to led_task  
- **wifi_task** — manages WiFi connectivity  
- **web_server_task** — provides HTTP API and web UI  
- **settings_task** — handles NVS read/write  
- **ota_task** — manages OTA updates  
- **watchdog_task** — monitors system health  
- **console_task** — allows command-line interaction and debugging  

Communication between tasks is fully **event-driven** via FreeRTOS queues, ensuring safe and efficient task execution.

---

## Hardware

- ESP32-C6 (Seeed Studio development board recommended)  
- LED Types:
  - COLD LED (PWM)
  - WARM LED (PWM)
  - WS2812 addressable RGB LED strip
- IR receiver (NEC protocol)

---

## Development Notes

- Focus on building **Console + Watchdog** early for stable debugging.  
- Use **LEDC example** for PWM LEDs and **RMT/LED Strip example** for WS2812.  
- Leverage **NVS** to store user preferences like default color and brightness.  
- Start with **HTTP + Web UI**, later consider **MQTT/RainMaker** for cloud integration.  
- OTA updates should only be tested once system is stable.

---

This README provides a clear project overview and a step-by-step development plan for anyone who wants to contribute or replicate the project.
