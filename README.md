# Hexapod

Seems like you want to build a Hexapod. Don't know why but since you're here, might as well tell you about the most important features about my design:

Cheap: 120 € (see Parts (toBuy) file)
RGB: 271 individual RGB LEDs
Cable management: Cables are routed inside the leg. no cable loop that tangle
ORP compatible: compatible with the Open Robotic Platform modules
Bluetooth control: Remote control via phone app

If this hasn't convinced you might I suggest watching by videos on the design and build.







Since you are reading this I assume you are at least considering my design. I have some instructions on Instucatbles if you want to take a look at that.

All the 3mf files are oriented correctly with the needed support material. My parts are design in a way that uses minimal support material. See “Parts (toPrint)” Excel sheet.



If you don't know what and where to buy take a look at the "Parts (toBuy)" Excel sheet. Those are the parts I bought and where. You can also update the prices and it will automatically calculate the final price. Everything I listed is without shipping since that varies drastically.

If I didn't buy it or the link doesn't work anymore I linked to where I would buy.



If you want the Code or remote App take a look at the Github or Instructables page. I have uploaded them here but the files on GitHub will be the most up to date.



To be clear, my design is NOT perfect. Here are all the problems I have:

highly integrated leg design: routing the cables inside the leg looks awesome but makes assembly more difficult. If you were to not use LEDs this problem would largely be negated.
Current spikes: the hexapod is able to handle some load but especially at times of a power spike the Buck converter powering the servos can be insufficient. So you might want to use two or buy a more powerful one.
I2C buss instability: I had problems with the I2C buss between the STM32 and servo drivers breaking down causing the Hexapod to freeze. I was able to fix that by using shielded cables, but it required some trial and error.
incomplete Code: Not all features have been added in code. For example no LED animations beside the startup animation.



Seeing the project work is the most rewarding thing for me. I know most people won't build something as complex as this but I still hope I was able to inspire you to build your own robotic Projects.

If you have any questions consider writing me a comment, preferably on YouTube. That is the place I will most likely see it.

Happy Making :)
