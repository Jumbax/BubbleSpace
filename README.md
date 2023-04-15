# BubbleSpace

Hello, in this plugin a BubbleSpaceComponent. The goal of this component is to calculate an area around the actor it is attached to and use it to modify a reverb (or a general game parameter) in order to dinamically adapt to the environment.

GIF

You will find a window and a few commands for debug aswell.

### Features:
- Use a Width and a Height value to adapt your reverb at runtime
- Set a custom raycast width length and raycast height length in order to create a custom area
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

--- 

### Window:

---

### Commands:

---
