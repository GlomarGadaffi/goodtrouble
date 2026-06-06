# Flash-and-Go (no experience needed)

You need: any ESP32 dev board (~$5-8, e.g. an "ESP32-WROOM DevKit"), a USB cable,
and a computer.

## Option A — set your message in code
1. Install the **Arduino IDE** (arduino.cc).
2. File > Preferences > "Additional boards manager URLs", paste:
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. Tools > Board > Boards Manager > search **esp32** > Install.
4. Open `good_trouble/good_trouble.ino`.
5. Change **one line**: `TAIL` to your own sign. Leave `PREFIX` and `SITIN_OUI`
   as the group agreed — that's the shared part.
6. Tools > Board: pick your ESP32. Tools > Port: pick the one that appears.
7. Click **Upload** (the arrow). When it finishes, it's broadcasting.
8. Off-switch: unplug it.

## Option B — set your message from your phone (no code)
1. Flash `good_trouble_config/good_trouble_config.ino` the same way (steps 1-7),
   without editing anything.
2. On first boot it makes a Wi-Fi network called **good-trouble-setup**. Join it from
   your phone. A page pops up (or open any website to trigger it).
3. Type your sign, hit **set & go**. It saves, the setup network disappears, and
   it starts broadcasting. You can turn your phone's Wi-Fi back off.
4. To change your message later: unplug, hold the **BOOT** button, plug back in
   while holding — the setup network comes back.

## What it's doing
Broadcasting a short Bluetooth advertisement whose name is your message, at low
power, like a tiny picket sign in the airwaves. It is meant to be SEEN. It does
not hide you and does not interfere with anyone's equipment. You're choosing to
be counted — that's the whole idea of a sit-in.

## Don't turn a sit-in into a blockade
- Leave the power LOW and the timing as-is. Don't crank it to "fill" the band —
  that breaks everyone's signal including the rest of the crowd's, and it stops
  being protected expression.
- Keep the made-up shared OUI. Don't impersonate a real brand or clone a real
  nearby network. Your sign is your own, not someone else's name.
- One beacon per person. The crowd is the message; you don't each need to be loud.
