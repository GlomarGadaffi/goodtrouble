# Spectrum Sit-In Beacon

A small ESP32 firmware for **expressive RF protest** — a "sit-in" in the
unlicensed 2.4 GHz commons. Each device broadcasts one legible message:
a shared chant (PREFIX) plus your own sign (TAIL), stamped with a shared,
made-up, locally-administered OUI so a crowd reads as one collective signal.

## What this IS
- Your own radio, broadcasting your own packet, in a band anyone may use.
- A digital picket sign. Recognizable, **loggable on purpose**, signed not hidden.
- Spectrum-polite: low TX power, ordinary advertising interval.

## What this is NOT (and must not become)
- **Not a jammer / flood.** Raising power or duty cycle to "fill" the band is a
  blockade, not a sit-in. It's illegal RF interference and it stomps your own
  crowd's beacons first. The code is tuned to coexist; keep it that way.
- **Not impersonation.** The OUI is locally-administered (made-up, self-assigned).
  Do NOT swap in a real vendor's OUI, a camera-vendor OUI, or clone a nearby
  real SSID. Display your own mark, not someone else's identity.
- **Not a cloak.** This does not hide you or blind any collection system. It is
  the opposite: you are choosing to be seen. Understand that before running it.

## Consent
A sit-in works because people consent to being seen doing it. Anyone flashing
this is holding up a sign in the open. That's the point. Make the choice knowingly.

## Files
- `good_trouble/good_trouble.ino` — BLE beacon, message set in code.
- `good_trouble_config/good_trouble_config.ino` — BLE beacon with a set-once
  Wi-Fi config portal (no toolchain needed to set your message; config surface
  turns off after saving).
- `FLASH_GUIDE.md` — plain-language flash-and-go for non-technical folks.

## Boundaries kept in code on purpose
- `TX_POWER = ESP_PWR_LVL_N12` (low — be one of many, not loud)
- ~100 ms advertising interval (ordinary, not a flood)
- rotating lower-MAC under a FIXED shared OUI (legible collective, not ghosts)

MIT-style: do what you want, own what you send.
