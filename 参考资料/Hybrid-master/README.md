# Hybrid
Hearth rate variability research using STM32F103 and MAX30100.

For a Polish description see [here](https://forbot.pl/forum/topic/12765-blue-pill-jako-czujnik-hrv/).

## Introduction - what is HRV?
Have you known that the time between succesive hearthbeats almost never is the same? And I'm not talking about long-term changes (you start running - your pulse increases), but about very quick ones, changes you can only see measuring times between each two hearthbeats. Look at this image:

![HRV plot](https://github.com/Moderin/Hybrid/blob/master/17.12.18.png)

This plot shows how does my pulse change (red line). X-axis consists of the numbers of hearthbeat and Y-axis shows the pulse. As you can see in this case my pulse was **increasing and decreasing each breath**. Looks like the hearth is synchronized with the breathing. This is named **cardiac coherency**. Interesting? When using pulsometer you only see the yellow line - averaged pulse in some time. Well, now there's something even more interesting:

![HRV comparision](https://github.com/Moderin/Hybrid/blob/master/cmp2.png)

Here you can see to plots. The first one is chaotic - it shows how my pulse changes during stressful situation. The second one shows the opposite - when I'm calm and relaxed.


## Technical stuff
This project uses STM32F103 microcontroller (Bluepill board) and MAX30100 sensor. Software is written in bare-metal concept, using C++ and some assembly (vector table). MAX30100 simply checks how much light goes through your skin. And that depends of the blood pressure, that changes every hearthbeat. This allows us to detect pulse:

![pulse detection](https://github.com/Moderin/Hybrid/blob/master/beat_detect2.png)

Now we just need to measure the time between beats and send it to pc.
