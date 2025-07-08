## 01 & 02 - Init, Code & Assets
- Creating FirstProject in UE5.
- Importing some basic assets and working with them in editor.
- Creating basic actors with assets.
- Importing content from marketplace.
- Setting up Git Large File Storage for Unreal Engine Project.

## 03 - Simple Character & Moving Platform
- Setting up CharacterBase and CharacterControllerBase ready for input and movement via cpp.
- Implementing SCPlatform, scene component for platform along with IActivatesPlatform interface for objects that activate the platform.
- Platform consists of Automatic Mode (unconditional); Overlap Mode (overlap condition); Trap Mode (overlap and on timer sends overlapper off).

## 04 - Enhanced Input System
- Setting up character movement using CharacterMovementComponent.
- Using Enhanced Input Component to take user inputs.
- Adding InputMappingContexts with corresponding InputActions for Moving, Looking, Running, Jumping and Crouching

## 05 - Physics Grab, Throw, Rotate & Freeze Object
- Implementing Physics Interaction between Player and Object using Physics Handle.
- Line Trace Detection using IGrabbable Interface to check if Player can grab hit object.
- Using Player Input to perform interaction
- Implemeting the objects' weight for more accurate interaction, when grabbing and throwing the objects.

## 06 - Components & Refactoring Interaction
- Placing Grab Logic into component UGrabComponent, attatched to Player, containing logic, handling grabbing, releasing, throwing and distance adjustment.
- Creating UGrabbableComponent instead of using IGrabbableInterface to make system more modular and more easily applicable to any actor
- Overall improved code seperation out of the character class, for decluttering purposes and for better modularity.

## 07 - Further Interaction & Event Dispatchers
- Implementing Highlighting of objects using collision volume on player and checking for HighlightableComponent on actor which handles highlighting it.
- Implementing Player-Object interaction with Interactable and Activator - Using Event Dispatchers for communication between them (e.g. Door and button).
- Using IInteractable interface for interactable objects that will allow player to interact and for them to implement the interface functions.
- Using UInteractComponent and UHighlightingComponent to house interaction and highlighting logic from player.
