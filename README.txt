Rodrick Moore - rmoore@wpi.edu


Platform: Windows


Directory Structure: 
- Remixed menu music and additional SFX in /sounds
- New sprites in /sprites
- New header/cpp files for new Slash Mechanic


--Controls--
** 'W' and 'S' to move up and down
** 'Left Click' to shoot
** 'Right Click' activates Dimensional Slash (resets every 600 steps)
** 'Space' activates a nuke that kills all Saucers within the window (only 1)
** 'F:10' to pause the game (game resumes once another button 
   other than the pause button is pressed)
** 'Q' to self-destruct (activates Game Over screen, returns to title)


--Dimensional Slash--

* Once dimensional slash is activated, movement is disabled, and
  a list of arrow inputs will appear in the middle of the screen 
  based on how many Saucers are visible in the window.

* The number of Saucers destroyed will depend on how many 
  correct inputs the player makes

* If the player messes up an input, input is disabled

*** Arrow Reference ***
'^^' == Press 'W' (up)

'<-' == Press 'A' (left)

'vv' == Press 'S' (down)

'->' == Press 'D' (right)