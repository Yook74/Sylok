# Project Evergreen
Software specification  
Andrew Blomenberg, 10/13/2017    
Status: Incomplete

## Overview
Evergreen is an expansion of the clipboard in Android. It will allow it's users to copy and paste more than one string at a time. Evergreen will remember the last few copies and allow the user to select which of them to paste.

## General specifications
| What | Priority | Status | Time estimate (hr) | 
|-|-|-|-| 
| The application will have a name other than EVERGREEN | med | not started | 1 | 
| The application will have an associated launcher icon that can be used without copyright claims. | med | not started | 3 |
| The application will appear in the top ten apps listed when a user searches for “copy and paste” or “clipboard” in the Google play store. | low | not started | | 
| EVERGREEN will operate with full functionality on any phone running Android 4.2 or higher | high | not started | 2 | 
| EVERGREEN will operate with full functionality on any tablet running Android 4.2 or higher | low | not started |  | 
| If EVERGREEN is installed on a Pixel XL, the resulting slowdown and battery drain will not be noticable | high | not started | n/a | 
| EVERGREEN will be developed using the native Android libraries | high| not started | n/a | 
| EVERGREEN will not be designed with Titanium or HTML | high | not started | n/a | 
| EVERGREEN will not require an internet connection for any features | med | not started | n/a | 
| EVERGREEN will not request any permissions that are not completely necessary for the functionality of the application. | low  | not started | n/a | 

## Interface Specifications
Evergreen has two primary interfaces: a buffer interface and a prefs interface.
### Buffer Interface
| Behavior | Priority | Status | Time estimate (hr) | 
|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------|-------------|--------------------| 
| The buffer interface will be accessible in no more than three taps from any application that does not hide the notification bar | high | not started | 4  |
| The buffer interface will not steal application focus from the current application, instead, the buffer interface will be accessed within the notification menu or within a floating menu | high | not started | | 
| The buffer interface will provide the user with a list of most recent clippings. | high | not started | 3 | 
| The buffer interface will allow the user to pin a clipping. A pinned clipping will be accessible in the buffer interface until it is unpinned | low | not started | 5 | 
| The buffer interface will only display strings that have been copied. Images will not be displayed. | high | not started | | 
| Clippings stored in the buffer interface will be truncated when they are too long to be entirely visible in the buffer menu. | high | not started | 1 | 
| When a item is selected in the buffer interface, it will be placed into Andoid system clipboard and not directly pasted in | high | not started | 5 | 
| When the user copies an item, the item will be diplayed in the buffer interface | high | not started | 5 | 
| EVERGREEN will persistently display a notification that can be used to access the butter interface | high | not started | 4 | 
| Pinned items will appear in a different list than the recently copied items | med | not started | 2  | 
### Prefs Interface
| Behavior | Priority | Status | Time estimate (hr) | 
|-|-|-|-| 
| EVERGREEN will have at least one configuration option.| med | not started | 2 | 
| Configuration options will be accessible via a separate interface called “prefs.” | high | not started | 2 | 
| The prefs interface will allow the user to select how many clippings to save in the buffer at one time | med | not started | 1 | 
| The prefs interface will use a UI theme which is popular in other Android configuration menus | low| not started | 1 | 
| The prefs interface will be as simple as possible while still presenting the required information | med | not started | 1 | 



