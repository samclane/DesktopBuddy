# DesktopBuddy

![happy face](https://i.imgur.com/cGgORId.png)

Another iteration on my [DiscordMatrix](https://github.com/samclane/DiscordMatrix) obsession. Displays a little friend 
on your desktop, with dynamic facial animations. Changes to relevant symbols when connected to a Discord Voice Server. 

I've ditched PyFirmata, as it was running relatively heavy on my host computer. It would be much more efficient to just write the Arduino code myself and remove unnecessary overhead. The clock marquee, while cool, was on such a small display as to be mostly useless. 

# Running the source code

To install the dependencies, run `pip install -r requirements.txt`. 

The default pinout is:
* dataIn = 2
* load = 4
* clock = 3

These values can be changed in your `config.ini` which appears after running the code once. 

### Credits

[Happy Icon by Ilaria Bernareggi from the Noun Project](https://thenounproject.com/search/?q=pixel%20smile&i=508957)

[SysTrayIcon.py by Simon Brunning](http://www.brunningonline.net/simon/blog/archives/SysTrayIcon.py.html)