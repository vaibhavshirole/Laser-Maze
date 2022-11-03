# Laser-Maze

The Bakken Museum is a children's science museum here in Minneapolis. They were looking for student groups to build and bring a project to an event they were hosting for kids to learn about science and engineering. As one of the members of the IEEE UMN board, I had hoped to bring more outreach events to the student group. I was incredibly excited by this project because it allowed me to apply some of the things I'd learned, and teach it to kids who love to learn and are always genuinely curious. The theme had something to do with light and shadows, and I felt a laser maze was a really fun and interactive way to do just that. 

![Quick glamor shot of the project](https://github.com/vaibhavshirole/Laser-Maze/blob/main/bakken-mesh/images/20221029_134411.JPG)

The execution was fairly straightforward. Each ESP32 was assigned a "device type" of either a laser, photoresistor, or button. The laser nodes contain 15 laser diodes plugged into the available GPIO pins on the ESP32. Photoresistor nodes sit on the other end of the laser beam, and catch those lasers with a photoresistor. When the laser beam has broken, the photoresistor's resistance will drop and the system will dsiable by turning off the lasers. This is the feedback for the kids playing in the maze. After 2 seconds, the system goes through the code, checking states, in an attempt to re-enable the system. It will re-enable if the laser makes contact with the face of the photoresistor.

Finally, there is a that 3rd device type, a button node, which is how kids would beat the game. If they get through all the laser beams, they push the button, the system turns off, and they win the game! This is a very intuitive game built on mesh communication and helps show a kids a couple things. First, you can make incredible and fun things with just a few tiny and affordable computers. Also, laser beams become visible if theres dust, smoke, or steam in the air. 

This was all set up in a really makeshift way. The laser nodes were attached onto an old coat hanger, and photoresistors were placed along the perimeter of the space. In order to get the laser beams to show, we put the entire operation inside a large tent directly outside the museum doors. 

![The laser maze](https://github.com/vaibhavshirole/Laser-Maze/blob/main/bakken-mesh/images/20221029_141320.jpg)

Along with the laser maze, I demoed some the basics of it at a table away from the main laser maze. This came as a precursor so that kids knew exactly what was going on before going into the tent and having fun. 

![Little demo outside of the laser maze](https://github.com/vaibhavshirole/Laser-Maze/blob/main/bakken-mesh/images/20221029_141056.JPG)

Although this was a ridiculously fun project, I want to improve it so that its easier to set up. I had organized a group of volunteers to help with the setup, and although I had 2-3 other pairs of hands at all times, it took around 2 hours to set up. Instead of having a photoresistor node, this project could potentially be simplified by having some distance sensor with the laser nodes so that there's no receiver to set up.
