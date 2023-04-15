# BubbleSpace

Hello, in this plugin a BubbleSpaceComponent. The goal of this component is to calculate an area around the actor it is attached to and use it to modify a reverb (or a general game parameter) in order to dinamically adapt to the environment.

GIF

You will find a window and a few commands for debug aswell.

### Features:
- Use a Width and a Height value to adapt your reverb at runtime
- Set a custom raycast width distance and raycast height distance in order to create a custom area
- Draw the raycast in order to have a visual response
- Draw a cone shape to better understand what kind of area you are going to have
---

### Usage:
- Download the content from LINK
- Drag and drop the folder inside your plugin folder

![Cattura](https://user-images.githubusercontent.com/70896924/232243301-7dfcb76d-27df-4e52-9c24-e20f15bc868a.png)

(If you haven’t this folder create a plugin following this guide to create it: https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/Plugins/)
- Add the string “AudioSplinePlugin” to your Build.cs file like this

![Cattura3](https://user-images.githubusercontent.com/70896924/232243575-bb49994a-c182-43dd-969e-4c94c8586de7.png)

- In Unreal Engine Content Browser
- In the folder Plugin/BubbleSpaceComponentPluginC++Classes/BubbleSpaceComponentPlugin/Public you will find the Actor
- In the folder Plugin/BubbleSpaceComponentPluginContent you will find the Blueprint.
- Choose the right one for you
- Attach it to your Actor (in this example I'm attaching it to the BP_ThirdPersonCharacter)

![Cattura7](https://user-images.githubusercontent.com/70896924/232243913-194cc42b-0e9c-4f19-9a82-63ccff4d6e80.png)

- Now you can use the functions GetWidthValueTarget() and GetHeightValueTarget() to get a 0-1 value that change based on environment, use it to modify a reverb volume value like me in the video example or for another kind of parameter

---

### Properties:

- HorizontalRaysDistance: The raycast width length
- VerticalRaysDistance: The raycast height length
- DrawHorizontalRays: If true the component is drawing the horizontal raycast
- DrawVerticalRays: If true the component is drawing the vertical raycast
- DrawShape: If true the component is drawing the cone shape that represent the area
---

### Functions:

- GetWidthTargetValue(): return a 0-1 value that represent how much space your actor has around him, in the horizontal plane, based on the HorizontalRaysDistance value
- GetHeightTargetValue(): return a 0-1 value that represent how much space your actor has around him, in the vertical plane, based on the VerticalRaysDistance value
- GetHorizontalRaysDistance(): return the horizontal raycast width length
- SetHorizontalRaysDistance(const float InHorizontalRaysDistance): set your custom horizontal raycast width length
- GetVerticalRaysDistance(): return the vertical raycast width length
- SetVerticalRaysDistance(const float InVerticalRaysDistance): set your custom vertical raycast width length
- IsDrawingHorizontalRays(): return true if the component is drawing the horizontal raycast
- ShouldDrawHorizontalRays(const bool InShouldDrawHorizontalRays): set true if you want to draw the horizontal raycast
- IsDrawingVerticalRays(): return true if the component is drawing the vertical raycast
- ShouldDrawVerticalRays(const bool InShouldDrawVerticalRays): set true if you want to draw the vertical raycast
- IsDrawingShape(): return true if the component is drawing the cone shape
- ShouldDrawShape(const bool InShouldDrawShape): set true if you want to draw the cone shape
- ToggleShape(): toggle the ShouldDrawShape, if it’s true will became false and viceversa
- ToggleDrawHRays(): toggle the ShouldDrawHorizontalRays, if it’s true will became false and viceversa
- ToggleDrawVRays(): toggle the ShouldDrawVerticalRays, if it’s true will became false and viceversa

--- 

### Window:

![Cattura5](https://user-images.githubusercontent.com/70896924/232245963-a21fced6-583e-460d-9b51-6720b6372447.png)

- Toggle Draw Shape: Call ToggleShape() 
- Toggle Draw Horizontal Rays: Call ToggleDrawHRays()
- Toggle Draw Vertical Rays: ToggleDrawVRays()
- Horizontal Rays Distances: Use this slider to change the HorizontalRaysDistances at runtime and modify your Actor's area
- Vertical Rays Distances: Use this slider to change the VerticalRaysDistances at runtime and modify your Actor's area
- Horizontal Distance Value: Show the HorizontalRaysDistances value
- Vertical Distance Value: Show the VerticalRaysDistances value
- Width Value: Show the 0-1 value that is an avarage of the various horizontal raycast around your actor
- Height Value: Show the 0-1 value that is an avarage of the various vertical raycast around your actor
---

### Commands:

- BubbleSpace Tab: Open the BubbleSpaceComponent window
- BubbleSpace DrawShape: Call ToggleShape() 
- BubbleSpace DrawHRays: Call ToggleDrawHRays()
- BubbleSpace DrawVRays: ToggleDrawVRays()
---
