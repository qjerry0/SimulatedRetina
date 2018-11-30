========================================================================
Overview:

In order to use the Retina model, there are two separate projects for the two separate programs. The first is the retina generation project, which generates a virtual retina and allows the user to input images and receive the resulting activatinos in a text file output. The second is the retina output viewer project, which takes in the text file outputs of the retina generation program and can display the individual receptors for a given channel and a given zoom level.

========================================================================
Retina Generation Program:

The retina generation program is a console application which takes in parameters set by the user in the source code (not in the console) and generates a retina. It runs using the following process:

1) It segments the retina into small square regions. 

2) It calculates the approximate denisty that each region should have based on its distance from the center of the retina. 

3) Based on the size of the region and the density, an appropriate number of photoreceptors is generated for each region.

4) Based on the size of the region and the density, an appropriate number of opponents is generated for each region.

5) The opponents and photoreceptors are assigned random locations within the small region.

6) The opponents are connected to nearby photoreceptors based on the range of the opponent cell, which depends on distance from the center of the retina.
Note: This is the rate-limiting step

7) The program prompts the user for a file name and generates the appropriate actiavtions for both the photoreceptor and the opponent layer. The opponent activations are saved to a text file output. A small picture of the generated cones and their activations is also generated.
Note: The program will then ask the user if they want to input a different image, and if so will re-run the above step. This will overwrite the old output. 

The program parameters allow the user to specify the density distributions of the photoreceptors, the density distributions of the opponents, the number of the small regions of constant density (labeled "size"), the minimum receptive field size, maximum receptive field size, surround constant, and the percent of the retina modeled (WIP).

Program Variable Values for Full-Scale Retina:
Factory::size = 512.0f
Factory::maxDensityPhotoreceptor = 1000.0f
Factory::minDensityPhotoreceptor = 4.0f
Factory::spreadParameterPhotoreceptor = 200.f
Factory::maxDensityOpponent = 100.0f
Factory::minDensityOpponent = 1.0f
Factory::spreadParameterOpponent = 64.0f
Factory::percentRetinaModeled = 1.0f
Opponent::SURROUND_CONSTANT = 2.0f
Opponent::OPPONENT_MAX_FIELD_SIZE = 1.0f
Opponent::OPPONENT_MIN_FIELD_SIZE = 0.125f

Note: Surround Constant does not alter retina generation

========================================================================
Retina Output Viewer:

The retina output viewer is a console application that uses the SFML graphics library. It allows the user to view one of the three opponent channels and switch between the channels at will. It allows the user to zoom in and zoom out to see the differences in the periphery and the fovea of the retina. Finally, it allows the user to save the current displayed opponents to an image.

The program currently only uses the file in the input folder and does not take any user input for selecting the file. Thus, to change files, one must switch the file in the input folder with another activation output file (not an ideal file IO system but it works). Once the file is found, it will begin recreating the retina from the outputs and will display the number of opponent cells inserted. When all of the opponents have been inserted, the program will open a window and, after some computation, dislpay the full retina luminance channel outputs. 

Controls:
1 - switches the channel being displayed to "Luminance" (if already on "Luminance", has no effect)
2 - switches the channel being displayed to "RedGreen" (if already on "RedGreen", has no effect) 
3 - switches the channel being displayed to "BlueYellow" (if already on "BlueYellow", has no effect)
z - allows the user to zoom into the retina; in the console, user must enter:
	a) the zoom amount (0.5 zooms out to 50% of the current zoom; 2.0 zooms in to 200% of the current zoom)
	b) the zoom x location (to get a point 1/4 from the left of the screen, enter 0.25)
	c) the zoom y location (to get a point 1/4 from the top of the screen, enter 0.25)
	Note: Zooming can also be done with the mouse, but it is painfully slow
c - allows the user to save the image currently displayed on the window
b - changes the background color to black
w - changes the backgorund color to white
g - changes the background color to gray

========================================================================

Program Notes:

Retina Program takes very long to run at full capacity, took around half a day to a day for the settings listed above. Output Viewer takes a fair amount of time to finish, and if you know how many opponents were created with the retina generator, then you can gauge how far along the loading is by looking at the number of opponents inserted. 

/////////////////////////////////////////////////////////////////////////////


