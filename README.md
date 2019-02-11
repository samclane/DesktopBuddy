# DesktopBuddy

![](https://i.imgur.com/NESF2KM.gif)

Another iteration on my [DiscordMatrix](https://github.com/samclane/DiscordMatrix) obsession. Displays a little friend on your desktop, with dynamic facial animations. Changes to relevant symbols when connected to a Discord Voice Server. 

I've ditched PyFirmata, as it was running relatively heavy on my host computer. It would be much more efficient to just write the Arduino code myself and remove unnecessary overhead. The clock marquee, while cool, was on such a small display as to be mostly useless. 

# Running the source code

To install the dependencies, run `pip install -r requirements.txt`. 

The default pinout is:
* DIN = 4
* CLK = 6
* CS = 5
* SENSOR = 3

These values can be changed in `Arduino/MatrixBuddy/main.h`.

To login, copy the contents of `default.ini` into a file called `config.ini` in the same folder. Then, change `Token` to your personal Discord Token ([instructions here](https://github.com/appu1232/Discord-Selfbot/wiki/Installation-&-Setup#grab-your-token-from-discord)). Also change the `mute` value under `[Keybinds]` to the desired key ([key names can be found here](https://pyautogui.readthedocs.io/en/latest/keyboard.html#keyboard-keys)).

### Credits

[Happy Icon by Ilaria Bernareggi from the Noun Project](https://thenounproject.com/search/?q=pixel%20smile&i=508957)

[SysTrayIcon.py by Simon Brunning](http://www.brunningonline.net/simon/blog/archives/SysTrayIcon.py.html)

![happy face](https://i.imgur.com/cGgORId.png)